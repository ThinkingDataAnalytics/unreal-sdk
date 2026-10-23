// Copyright 2020 Thinking Data Ltd. All Rights Reserved.

#if TDANALYTICS_ANDROID_TESTS && WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "Android/TDAnalyticsJNIUtils.h"
#include "Android/TDAnalyticsJNI.h"
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#include "TDAnalytics.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

using thinkinganalytics::EJavaExceptionPolicy;
using thinkinganalytics::FJavaCallContext;

extern "C" jstring Java_com_epicgames_ue4_GameActivity_getAutoTrackEventProperties(JNIEnv*, jobject, jstring, jstring, jstring);
extern "C" jstring Java_com_epicgames_unreal_GameActivity_getAutoTrackEventProperties(JNIEnv*, jobject, jstring, jstring, jstring);

namespace
{
    const FString UnicodeValue = TEXT("1A_\U0001F60D\u3002\u6570\u6570 ");

    // Fault injection uses the real JNI function table ABI without exhausting device memory.
    struct FTestJNIEnv : JNIEnv
    {
        JNINativeInterface Functions{};
        bool bPendingException = false;
        bool bFailRead = false;
        bool bFailArray = false;
        bool bFailElement = false;
        bool bFailMethod = false;
        bool bFailCall = false;
        bool bFailWithoutException = false;
        int32 FailStringAt = 0;
        int32 StringsCreated = 0;
        int32 StringsRequested = 0;
        int32 ReferencesDeleted = 0;
        int32 ExceptionsCleared = 0;
        int32 Calls = 0;
        int32 IllegalCalls = 0;
        int32 NextRef = 1;

        static FTestJNIEnv& State(JNIEnv* Env) { return *static_cast<FTestJNIEnv*>(Env); }

        void CheckCall()
        {
            if (bPendingException) ++IllegalCalls;
        }

        template <typename T>
        T Ref() { return reinterpret_cast<T>(static_cast<UPTRINT>(NextRef++)); }

        FTestJNIEnv()
        {
            functions = &Functions;
            Functions.ExceptionCheck = [](JNIEnv* Env) -> jboolean { return State(Env).bPendingException ? JNI_TRUE : JNI_FALSE; };
            Functions.ExceptionClear = [](JNIEnv* Env) { auto& S = State(Env); S.bPendingException = false; ++S.ExceptionsCleared; };
            Functions.DeleteLocalRef = [](JNIEnv* Env, jobject) { ++State(Env).ReferencesDeleted; };
            Functions.NewString = [](JNIEnv* Env, const jchar*, jsize) -> jstring
            {
                auto& S = State(Env);
                S.CheckCall();
                if (++S.StringsRequested == S.FailStringAt)
                {
                    S.bPendingException = !S.bFailWithoutException;
                    return nullptr;
                }
                ++S.StringsCreated;
                return S.Ref<jstring>();
            };
            Functions.GetStringLength = [](JNIEnv* Env, jstring) -> jsize { State(Env).CheckCall(); return 1; };
            Functions.GetStringChars = [](JNIEnv* Env, jstring, jboolean*) -> const jchar*
            {
                auto& S = State(Env);
                S.CheckCall();
                if (S.bFailRead) { S.bPendingException = true; return nullptr; }
                static const jchar Value[] = {'a'};
                return Value;
            };
            Functions.ReleaseStringChars = [](JNIEnv*, jstring, const jchar*) {};
            Functions.FindClass = [](JNIEnv* Env, const char*) -> jclass { auto& S = State(Env); S.CheckCall(); return S.Ref<jclass>(); };
            Functions.NewObjectArray = [](JNIEnv* Env, jsize, jclass, jobject) -> jobjectArray
            {
                auto& S = State(Env);
                S.CheckCall();
                if (S.bFailArray) { S.bPendingException = true; return nullptr; }
                return S.Ref<jobjectArray>();
            };
            Functions.SetObjectArrayElement = [](JNIEnv* Env, jobjectArray, jsize, jobject)
            {
                auto& S = State(Env);
                S.CheckCall();
                if (S.bFailElement) S.bPendingException = true;
            };
            Functions.GetMethodID = [](JNIEnv* Env, jclass, const char*, const char*) -> jmethodID
            {
                auto& S = State(Env);
                S.CheckCall();
                if (S.bFailMethod) { S.bPendingException = true; return nullptr; }
                return S.Ref<jmethodID>();
            };
            Functions.CallVoidMethodA = [](JNIEnv* Env, jobject, jmethodID, const jvalue*)
            {
                auto& S = State(Env);
                S.CheckCall();
                ++S.Calls;
                if (S.bFailCall) S.bPendingException = true;
            };
            Functions.CallObjectMethodA = [](JNIEnv* Env, jobject, jmethodID, const jvalue*) -> jobject
            {
                auto& S = State(Env);
                S.CheckCall();
                ++S.Calls;
                if (S.bFailCall) { S.bPendingException = true; return nullptr; }
                return nullptr;
            };
        }
    };
}

IMPLEMENT_COMPLEX_AUTOMATION_TEST(
    FTDAnalyticsJNIExceptionTest,
    "TDAnalytics.Android.JNI.Exceptions",
    EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

void FTDAnalyticsJNIExceptionTest::GetTests(TArray<FString>& Names, TArray<FString>& Commands) const
{
    Names = {TEXT("ReadFailure"), TEXT("AllocationFailure"), TEXT("NullWithoutException"), TEXT("ArrayFailure"), TEXT("ArrayElementFailure"), TEXT("MissingMethod"), TEXT("JavaObjectException"), TEXT("JavaVoidException"), TEXT("CallbackPropagation"), TEXT("NullEnvironment")};
    Commands = Names;
}

bool FTDAnalyticsJNIExceptionTest::RunTest(const FString& Parameters)
{
    FTestJNIEnv Env;
    const bool bPropagate = Parameters == TEXT("CallbackPropagation");
    if (!bPropagate)
    {
        AddExpectedError(TEXT("TDAnalytics JNI JNIExceptionTest:"), EAutomationExpectedErrorFlags::Contains, 1);
    }

    int32 ExpectedDeletes = 1;
    {
        FJavaCallContext Call(Parameters == TEXT("NullEnvironment") ? nullptr : &Env, "JNIExceptionTest",
            bPropagate ? EJavaExceptionPolicy::Propagate : EJavaExceptionPolicy::ClearAndLog);
        jstring First = Call.NewString(TEXT("first"));
        const jobject Object = reinterpret_cast<jobject>(static_cast<UPTRINT>(100));
        const jmethodID Method = reinterpret_cast<jmethodID>(static_cast<UPTRINT>(101));
        if (Parameters == TEXT("ReadFailure") || bPropagate)
        {
            Env.bFailRead = true;
            TestTrue(TEXT("Failed read returns empty"), Call.ReadString(First).IsEmpty());
        }
        else if (Parameters == TEXT("AllocationFailure") || Parameters == TEXT("NullWithoutException"))
        {
            Env.FailStringAt = 2;
            Env.bFailWithoutException = Parameters == TEXT("NullWithoutException");
            TestNull(TEXT("Failed allocation returns null"), Call.NewString(TEXT("second")));
        }
        else if (Parameters == TEXT("ArrayFailure") || Parameters == TEXT("ArrayElementFailure"))
        {
            Env.bFailArray = Parameters == TEXT("ArrayFailure");
            Env.bFailElement = !Env.bFailArray;
            TestNull(TEXT("Failed array conversion returns null"), Call.NewStringArray({UnicodeValue}));
            ExpectedDeletes = Env.bFailArray ? 2 : 4;
        }
        else if (Parameters == TEXT("MissingMethod"))
        {
            Env.bFailMethod = true;
            TestNull(TEXT("Missing method returns null"), Call.FindMethod(reinterpret_cast<jclass>(Object), "missing", "()V"));
        }
        else if (Parameters == TEXT("JavaObjectException"))
        {
            Env.bFailCall = true;
            TestNull(TEXT("Throwing Java call returns null"), Call.CallObject(Object, Method, nullptr));
        }
        else if (Parameters == TEXT("JavaVoidException"))
        {
            Env.bFailCall = true;
            Call.CallVoid(Object, Method, nullptr);
        }
        else if (Parameters == TEXT("NullEnvironment"))
        {
            ExpectedDeletes = 0;
        }

        TestFalse(TEXT("The failed context is invalid"), Call.IsValid());
        const int32 Allocations = Env.StringsRequested;
        TestNull(TEXT("Later allocations are skipped"), Call.NewString(TEXT("skipped")));
        TestEqual(TEXT("No JNI allocation after failure"), Env.StringsRequested, Allocations);
        const int32 Calls = Env.Calls;
        Call.CallVoid(Object, Method, nullptr);
        TestEqual(TEXT("The Java target is not called after failure"), Env.Calls, Calls);
    }
    TestEqual(TEXT("All owned local references are released"), Env.ReferencesDeleted, ExpectedDeletes);
    TestEqual(TEXT("No forbidden JNI call while an exception is pending"), Env.IllegalCalls, 0);
    TestEqual(TEXT("Callback exception is preserved; native API exceptions are cleared"), Env.bPendingException, bPropagate);
    const int32 ExpectedClears = bPropagate || Parameters == TEXT("NullEnvironment") || Parameters == TEXT("NullWithoutException") ? 0 : 1;
    TestEqual(TEXT("Exception clearing follows the selected policy"), Env.ExceptionsCleared, ExpectedClears);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FTDAnalyticsJNIStringTest,
    "TDAnalytics.Android.JNI.StringBoundaries",
    EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FTDAnalyticsJNIStringTest::RunTest(const FString& Parameters)
{
    JNIEnv* Env = FAndroidApplication::GetJavaEnv();
    if (!TestNotNull(TEXT("JNI environment"), Env)) return false;
    FString EmbeddedNull(TEXT("A"));
    EmbeddedNull.AppendChar(0);
    EmbeddedNull.AppendChar(TEXT('B'));
    FString LongValue;
    for (int32 Index = 0; Index < 1024; ++Index) LongValue += UnicodeValue;
    const TArray<FString> Values = {FString(), UnicodeValue, EmbeddedNull, LongValue, TEXT("\U0001F469\u200D\U0001F4BB")};
    FJavaCallContext Call(Env, "JNIStringTest");
    TestTrue(TEXT("Null Java string maps to empty"), Call.ReadString(nullptr).IsEmpty());
    for (const FString& Value : Values)
    {
        jstring String = Call.NewString(Value);
        const FString Actual = Call.ReadString(String);
        TestTrue(TEXT("UTF-16 round trip preserves every code unit, including embedded null"),
            Actual.Len() == Value.Len() && FMemory::Memcmp(*Actual, *Value, Value.Len() * sizeof(TCHAR)) == 0);
        TestEqual(TEXT("Borrowed string remains readable"), Call.ReadString(String), Value);
    }
    jstring Returned;
    {
        FJavaCallContext Callback(Env, "JNICallbackReturn", EJavaExceptionPolicy::Propagate);
        Returned = Callback.Detach(Callback.NewString(UnicodeValue));
    }
    TestEqual(TEXT("Callback return survives native scope cleanup"), Call.ReadString(Returned), UnicodeValue);
    Env->DeleteLocalRef(Returned);
    TestTrue(TEXT("JNI context remains valid"), Call.IsValid());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FTDAnalyticsJNIArrayTest,
    "TDAnalytics.Android.JNI.StringArray",
    EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FTDAnalyticsJNIArrayTest::RunTest(const FString& Parameters)
{
    JNIEnv* Env = FAndroidApplication::GetJavaEnv();
    if (!TestNotNull(TEXT("JNI environment"), Env)) return false;
    FJavaCallContext Call(Env, "JNIArrayTest");
    TArray<FString> Values;
    for (int32 Index = 0; Index < 1024; ++Index) Values.Add(Index % 2 == 0 ? UnicodeValue : FString());
    jobjectArray Array = Call.NewStringArray(Values);
    if (!TestNotNull(TEXT("Unicode string array"), Array)) return false;
    for (int32 Index = 0; Index < Values.Num(); ++Index)
    {
        jstring String = static_cast<jstring>(Env->GetObjectArrayElement(Array, Index));
        TestEqual(TEXT("Array element preserves Unicode and empty strings"), Call.ReadString(String), Values[Index]);
        Env->DeleteLocalRef(String);
    }
    TestTrue(TEXT("Large array avoids local-reference exhaustion"), Call.IsValid());
    return true;
}

IMPLEMENT_COMPLEX_AUTOMATION_TEST(
    FTDAnalyticsJNICallbackTest,
    "TDAnalytics.Android.JNI.AutoTrackCallback",
    EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

void FTDAnalyticsJNICallbackTest::GetTests(TArray<FString>& Names, TArray<FString>& Commands) const
{
    Names = {TEXT("UE4"), TEXT("UE5")};
    Commands = Names;
}

bool FTDAnalyticsJNICallbackTest::RunTest(const FString& Parameters)
{
    const FString AppId = TEXT("tdanalytics-unicode-regression");
    UTDAnalytics::InitializeInstance(AppId, TEXT("http://127.0.0.1:1"), TAMode::NORMAL, false, TEXT("UTC"));
    UTDAnalytics::SetTrackStatus(TEXT("SAVE_ONLY"), AppId);
    int32 CallbackCount = 0;
    TAAutoTrackEventRetValDelegate Delegate;
    Delegate.BindLambda([this, &CallbackCount](FString EventType, FString Properties)
    {
        ++CallbackCount;
        TestEqual(TEXT("Callback receives Unicode event type"), EventType, UnicodeValue);
        TestEqual(TEXT("Callback receives Unicode properties"), Properties, UnicodeValue);
        return UnicodeValue;
    });
    thinkinganalytics::jni_ta_setAutoTrackEventListener(Delegate, {}, AppId);
    JNIEnv* Env = FAndroidApplication::GetJavaEnv();
    FJavaCallContext Call(Env, "JNICallbackTest");
    jstring Event = Call.NewString(UnicodeValue);
    jstring Properties = Call.NewString(UnicodeValue);
    jstring JavaAppId = Call.NewString(AppId);
    jstring Result = Parameters == TEXT("UE4")
        ? Java_com_epicgames_ue4_GameActivity_getAutoTrackEventProperties(Env, nullptr, Event, Properties, JavaAppId)
        : Java_com_epicgames_unreal_GameActivity_getAutoTrackEventProperties(Env, nullptr, Event, Properties, JavaAppId);
    TestEqual(TEXT("Callback result preserves Unicode"), Call.ReadString(Result), UnicodeValue);
    TestEqual(TEXT("Borrowed callback argument is still valid"), Call.ReadString(Event), UnicodeValue);
    TestEqual(TEXT("Registered callback ran once"), CallbackCount, 1);
    Env->DeleteLocalRef(Result);
    thinkinganalytics::jni_ta_setAutoTrackEventListener(TAAutoTrackEventRetValDelegate(), {}, AppId);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FTDAnalyticsJNIRecoveryTest,
    "TDAnalytics.Android.JNI.JavaExceptionRecovery",
    EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FTDAnalyticsJNIRecoveryTest::RunTest(const FString& Parameters)
{
    const FString AppId = TEXT("tdanalytics-unicode-regression");
    UTDAnalytics::InitializeInstance(AppId, TEXT("http://127.0.0.1:1"), TAMode::NORMAL, false, TEXT("UTC"));
    UTDAnalytics::SetTrackStatus(TEXT("SAVE_ONLY"), AppId);
    const FString OriginalId = UTDAnalytics::GetDistinctId(AppId);
    AddExpectedError(TEXT("TDAnalytics JNI taSetTrackStatus: Java exception at CallVoidMethod"), EAutomationExpectedErrorFlags::Contains, 1);
    UTDAnalytics::SetTrackStatus(TEXT("INVALID_REGRESSION_STATUS"), AppId);
    JNIEnv* Env = FAndroidApplication::GetJavaEnv();
    TestFalse(TEXT("Java void-call exception is cleared"), Env->ExceptionCheck() == JNI_TRUE);
    AddExpectedError(TEXT("TDAnalytics JNI taCreateLightInstance: Java exception at CallObjectMethod"), EAutomationExpectedErrorFlags::Contains, 1);
    TestTrue(TEXT("Throwing Java getter returns empty"), UTDAnalytics::CreateLightInstance(TEXT("tdanalytics-missing-regression")).IsEmpty());
    TestFalse(TEXT("Java object-call exception is cleared"), Env->ExceptionCheck() == JNI_TRUE);
    UTDAnalytics::SetDistinctId(UnicodeValue, AppId);
    TestEqual(TEXT("Subsequent SDK calls still work after Java exceptions"), UTDAnalytics::GetDistinctId(AppId), UnicodeValue);
    UTDAnalytics::SetDistinctId(OriginalId, AppId);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FTDAnalyticsJNIEntrypointsTest,
    "TDAnalytics.Android.JNI.SDKEntrypoints",
    EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FTDAnalyticsJNIEntrypointsTest::RunTest(const FString& Parameters)
{
    const FString AppId = TEXT("tdanalytics-unicode-regression");
    UTDAnalytics::InitializeInstance(AppId, TEXT("http://127.0.0.1:1"), TAMode::NORMAL, false, TEXT("UTC"));
    UTDAnalytics::SetTrackStatus(TEXT("SAVE_ONLY"), AppId);
    JNIEnv* Env = FAndroidApplication::GetJavaEnv();
    FJavaCallContext Call(Env, "JNIEntrypointsTest");
    jmethodID GetInstance = Env->GetStaticMethodID(FJavaWrapper::GameActivityClassID, "taGetCurrentInstance", "(Ljava/lang/String;)Lcn/thinkingdata/analytics/ThinkingAnalyticsSDK;");
    if (Env->ExceptionCheck())
    {
        Env->ExceptionClear();
        AddError(TEXT("Could not resolve the Java SDK instance accessor"));
        return false;
    }
    if (!TestNotNull(TEXT("Java SDK accessor"), GetInstance)) return false;
    jobject Instance = Env->CallStaticObjectMethod(FJavaWrapper::GameActivityClassID, GetInstance, Call.NewString(AppId));
    if (Env->ExceptionCheck())
    {
        Env->ExceptionClear();
        if (Instance) Env->DeleteLocalRef(Instance);
        AddError(TEXT("Could not get the Java SDK instance"));
        return false;
    }
    if (!TestNotNull(TEXT("Java SDK instance"), Instance)) return false;
    jclass SDKClass = Env->GetObjectClass(Instance);
    jmethodID GetLoginId = Call.FindMethod(SDKClass, "getLoginId", "()Ljava/lang/String;");
    const FString OriginalLogin = Call.ReadString(static_cast<jstring>(Call.CallObject(Instance, GetLoginId, nullptr)));
    UTDAnalytics::Login(UnicodeValue, AppId);
    TestEqual(TEXT("Login passes Chinese, emoji and trailing space to Java"),
        Call.ReadString(static_cast<jstring>(Call.CallObject(Instance, GetLoginId, nullptr))), UnicodeValue);
    if (OriginalLogin.IsEmpty()) UTDAnalytics::Logout(AppId);
    else UTDAnalytics::Login(OriginalLogin, AppId);

    const FString Properties = TEXT("{\"unicode\":\"1A_\U0001F60D\u3002\u6570\u6570 \"}");
    UTDAnalytics::TimeEvent(TEXT("jni_unicode_event"), AppId);
    UTDAnalytics::Track(TEXT("jni_unicode_event"), Properties, AppId);
    UTDAnalytics::TrackFirstWithId(TEXT("jni_unicode_first"), Properties, UnicodeValue, AppId);
    UTDAnalytics::TrackUpdate(TEXT("jni_unicode_update"), Properties, UnicodeValue, AppId);
    UTDAnalytics::TrackOverwrite(TEXT("jni_unicode_overwrite"), Properties, UnicodeValue, AppId);
    TestFalse(TEXT("Device ID getter returns a value"), UTDAnalytics::GetDeviceId(AppId).IsEmpty());
    TSharedPtr<FJsonObject> Preset;
    TestTrue(TEXT("Preset properties getter returns valid JSON"), FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(UTDAnalytics::GetPresetProperties(AppId)), Preset) && Preset.IsValid());
    TestEqual(TEXT("Current app ID is preserved"), thinkinganalytics::jni_ta_getCurrentAppId(AppId), AppId);
    const FString LightInstance = UTDAnalytics::CreateLightInstance(AppId);
    TestFalse(TEXT("Light instance getter returns a token"), LightInstance.IsEmpty());
    if (!LightInstance.IsEmpty()) TestFalse(TEXT("Light instance can be read"), UTDAnalytics::GetDistinctId(LightInstance).IsEmpty());
    Env->DeleteLocalRef(SDKClass);
    Env->DeleteLocalRef(Instance);
    TestTrue(TEXT("SDK entrypoint test finishes without a JNI error"), Call.IsValid());
    return true;
}

#endif // TDANALYTICS_ANDROID_TESTS && WITH_DEV_AUTOMATION_TESTS
