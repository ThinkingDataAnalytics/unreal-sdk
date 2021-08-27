// Copyright 2020 Thinking Data Ltd. All Rights Reserved.

#include "ThinkingAnalytics.h"
#include "Core.h"
#include "Json.h"
#include <string>
#include "ISettingsModule.h"
#include "AnalyticsEventAttribute.h"

#include "ThinkingAnalyticsSettings.h"
#include "Interfaces/IPluginManager.h"
#if PLATFORM_ANDROID
#include "./Android/ThinkingAnalyticsJNI.h"
#elif PLATFORM_IOS
#include "./IOS/ThinkingAnalyticsCpp.h"
#endif

std::map<FString,GetDynamicSuperProperties> UThinkingAnalytics::dynamicPropertiesMap = {};

DEFINE_LOG_CATEGORY_STATIC(ThinkingAnalytics, Display, All);

UThinkingAnalytics::UThinkingAnalytics(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UThinkingAnalytics::Initialize() {
    auto  &manager = IPluginManager::Get();
    auto  plugin = manager.FindPlugin("ThinkingAnalytics");

    if ( !plugin.IsValid() )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("ThinkingAnalytics is not correctly loaded"));
        return;
    }

    const UThinkingAnalyticsSettings *defaultSettings = GetDefault<UThinkingAnalyticsSettings>();
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_initialize(defaultSettings, plugin->GetDescriptor().VersionName);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_initialize(defaultSettings, plugin->GetDescriptor().VersionName);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Calling Initialize... The platform is not supported. %d"), defaultSettings->Mode);
#endif
}

void UThinkingAnalytics::InitializeInstance(const FString& appid, const FString& serverurl, TAMode mode, bool bEnableLog, const FString& timeZone) {

    auto  &manager = IPluginManager::Get();
    auto  plugin = manager.FindPlugin("ThinkingAnalytics");

    if ( !plugin.IsValid() )
    {
        UE_LOG(ThinkingAnalytics, Warning, TEXT("ThinkingAnalytics is not correctly loaded"));
        return;
    }
    
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_initializeInstance(appid, serverurl, mode, bEnableLog, timeZone, plugin->GetDescriptor().VersionName);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_initialize(appid, serverurl, mode, bEnableLog, timeZone, plugin->GetDescriptor().VersionName);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Calling Initialize... The platform is not supported. %d"), mode);
#endif
}

void UThinkingAnalytics::EnableAutoTrack(const FString& AppId) {

#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_enable_autotrack(AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_enable_autotrack(AppId);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Calling UThinkingAnalytics::EnableAutoTrack... The platform is not supported."));
#endif
}

void UThinkingAnalytics::Track(const FString& EventName, const FString& Properties, const FString& AppId)
{
#if PLATFORM_ANDROID
    FString appid = thinkinganalytics::jni_ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    thinkinganalytics::jni_ta_track(EventName, Properties, dyldproperties, AppId);
#elif PLATFORM_IOS
    FString appid = ThinkingAnalyticsCpp::ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    ThinkingAnalyticsCpp::ta_track(EventName, Properties, dyldproperties, AppId);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::Track"));
#endif
}

void UThinkingAnalytics::TrackFirst(const FString& EventName, const FString& Properties, const FString& AppId)
{
#if PLATFORM_ANDROID
    FString appid = thinkinganalytics::jni_ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    thinkinganalytics::jni_ta_track_first(EventName, Properties, dyldproperties, AppId);
#elif PLATFORM_IOS
    FString appid = ThinkingAnalyticsCpp::ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    ThinkingAnalyticsCpp::ta_track_first(EventName, Properties, dyldproperties, AppId);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::TrackUnique"));
#endif
}

void UThinkingAnalytics::TrackFirstWithId(const FString& EventName, const FString& Properties, const FString& FirstCheckId, const FString& AppId)
{
#if PLATFORM_ANDROID
    FString appid = thinkinganalytics::jni_ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    thinkinganalytics::jni_ta_track_first_withId(EventName, Properties, FirstCheckId, dyldproperties, AppId);
#elif PLATFORM_IOS
    FString appid = ThinkingAnalyticsCpp::ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    ThinkingAnalyticsCpp::ta_track_first_withId(EventName, Properties, FirstCheckId, dyldproperties, AppId);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::TrackUniqueWithId"));
#endif
}

void UThinkingAnalytics::TrackUpdate(const FString& EventName, const FString& Properties, const FString& EventId, const FString& AppId)
{
#if PLATFORM_ANDROID
    FString appid = thinkinganalytics::jni_ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    thinkinganalytics::jni_ta_track_update(EventName, Properties, EventId, dyldproperties, AppId);
#elif PLATFORM_IOS
    FString appid = ThinkingAnalyticsCpp::ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    ThinkingAnalyticsCpp::ta_track_update(EventName, Properties, EventId, dyldproperties, AppId);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::TrackUpdate"));
#endif
}

void UThinkingAnalytics::TrackOverwrite(const FString& EventName, const FString& Properties, const FString& EventId, const FString& AppId)
{
#if PLATFORM_ANDROID
    FString appid = thinkinganalytics::jni_ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    thinkinganalytics::jni_ta_track_overwrite(EventName, Properties, EventId, dyldproperties, AppId);
#elif PLATFORM_IOS
    FString appid = ThinkingAnalyticsCpp::ta_getCurrentAppId(AppId);
    FString dyldproperties = UThinkingAnalytics::GetDynamicProperties(appid);
    ThinkingAnalyticsCpp::ta_track_overwrite(EventName, Properties, EventId, dyldproperties, AppId);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::TrackOverwrite"));
#endif
}

void UThinkingAnalytics::TimeEvent(const FString& EventName, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_time_event(EventName, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_time_event(EventName, AppId);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::TimeEvent"));
#endif
}

void UThinkingAnalytics::UserSet(const FString& Properties, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_user_set(Properties, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_user_set(Properties, AppId);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::UserSet"));
#endif
}

void UThinkingAnalytics::UserSetOnce(const FString& Properties, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_user_set_once(Properties, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_user_set_once(Properties, AppId);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::UserSetOnce"));
#endif
}

void UThinkingAnalytics::UserAdd(const FString& Property, const float Value, const FString& AppId)
{
    FString outStr;
    TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&outStr);
    /** Write JSON message */
    JsonWriter->WriteObjectStart();
    JsonWriter->WriteValue(Property, Value);
    JsonWriter->WriteObjectEnd();
    JsonWriter->Close();
    
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_user_add(outStr, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_user_add(outStr, AppId);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::UserAdd"));
#endif
}

void UThinkingAnalytics::UserUnset(const FString& Property, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_user_unset(Property, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_user_unset(Property, AppId);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::UserUnset"));
#endif
}

void UThinkingAnalytics::UserAppend(const FString& Properties, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_user_append(Properties, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_user_append(Properties, AppId);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::UserAppend"));
#endif
}

void UThinkingAnalytics::UserDelete(const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_user_delete(AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_user_delete(AppId);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::UserDelete"));
#endif
}

void UThinkingAnalytics::Identify(const FString& DistinctId, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_identify(DistinctId, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_identify(DistinctId, AppId);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::Identify"));
#endif
}

void UThinkingAnalytics::Login(const FString& AccountId, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_login(AccountId, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_login(AccountId, AppId);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::Login"));
#endif
}

void UThinkingAnalytics::Logout(const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_logout(AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_logout(AppId);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::Logout"));
#endif
}

void UThinkingAnalytics::Flush(const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_flush(AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_flush(AppId);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::Flush"));
#endif
}

void UThinkingAnalytics::CalibrateTime(int64 timestamp)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_calibrate_time(timestamp);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_calibrate_time(timestamp);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::CalibrateTime"));
#endif
}

void UThinkingAnalytics::CalibrateTimeWithNtp(const FString& urlString)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_calibrate_time_ntp(urlString);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_calibrate_time_ntp(urlString);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::calibrateTimeWithNtp"));
#endif
}

void UThinkingAnalytics::OptOutTracking(const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_opt_out_tracking(AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_opt_out_tracking(AppId);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::OptOutTracking"));
#endif
}

void UThinkingAnalytics::OptInTracking(const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_opt_in_tracking(AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_opt_in_tracking(AppId);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::OptInTracking"));
#endif
}

void UThinkingAnalytics::EnableTracking(bool bIsEnable, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_enable_tracking(bIsEnable, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_enable_tracking(bIsEnable, AppId);
#else
    UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::EnableTracking"));
#endif
}

FString UThinkingAnalytics::GetDeviceId(const FString& AppId)
{
#if PLATFORM_ANDROID
    return thinkinganalytics::jni_ta_get_device_id(AppId);
#elif PLATFORM_IOS
    return ThinkingAnalyticsCpp::ta_get_device_id(AppId);
#else
UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::GetDeviceId"));
    return FString("");
 #endif
    
}

FString UThinkingAnalytics::GetDistinctId(const FString& AppId)
{
#if PLATFORM_ANDROID
    return thinkinganalytics::jni_ta_get_distinct_id(AppId);
#elif PLATFORM_IOS
    return ThinkingAnalyticsCpp::ta_get_distinct_id(AppId);
#else
UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::GetDistinctId"));
    return FString("");
 #endif
}

void UThinkingAnalytics::SetSuperProperties(const FString& properties, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_set_superProperties(properties, AppId);
#elif PLATFORM_IOS
    ThinkingAnalyticsCpp::ta_set_superProperties(properties, AppId);
#else
UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::SetSuperProperties"));

 #endif
}

FString UThinkingAnalytics::GetSuperProperties(const FString& AppId)
{
#if PLATFORM_ANDROID
    return thinkinganalytics::jni_ta_get_superProperties(AppId);
#elif PLATFORM_IOS
    return ThinkingAnalyticsCpp::ta_get_superProperties(AppId);
#else
UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::GetSuperProperties"));
    return FString("");
 #endif
}

FString UThinkingAnalytics::GetPresetProperties(const FString& AppId)
{
#if PLATFORM_ANDROID
    return thinkinganalytics::jni_ta_get_presetProperties(AppId);
#elif PLATFORM_IOS
    return ThinkingAnalyticsCpp::ta_get_presetProperties(AppId);
#else
UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::GetPresetProperties"));
    return FString("");
 #endif
}

FString UThinkingAnalytics::CreateLightInstance(const FString& AppId)
{
#if PLATFORM_ANDROID
    return thinkinganalytics::jni_ta_createLightInstance(AppId);
#elif PLATFORM_IOS
    return ThinkingAnalyticsCpp::ta_createLightInstance(AppId);
#else
UE_LOG(ThinkingAnalytics, Warning, TEXT("Unsupported Platform. Calling UThinkingAnalytics::CreateLightInstance"));
    return FString("");
 #endif
}


FString UThinkingAnalytics::GetDynamicProperties(const FString& AppId)
{
    FString jsonString = "";
    if(UThinkingAnalytics::dynamicPropertiesMap.size() > 0 && UThinkingAnalytics::dynamicPropertiesMap.count(AppId) > 0)
    {
        GetDynamicSuperProperties getDynamicSuperProperties = UThinkingAnalytics::dynamicPropertiesMap[AppId];
        jsonString = getDynamicSuperProperties();
    }
    return jsonString;
}
