// Copyright 2020 Thinking Data Ltd. All Rights Reserved.

#include "TDAnalyticsJNI.h"
#include "TDAnalyticsJNIUtils.h"
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"

namespace thinkinganalytics
{
    namespace
    {
        template <typename... TArgs>
        void CallJavaVoid(const char* Name, const char* Signature, const TArgs&... Args)
        {
            FJavaCallContext Call(FAndroidApplication::GetJavaEnv(), Name);
            jmethodID Method = Call.FindMethod(FJavaWrapper::GameActivityClassID, Name, Signature);
            jvalue Arguments[sizeof...(Args) + 1] = {Call.Argument(Args)...};
            Call.CallVoid(FJavaWrapper::GameActivityThis, Method, Arguments);
        }

        template <typename... TArgs>
        FString CallJavaString(const char* Name, const char* Signature, const TArgs&... Args)
        {
            FJavaCallContext Call(FAndroidApplication::GetJavaEnv(), Name);
            jmethodID Method = Call.FindMethod(FJavaWrapper::GameActivityClassID, Name, Signature);
            jvalue Arguments[sizeof...(Args) + 1] = {Call.Argument(Args)...};
            jstring Result = static_cast<jstring>(Call.CallObject(FJavaWrapper::GameActivityThis, Method, Arguments));
            return Call.ReadString(Result);
        }

        jstring GetAutoTrackEventProperties(JNIEnv* Env, jstring EventType, jstring Properties, jstring AppId)
        {
            // These exceptions belong to the Java caller. Only legal cleanup calls may follow a failure.
            FJavaCallContext Call(Env, "getAutoTrackEventProperties", EJavaExceptionPolicy::Propagate);
            const FString NativeAppId = Call.ReadString(AppId);
            const FString NativeEventType = Call.ReadString(EventType);
            const FString NativeProperties = Call.ReadString(Properties);
            if (!Call.IsValid()) return nullptr;

            FString JsonString;
            TAAutoTrackEventRetValDelegate* Delegate = TAAutoTrackEventAndroidMethods.Find(NativeAppId);
            if (Delegate && Delegate->IsBound())
            {
                JsonString = Delegate->Execute(NativeEventType, NativeProperties);
            }
            return Call.Detach(Call.NewString(JsonString));
        }
    }

    extern "C"
    {
        FString jni_ta_getCurrentAppId(FString appId)
        {
            return CallJavaString("taGetCurrentAppId", "(Ljava/lang/String;)Ljava/lang/String;", appId);
        }

        void jni_ta_calibrate_time(long timestamp)
        {
            CallJavaVoid("taCalibrateTime", "(J)V", static_cast<jlong>(timestamp));
        }

        void jni_ta_enable_log()
        {
            CallJavaVoid("taEnableLog", "()V");
        }

        void jni_ta_initialize(const UTDAnalyticsSettings *defaultSettings, FString version)
        {
            if (defaultSettings->bEnableLog) jni_ta_enable_log();
            if (defaultSettings->bEnableEncrypt)
            {
                CallJavaVoid("taInitialize", "(Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;)V",
                    defaultSettings->AppID, defaultSettings->ServerUrl, static_cast<jint>(defaultSettings->Mode), defaultSettings->TimeZone,
                    version, defaultSettings->EncryptPublicKey, static_cast<jint>(defaultSettings->EncryptVersion), defaultSettings->SymmetricEncryption, defaultSettings->AsymmetricEncryption);
            }
            else
            {
                CallJavaVoid("taInitialize", "(Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;)V",
                    defaultSettings->AppID, defaultSettings->ServerUrl, static_cast<jint>(defaultSettings->Mode), defaultSettings->TimeZone, version);
            }
        }

        void jni_ta_initializeInstance(FString appid, FString serverurl, TAMode mode, bool bEnableLog, FString timeZone, FString version)
        {
            if (bEnableLog) jni_ta_enable_log();
            CallJavaVoid("taInitialize", "(Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;)V",
                appid, serverurl, static_cast<jint>(mode), timeZone, version);
        }

        void jni_ta_initializeEncryptInstance(FString appid, FString serverurl, TAMode mode, bool bEnableLog, FString timeZone, FString version, bool bEnableEncrypt, FString EncryptPublicKey, int EncryptVersion, FString SymmetricEncryption, FString AsymmetricEncryption)
        {
            if (bEnableEncrypt)
            {
                CallJavaVoid("taInitialize", "(Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;)V",
                    appid, serverurl, static_cast<jint>(mode), timeZone, version, EncryptPublicKey, static_cast<jint>(EncryptVersion), SymmetricEncryption, AsymmetricEncryption);
            }
            else
            {
                jni_ta_initializeInstance(appid, serverurl, mode, bEnableLog, timeZone, version);
            }
        }

        void jni_ta_track(FString eventName, FString properties, FString dyldproperties, FString appid)
        {
            CallJavaVoid("taTrack", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", eventName, properties, dyldproperties, appid);
        }

        void jni_ta_track_first(FString eventName, FString properties, FString dyldproperties, FString appid)
        {
            CallJavaVoid("taTrackFirst", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", eventName, properties, FString(), dyldproperties, appid);
        }

        void jni_ta_track_first_withId(FString eventName, FString properties, FString firstCheckId,  FString dyldproperties, FString appid)
        {
            CallJavaVoid("taTrackFirst", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", eventName, properties, firstCheckId, dyldproperties, appid);
        }

        void jni_ta_track_update(FString eventName, FString properties, FString eventId,  FString dyldproperties, FString appid)
        {
            CallJavaVoid("taTrackUpdate", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", eventName, properties, eventId, dyldproperties, appid);
        }

        void jni_ta_track_overwrite(FString eventName, FString properties, FString eventId, FString dyldproperties, FString appid)
        {
            CallJavaVoid("taTrackOverwrite", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", eventName, properties, eventId, dyldproperties, appid);
        }

        void jni_ta_time_event(FString eventName, FString appid)
        {
            CallJavaVoid("taTimeEvent", "(Ljava/lang/String;Ljava/lang/String;)V", eventName, appid);
        }

        void jni_ta_identify(FString distinctId, FString appid)
        {
            CallJavaVoid("taIdentify", "(Ljava/lang/String;Ljava/lang/String;)V", distinctId, appid);
        }

        void jni_ta_login(FString accountId, FString appid)
        {
            CallJavaVoid("taLogin", "(Ljava/lang/String;Ljava/lang/String;)V", accountId, appid);
        }

        void jni_ta_logout(FString appid)
        {
            CallJavaVoid("taLogout", "(Ljava/lang/String;)V", appid);
        }

        void jni_ta_flush(FString appid)
        {
            CallJavaVoid("taFlush", "(Ljava/lang/String;)V", appid);
        }

        void jni_ta_user_set(FString properties, FString appid)
        {
            CallJavaVoid("taUserOperations", "(ILjava/lang/String;Ljava/lang/String;)V", 0, properties, appid);
        }

        void jni_ta_user_set_once(FString properties, FString appid)
        {
            CallJavaVoid("taUserOperations", "(ILjava/lang/String;Ljava/lang/String;)V", 1, properties, appid);
        }

        void jni_ta_user_append(FString properties, FString appid)
        {
            CallJavaVoid("taUserOperations", "(ILjava/lang/String;Ljava/lang/String;)V", 2, properties, appid);
        }

        void jni_ta_user_unique_append(FString properties, FString appid)
        {
            CallJavaVoid("taUserOperations", "(ILjava/lang/String;Ljava/lang/String;)V", 5, properties, appid);
        }

        void jni_ta_user_unset(FString property, FString appid)
        {
            CallJavaVoid("taUserOperations", "(ILjava/lang/String;Ljava/lang/String;)V", 3, property, appid);
        }

        void jni_ta_user_add(FString properties, FString appid)
        {
            CallJavaVoid("taUserOperations", "(ILjava/lang/String;Ljava/lang/String;)V", 4, properties, appid);
        }

        void jni_ta_user_delete(FString appid)
        {
            CallJavaVoid("taUserDelete", "(Ljava/lang/String;)V", appid);
        }

        void jni_ta_enable_autotrack(FString appid)
        {
            CallJavaVoid("taEnableAutoTrack", "(Ljava/lang/String;)V", appid);
        }

        void jni_ta_enable_autotrack_with_type(FString appid, TArray<FString> eventTypeList)
        {
            CallJavaVoid("taEnableAutoTrack", "(Ljava/lang/String;[Ljava/lang/String;)V", appid, eventTypeList);
        }

        void jni_ta_enable_autotrack_with_type_and_prop(FString appid, TArray<FString> eventTypeList, FString properties)
        {
            CallJavaVoid("taEnableAutoTrack", "(Ljava/lang/String;[Ljava/lang/String;Ljava/lang/String;)V", appid, eventTypeList, properties);
        }

        void jni_ta_opt_out_tracking(FString appid)
        {
            CallJavaVoid("taOptOutTracking", "(Ljava/lang/String;)V", appid);
        }

        void jni_ta_opt_in_tracking(FString appid)
        {
            CallJavaVoid("taOptInTracking", "(Ljava/lang/String;)V", appid);
        }

        void jni_ta_enable_tracking(bool enabled, FString appid)
        {
            CallJavaVoid("taEnableTracking", "(ZLjava/lang/String;)V", enabled, appid);
        }

        FString jni_ta_get_distinct_id(FString appid)
        {
            return CallJavaString("taGetDistinctId", "(Ljava/lang/String;)Ljava/lang/String;", appid);
        }

        FString jni_ta_get_device_id(FString appid)
        {
            return CallJavaString("taGetDeviceId", "(Ljava/lang/String;)Ljava/lang/String;", appid);
        }

        void jni_ta_calibrate_time_ntp(FString urlstring)
        {
            CallJavaVoid("taCalibrateTimeWithNtp", "(Ljava/lang/String;)V", urlstring);
        }

        void jni_ta_set_superProperties(FString properties, FString appid)
        {
            CallJavaVoid("taSetSuperProperties", "(Ljava/lang/String;Ljava/lang/String;)V", properties, appid);
        }

        void jni_ta_clear_superProperties(FString appid)
        {
            CallJavaVoid("taClearSuperProperties", "(Ljava/lang/String;)V", appid);
        }

        void jni_ta_unset_superProperty(FString property, FString appid)
        {
            CallJavaVoid("taUnsetSuperProperty", "(Ljava/lang/String;Ljava/lang/String;)V", property, appid);
        }

        FString jni_ta_get_superProperties(FString appid)
        {
            return CallJavaString("taGetSuperProperties", "(Ljava/lang/String;)Ljava/lang/String;", appid);
        }

        FString jni_ta_get_presetProperties(FString appid)
        {
            return CallJavaString("taGetPresetProperties", "(Ljava/lang/String;)Ljava/lang/String;", appid);
        }

        FString jni_ta_createLightInstance(FString appid)
        {
            return CallJavaString("taCreateLightInstance", "(Ljava/lang/String;)Ljava/lang/String;", appid);
        }

        void jni_ta_set_trackStatus(FString status, FString appid)
        {
            CallJavaVoid("taSetTrackStatus", "(Ljava/lang/String;Ljava/lang/String;)V", status, appid);
        }

        void jni_ta_setAutoTrackEventListener(TAAutoTrackEventRetValDelegate Del, TArray<FString> eventTypeList, const FString& AppId)
        {
            TAAutoTrackEventAndroidMethods.Emplace(AppId, Del);
            CallJavaVoid("taEnableAutoTrackWithListener", "(Ljava/lang/String;[Ljava/lang/String;)V", AppId, eventTypeList);
        }

        JNI_METHOD jstring Java_com_epicgames_ue4_GameActivity_getAutoTrackEventProperties(JNIEnv* jenv, jobject thiz, jstring eventType, jstring propertiesStr, jstring AppId)
        {
            return GetAutoTrackEventProperties(jenv, eventType, propertiesStr, AppId);
        }

        JNI_METHOD jstring Java_com_epicgames_unreal_GameActivity_getAutoTrackEventProperties(JNIEnv* jenv, jobject thiz, jstring eventType, jstring propertiesStr, jstring AppId)
        {
            return GetAutoTrackEventProperties(jenv, eventType, propertiesStr, AppId);
        }

        void jni_ta_enableThirdPartySharing(int types, FString appid)
        {
            CallJavaVoid("taEnableThirdPartySharing", "(ILjava/lang/String;)V", (jint)types, appid);
        }

        void jni_ta_enableThirdPartySharingWithCustomProperties(int typeList, FString properties, FString appId)
        {
            CallJavaVoid("taEnableThirdPartySharing", "(ILjava/lang/String;Ljava/lang/String;)V", (jint)typeList, properties, appId);
        }

    }
}
