// Copyright 2020 Thinking Data Ltd. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Containers/StringConv.h"
#include <jni.h>

namespace thinkinganalytics
{
    enum class EJavaExceptionPolicy
    {
        ClearAndLog,
        Propagate
    };

    // Owns only local references created by this call. Cleanup remains legal with a pending exception.
    class FJavaCallContext
    {
    public:
        FJavaCallContext(JNIEnv* InEnv, const char* InOperation, EJavaExceptionPolicy InPolicy = EJavaExceptionPolicy::ClearAndLog)
            : Env(InEnv), Operation(InOperation), Policy(InPolicy)
        {
            if (!Env)
            {
                Fail(TEXT("JNIEnv is unavailable"));
            }
            else
            {
                CheckException(TEXT("entry"));
            }
        }

        FJavaCallContext(const FJavaCallContext&) = delete;
        FJavaCallContext& operator=(const FJavaCallContext&) = delete;

        ~FJavaCallContext()
        {
            for (jobject Ref : LocalRefs)
            {
                Env->DeleteLocalRef(Ref);
            }
        }

        bool IsValid() const { return !bFailed; }

        jmethodID FindMethod(jclass Class, const char* Name, const char* Signature)
        {
            if (!IsReady()) return nullptr;
            if (!Class)
            {
                Fail(TEXT("Java class is unavailable"));
                return nullptr;
            }
            jmethodID Method = Env->GetMethodID(Class, Name, Signature);
            if (CheckException(TEXT("GetMethodID"))) return nullptr;
            if (!Method) Fail(TEXT("GetMethodID returned null"));
            return Method;
        }

        jstring NewString(const FString& Value)
        {
            if (!IsReady()) return nullptr;
            // JNI's Modified UTF-8 differs from standard UTF-8. Pass UTF-16 with its explicit length.
            const FTCHARToUTF16 Converted(*Value, Value.Len());
            jstring String = Own(Env->NewString(reinterpret_cast<const jchar*>(Converted.Get()), Converted.Length()));
            if (CheckException(TEXT("NewString"))) return nullptr;
            if (!String) Fail(TEXT("NewString returned null"));
            return String;
        }

        // Borrows String, including Java callback parameters; ownership stays with the caller.
        FString ReadString(jstring String)
        {
            if (!IsReady() || !String) return FString();
            const jsize Length = Env->GetStringLength(String);
            if (CheckException(TEXT("GetStringLength"))) return FString();
            const jchar* Chars = Env->GetStringChars(String, nullptr);
            const bool bException = CheckException(TEXT("GetStringChars"));
            if (bException || !Chars)
            {
                if (Chars) Env->ReleaseStringChars(String, Chars);
                if (!bException) Fail(TEXT("GetStringChars returned null"));
                return FString();
            }

            const FUTF16ToTCHAR Converted(reinterpret_cast<const UTF16CHAR*>(Chars), Length);
            FString Result(Converted.Length(), Converted.Get());
            Env->ReleaseStringChars(String, Chars);
            if (CheckException(TEXT("ReleaseStringChars"))) return FString();
            return Result;
        }

        jobjectArray NewStringArray(const TArray<FString>& Values)
        {
            if (!IsReady()) return nullptr;
            jclass StringClass = Own(Env->FindClass("java/lang/String"));
            if (CheckException(TEXT("FindClass"))) return nullptr;
            if (!StringClass)
            {
                Fail(TEXT("FindClass returned null"));
                return nullptr;
            }
            jobjectArray Array = Own(Env->NewObjectArray(Values.Num(), StringClass, nullptr));
            if (CheckException(TEXT("NewObjectArray"))) return nullptr;
            if (!Array)
            {
                Fail(TEXT("NewObjectArray returned null"));
                return nullptr;
            }
            for (int32 Index = 0; Index < Values.Num(); ++Index)
            {
                jstring String = NewString(Values[Index]);
                if (!IsValid()) return nullptr;
                Env->SetObjectArrayElement(Array, Index, String);
                const bool bException = CheckException(TEXT("SetObjectArrayElement"));
                Env->DeleteLocalRef(Detach(String));
                if (bException) return nullptr;
            }
            return Array;
        }

        void CallVoid(jobject Object, jmethodID Method, const jvalue* Args)
        {
            if (!CanCall(Object, Method)) return;
            Env->CallVoidMethodA(Object, Method, Args);
            CheckException(TEXT("CallVoidMethod"));
        }

        jobject CallObject(jobject Object, jmethodID Method, const jvalue* Args)
        {
            if (!CanCall(Object, Method)) return nullptr;
            jobject Result = Own(Env->CallObjectMethodA(Object, Method, Args));
            if (CheckException(TEXT("CallObjectMethod"))) return nullptr;
            return Result;
        }

        jvalue Argument(const FString& Value) { jvalue Arg{}; Arg.l = NewString(Value); return Arg; }
        jvalue Argument(const TArray<FString>& Value) { jvalue Arg{}; Arg.l = NewStringArray(Value); return Arg; }
        jvalue Argument(jint Value) { jvalue Arg{}; Arg.i = Value; return Arg; }
        jvalue Argument(jlong Value) { jvalue Arg{}; Arg.j = Value; return Arg; }
        jvalue Argument(bool Value) { jvalue Arg{}; Arg.z = Value ? JNI_TRUE : JNI_FALSE; return Arg; }

        // Transfer a native callback's return reference to Java without deleting it on scope exit.
        template <typename T>
        T Detach(T Ref)
        {
            LocalRefs.RemoveSingleSwap(Ref);
            return Ref;
        }

    private:
        template <typename T>
        T Own(T Ref)
        {
            if (Ref) LocalRefs.Add(Ref);
            return Ref;
        }

        bool CanCall(jobject Object, jmethodID Method)
        {
            if (!IsReady()) return false;
            if (!Object || !Method)
            {
                Fail(TEXT("Java object or method is unavailable"));
                return false;
            }
            return true;
        }

        bool IsReady()
        {
            return IsValid() && !CheckException(TEXT("before JNI operation"));
        }

        bool CheckException(const TCHAR* Stage)
        {
            if (!Env->ExceptionCheck()) return false;
            bFailed = true;
            if (Policy == EJavaExceptionPolicy::ClearAndLog)
            {
                Env->ExceptionClear();
                UE_LOG(LogTemp, Warning, TEXT("TDAnalytics JNI %s: Java exception at %s; call aborted."), UTF8_TO_TCHAR(Operation), Stage);
            }
            return true;
        }

        void Fail(const TCHAR* Reason)
        {
            bFailed = true;
            UE_LOG(LogTemp, Warning, TEXT("TDAnalytics JNI %s: %s; call aborted."), UTF8_TO_TCHAR(Operation), Reason);
        }

        JNIEnv* Env;
        const char* Operation;
        EJavaExceptionPolicy Policy;
        bool bFailed = false;
        TArray<jobject, TInlineAllocator<16>> LocalRefs;
    };
}
