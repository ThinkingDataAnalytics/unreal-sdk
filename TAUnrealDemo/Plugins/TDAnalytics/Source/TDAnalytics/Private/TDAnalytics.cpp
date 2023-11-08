// Copyright 2020 Thinking Data Ltd. All Rights Reserved.

#include "TDAnalytics.h"
#include "Core.h"
#include "Json.h"
#include <string>
#include "ISettingsModule.h"
#include "AnalyticsEventAttribute.h"

#include "TDAnalyticsSettings.h"
#include "Interfaces/IPluginManager.h"
#include "Common/TAConstants.h"

#if PLATFORM_ANDROID
#include "./Android/TDAnalyticsJNI.h"
#elif PLATFORM_IOS
#include "./IOS/TDAnalyticsCpp.h"
#elif PLATFORM_MAC || PLATFORM_WINDOWS
#include "./PC/TDAnalyticsPC.h"
#endif

DEFINE_LOG_CATEGORY_STATIC(TDAnalytics, Display, All);

UTDAnalytics::UTDAnalytics(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UTDAnalytics::Initialize() 
{
    auto  &manager = IPluginManager::Get();
    auto  plugin = manager.FindPlugin("TDAnalytics");

    if ( !plugin.IsValid() )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("TDAnalytics is not correctly loaded"));
        return;
    }

    const UTDAnalyticsSettings *defaultSettings = GetDefault<UTDAnalyticsSettings>();
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_initialize(defaultSettings, plugin->GetDescriptor().VersionName);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_initialize(defaultSettings, plugin->GetDescriptor().VersionName);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC::Initialize(defaultSettings, plugin->GetDescriptor().VersionName);
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Calling Initialize... The platform is not supported. %d"), defaultSettings->Mode);
#endif
}

void UTDAnalytics::InitializeInstance(const FString& appid, const FString& serverurl, TAMode mode, bool bEnableLog, const FString& timeZone) 
{

    auto  &manager = IPluginManager::Get();
    auto  plugin = manager.FindPlugin("TDAnalytics");

    if ( !plugin.IsValid() )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("TDAnalytics is not correctly loaded"));
        return;
    }
    
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_initializeInstance(appid, serverurl, mode, bEnableLog, timeZone, plugin->GetDescriptor().VersionName);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_initialize(appid, serverurl, mode, bEnableLog, timeZone, plugin->GetDescriptor().VersionName);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC::Initialize(appid, serverurl, mode, bEnableLog, timeZone, plugin->GetDescriptor().VersionName);
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Calling Initialize... The platform is not supported. %d"), mode);
#endif
}

void UTDAnalytics::InitializeEncryptInstance(const FString& appid, const FString& serverurl, TAMode mode, bool bEnableLog, const FString& timeZone, bool bEnableEncrypt, const FString& EncryptPublicKey, int EncryptVersion, const FString& SymmetricEncryption, const FString& AsymmetricEncryption)
{
    auto  &manager = IPluginManager::Get();
    auto  plugin = manager.FindPlugin("TDAnalytics");

    if ( !plugin.IsValid() )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("TDAnalytics is not correctly loaded"));
        return;
    }
    
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_initializeEncryptInstance(appid, serverurl, mode, bEnableLog, timeZone, plugin->GetDescriptor().VersionName, bEnableEncrypt, EncryptPublicKey, EncryptVersion, SymmetricEncryption, AsymmetricEncryption);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_initializeEncryptInstance(appid, serverurl, mode, bEnableLog, timeZone, plugin->GetDescriptor().VersionName, EncryptPublicKey, EncryptVersion, SymmetricEncryption, AsymmetricEncryption);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC::Initialize(appid, serverurl, mode, bEnableLog, timeZone, plugin->GetDescriptor().VersionName);
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Calling Initialize... The platform is not supported. %d"), mode);
#endif
}

void UTDAnalytics::EnableAutoTrack(const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_enable_autotrack(AppId);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_enable_autotrack(AppId);
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Calling UTDAnalytics::EnableAutoTrack... The platform is not supported."));
#endif
}

void UTDAnalytics::EnableAutoTrackWithType(const TArray<FString>& EventTypeList, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_enable_autotrack_with_type(AppId, EventTypeList);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_enable_autotrack_with_type(AppId, EventTypeList);
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Calling UTDAnalytics::EnableAutoTrack... The platform is not supported."));
#endif
}

void UTDAnalytics::EnableAutoTrackWithTypeAndProperties(const TArray<FString>& EventTypeList, const FString& Properties, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_enable_autotrack_with_type_and_prop(AppId, EventTypeList, Properties);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_enable_autotrack_with_type_and_prop(AppId, EventTypeList, Properties);
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Calling UTDAnalytics::EnableAutoTrack... The platform is not supported."));
#endif
}

void UTDAnalytics::EnableAutoTrackWithTypeAndProperties(const TArray<FString>& EventTypeList, TSharedPtr<FJsonObject> Properties, const FString& AppId)
{
    FString PropertiesStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PropertiesStr);
    FJsonSerializer::Serialize(Properties.ToSharedRef(), Writer);

#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_enable_autotrack_with_type_and_prop(AppId, EventTypeList, PropertiesStr);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_enable_autotrack_with_type_and_prop(AppId, EventTypeList, PropertiesStr);
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Calling UTDAnalytics::EnableAutoTrack... The platform is not supported."));
#endif
}

void UTDAnalytics::Track(const FString& EventName, const FString& Properties, const FString& AppId)
{
#if PLATFORM_ANDROID
    FString appid = thinkinganalytics::jni_ta_getCurrentAppId(AppId);
    FString dyldproperties = UTDAnalytics::GetDynamicProperties(appid);
    thinkinganalytics::jni_ta_track(EventName, Properties, dyldproperties, AppId);
#elif PLATFORM_IOS
    FString appid = TDAnalyticsCpp::ta_getCurrentAppId(AppId);
    FString dyldproperties = UTDAnalytics::GetDynamicProperties(appid);
    TDAnalyticsCpp::ta_track(EventName, Properties, dyldproperties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        FString Dyldproperties = UTDAnalytics::GetDynamicProperties(Instance->InstanceAppID);
        Instance->Track(EventName, Properties, Dyldproperties); 
    }
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::Track"));
#endif
}

void UTDAnalytics::Track(const FString& EventName, TSharedPtr<FJsonObject> Properties, const FString& AppId)
{
    FString PropertiesStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PropertiesStr);
    FJsonSerializer::Serialize(Properties.ToSharedRef(), Writer);

#if PLATFORM_ANDROID
    FString appid = thinkinganalytics::jni_ta_getCurrentAppId(AppId);
    FString dyldproperties = UTDAnalytics::GetDynamicProperties(appid);
    thinkinganalytics::jni_ta_track(EventName, PropertiesStr, dyldproperties, AppId);
#elif PLATFORM_IOS
    FString appid = TDAnalyticsCpp::ta_getCurrentAppId(AppId);
    FString dyldproperties = UTDAnalytics::GetDynamicProperties(appid);
    TDAnalyticsCpp::ta_track(EventName, PropertiesStr, dyldproperties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        FString Dyldproperties = UTDAnalytics::GetDynamicProperties(Instance->InstanceAppID);
        Instance->Track(EventName, PropertiesStr, Dyldproperties); 
    }
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::Track"));
#endif
}

void UTDAnalytics::TrackFirst(const FString& EventName, const FString& Properties, const FString& AppId)
{
#if PLATFORM_ANDROID
    FString appid = thinkinganalytics::jni_ta_getCurrentAppId(AppId);
    FString dyldproperties = UTDAnalytics::GetDynamicProperties(appid);
    thinkinganalytics::jni_ta_track_first(EventName, Properties, dyldproperties, AppId);
#elif PLATFORM_IOS
    FString appid = TDAnalyticsCpp::ta_getCurrentAppId(AppId);
    FString dyldproperties = UTDAnalytics::GetDynamicProperties(appid);
    TDAnalyticsCpp::ta_track_first(EventName, Properties, dyldproperties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        FString Dyldproperties = UTDAnalytics::GetDynamicProperties(Instance->InstanceAppID);
        Instance->TrackFirst(EventName, Properties, Dyldproperties); 
    }
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::TrackUnique"));
#endif
}

void UTDAnalytics::TrackFirst(const FString& EventName, TSharedPtr<FJsonObject> Properties, const FString& AppId)
{
    FString PropertiesStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PropertiesStr);
    FJsonSerializer::Serialize(Properties.ToSharedRef(), Writer);

#if PLATFORM_ANDROID
    FString appid = thinkinganalytics::jni_ta_getCurrentAppId(AppId);
    FString dyldproperties = UTDAnalytics::GetDynamicProperties(appid);
    thinkinganalytics::jni_ta_track_first(EventName, PropertiesStr, dyldproperties, AppId);
#elif PLATFORM_IOS
    FString appid = TDAnalyticsCpp::ta_getCurrentAppId(AppId);
    FString dyldproperties = UTDAnalytics::GetDynamicProperties(appid);
    TDAnalyticsCpp::ta_track_first(EventName, PropertiesStr, dyldproperties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        FString Dyldproperties = UTDAnalytics::GetDynamicProperties(Instance->InstanceAppID);
        Instance->TrackFirst(EventName, PropertiesStr, Dyldproperties); 
    }
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::TrackUnique"));
#endif
}

void UTDAnalytics::TrackFirstWithId(const FString& EventName, const FString& Properties, const FString& FirstCheckId, const FString& AppId)
{
#if PLATFORM_ANDROID
    FString appid = thinkinganalytics::jni_ta_getCurrentAppId(AppId);
    FString dyldproperties = UTDAnalytics::GetDynamicProperties(appid);
    thinkinganalytics::jni_ta_track_first_withId(EventName, Properties, FirstCheckId, dyldproperties, AppId);
#elif PLATFORM_IOS
    FString appid = TDAnalyticsCpp::ta_getCurrentAppId(AppId);
    FString dyldproperties = UTDAnalytics::GetDynamicProperties(appid);
    TDAnalyticsCpp::ta_track_first_withId(EventName, Properties, FirstCheckId, dyldproperties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        FString Dyldproperties = UTDAnalytics::GetDynamicProperties(Instance->InstanceAppID);
        Instance->TrackFirstWithId(EventName, Properties, FirstCheckId, Dyldproperties); 
    }
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::TrackUniqueWithId"));
#endif
}

void UTDAnalytics::TrackFirstWithId(const FString& EventName, TSharedPtr<FJsonObject> Properties, const FString& FirstCheckId, const FString& AppId)
{
    FString PropertiesStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PropertiesStr);
    FJsonSerializer::Serialize(Properties.ToSharedRef(), Writer);

#if PLATFORM_ANDROID
    FString appid = thinkinganalytics::jni_ta_getCurrentAppId(AppId);
    FString dyldproperties = UTDAnalytics::GetDynamicProperties(appid);
    thinkinganalytics::jni_ta_track_first_withId(EventName, PropertiesStr, FirstCheckId, dyldproperties, AppId);
#elif PLATFORM_IOS
    FString appid = TDAnalyticsCpp::ta_getCurrentAppId(AppId);
    FString dyldproperties = UTDAnalytics::GetDynamicProperties(appid);
    TDAnalyticsCpp::ta_track_first_withId(EventName, PropertiesStr, FirstCheckId, dyldproperties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        FString Dyldproperties = UTDAnalytics::GetDynamicProperties(Instance->InstanceAppID);
        Instance->TrackFirstWithId(EventName, PropertiesStr, FirstCheckId, Dyldproperties); 
    }
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::TrackUniqueWithId"));
#endif
}

void UTDAnalytics::TrackUpdate(const FString& EventName, const FString& Properties, const FString& EventId, const FString& AppId)
{
#if PLATFORM_ANDROID
    FString appid = thinkinganalytics::jni_ta_getCurrentAppId(AppId);
    FString dyldproperties = UTDAnalytics::GetDynamicProperties(appid);
    thinkinganalytics::jni_ta_track_update(EventName, Properties, EventId, dyldproperties, AppId);
#elif PLATFORM_IOS
    FString appid = TDAnalyticsCpp::ta_getCurrentAppId(AppId);
    FString dyldproperties = UTDAnalytics::GetDynamicProperties(appid);
    TDAnalyticsCpp::ta_track_update(EventName, Properties, EventId, dyldproperties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        FString Dyldproperties = UTDAnalytics::GetDynamicProperties(Instance->InstanceAppID);
        Instance->TrackUpdate(EventName, Properties, EventId, Dyldproperties); 
    }
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::TrackUpdate"));
#endif
}

void UTDAnalytics::TrackUpdate(const FString& EventName, TSharedPtr<FJsonObject> Properties, const FString& EventId, const FString& AppId)
{
    FString PropertiesStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PropertiesStr);
    FJsonSerializer::Serialize(Properties.ToSharedRef(), Writer);

#if PLATFORM_ANDROID
    FString appid = thinkinganalytics::jni_ta_getCurrentAppId(AppId);
    FString dyldproperties = UTDAnalytics::GetDynamicProperties(appid);
    thinkinganalytics::jni_ta_track_update(EventName, PropertiesStr, EventId, dyldproperties, AppId);
#elif PLATFORM_IOS
    FString appid = TDAnalyticsCpp::ta_getCurrentAppId(AppId);
    FString dyldproperties = UTDAnalytics::GetDynamicProperties(appid);
    TDAnalyticsCpp::ta_track_update(EventName, PropertiesStr, EventId, dyldproperties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        FString Dyldproperties = UTDAnalytics::GetDynamicProperties(Instance->InstanceAppID);
        Instance->TrackUpdate(EventName, PropertiesStr, EventId, Dyldproperties); 
    }
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::TrackUpdate"));
#endif
}

void UTDAnalytics::TrackOverwrite(const FString& EventName, const FString& Properties, const FString& EventId, const FString& AppId)
{
#if PLATFORM_ANDROID
    FString appid = thinkinganalytics::jni_ta_getCurrentAppId(AppId);
    FString dyldproperties = UTDAnalytics::GetDynamicProperties(appid);
    thinkinganalytics::jni_ta_track_overwrite(EventName, Properties, EventId, dyldproperties, AppId);
#elif PLATFORM_IOS
    FString appid = TDAnalyticsCpp::ta_getCurrentAppId(AppId);
    FString dyldproperties = UTDAnalytics::GetDynamicProperties(appid);
    TDAnalyticsCpp::ta_track_overwrite(EventName, Properties, EventId, dyldproperties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        FString Dyldproperties = UTDAnalytics::GetDynamicProperties(Instance->InstanceAppID);
        Instance->TrackOverwrite(EventName, Properties, EventId, Dyldproperties); 
    }
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::TrackOverwrite"));
#endif
}

void UTDAnalytics::TrackOverwrite(const FString& EventName, TSharedPtr<FJsonObject> Properties, const FString& EventId, const FString& AppId)
{
    FString PropertiesStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PropertiesStr);
    FJsonSerializer::Serialize(Properties.ToSharedRef(), Writer);

#if PLATFORM_ANDROID
    FString appid = thinkinganalytics::jni_ta_getCurrentAppId(AppId);
    FString dyldproperties = UTDAnalytics::GetDynamicProperties(appid);
    thinkinganalytics::jni_ta_track_overwrite(EventName, PropertiesStr, EventId, dyldproperties, AppId);
#elif PLATFORM_IOS
    FString appid = TDAnalyticsCpp::ta_getCurrentAppId(AppId);
    FString dyldproperties = UTDAnalytics::GetDynamicProperties(appid);
    TDAnalyticsCpp::ta_track_overwrite(EventName, PropertiesStr, EventId, dyldproperties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        FString Dyldproperties = UTDAnalytics::GetDynamicProperties(Instance->InstanceAppID);
        Instance->TrackOverwrite(EventName, PropertiesStr, EventId, Dyldproperties); 
    }
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::TrackOverwrite"));
#endif
}

void UTDAnalytics::TimeEvent(const FString& EventName, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_time_event(EventName, AppId);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_time_event(EventName, AppId);
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::TimeEvent"));
#endif
}

void UTDAnalytics::UserSet(const FString& Properties, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_user_set(Properties, AppId);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_user_set(Properties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->UserSet(Properties); 
    }
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::UserSet"));
#endif
}

void UTDAnalytics::UserSet(TSharedPtr<FJsonObject> Properties, const FString& AppId)
{
    FString PropertiesStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PropertiesStr);
    FJsonSerializer::Serialize(Properties.ToSharedRef(), Writer);

#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_user_set(PropertiesStr, AppId);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_user_set(PropertiesStr, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->UserSet(PropertiesStr); 
    }
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::UserSet"));
#endif
}

void UTDAnalytics::UserSetOnce(const FString& Properties, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_user_set_once(Properties, AppId);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_user_set_once(Properties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->UserSetOnce(Properties); 
    }
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::UserSetOnce"));
#endif
}

void UTDAnalytics::UserSetOnce(TSharedPtr<FJsonObject> Properties, const FString& AppId)
{
    FString PropertiesStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PropertiesStr);
    FJsonSerializer::Serialize(Properties.ToSharedRef(), Writer);

#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_user_set_once(PropertiesStr, AppId);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_user_set_once(PropertiesStr, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->UserSetOnce(PropertiesStr); 
    }
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::UserSetOnce"));
#endif
}

void UTDAnalytics::UserAdd(const FString& Property, const float Value, const FString& AppId)
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
    TDAnalyticsCpp::ta_user_add(outStr, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->UserAdd(outStr); 
    }
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::UserAdd"));
#endif
}

void UTDAnalytics::UserUnset(const FString& Property, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_user_unset(Property, AppId);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_user_unset(Property, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->UserUnset(Property); 
    }
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::UserUnset"));
#endif
}

void UTDAnalytics::UserAppend(const FString& Properties, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_user_append(Properties, AppId);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_user_append(Properties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->UserAppend(Properties); 
    }
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::UserAppend"));
#endif
}

void UTDAnalytics::UserAppend(TSharedPtr<FJsonObject> Properties, const FString& AppId)
{
    FString PropertiesStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PropertiesStr);
    FJsonSerializer::Serialize(Properties.ToSharedRef(), Writer);

#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_user_append(PropertiesStr, AppId);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_user_append(PropertiesStr, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->UserAppend(PropertiesStr); 
    }
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::UserAppend"));
#endif
}

void UTDAnalytics::UserUniqueAppend(const FString& Properties, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_user_unique_append(Properties, AppId);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_user_unique_append(Properties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->UserUniqueAppend(Properties); 
    }
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::UserUniqueAppend"));
#endif
}

void UTDAnalytics::UserUniqueAppend(TSharedPtr<FJsonObject> Properties, const FString& AppId)
{
    FString PropertiesStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PropertiesStr);
    FJsonSerializer::Serialize(Properties.ToSharedRef(), Writer);

#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_user_unique_append(PropertiesStr, AppId);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_user_unique_append(PropertiesStr, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->UserUniqueAppend(PropertiesStr); 
    }
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::UserUniqueAppend"));
#endif
}

void UTDAnalytics::UserDelete(const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_user_delete(AppId);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_user_delete(AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->UserDelete(); 
    }
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::UserDelete"));
#endif
}

void UTDAnalytics::SetDistinctId(const FString& DistinctId, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_identify(DistinctId, AppId);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_identify(DistinctId, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->ta_Identify(DistinctId); 
    }    
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::SetDistinctId"));
#endif
}

void UTDAnalytics::Login(const FString& AccountId, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_login(AccountId, AppId);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_login(AccountId, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {  
        Instance->ta_Login(AccountId); 
    }
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::Login"));
#endif
}

void UTDAnalytics::Logout(const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_logout(AppId);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_logout(AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->ta_Logout(); 
    }    
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::Logout"));
#endif
}

void UTDAnalytics::Flush(const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_flush(AppId);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_flush(AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->ta_Flush();
    }
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::Flush"));
#endif
}

void UTDAnalytics::CalibrateTime(int64 timestamp)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_calibrate_time(timestamp);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_calibrate_time(timestamp);
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::CalibrateTime"));
#endif
}

void UTDAnalytics::CalibrateTimeWithNtp(const FString& urlString)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_calibrate_time_ntp(urlString);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_calibrate_time_ntp(urlString);
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::calibrateTimeWithNtp"));
#endif
}

void UTDAnalytics::OptOutTracking(const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_opt_out_tracking(AppId);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_opt_out_tracking(AppId);
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::OptOutTracking"));
#endif
}

void UTDAnalytics::OptInTracking(const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_opt_in_tracking(AppId);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_opt_in_tracking(AppId);
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::OptInTracking"));
#endif
}

void UTDAnalytics::EnableTracking(bool bIsEnable, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_enable_tracking(bIsEnable, AppId);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_enable_tracking(bIsEnable, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->EnableTracking(bIsEnable); 
    }    
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::EnableTracking"));
#endif
}

FString UTDAnalytics::GetDeviceId(const FString& AppId)
{
#if PLATFORM_ANDROID
    return thinkinganalytics::jni_ta_get_device_id(AppId);
#elif PLATFORM_IOS
    return TDAnalyticsCpp::ta_get_device_id(AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
        return FString(""); 
    }
    else
    {
        return Instance->ta_GetDeviceID(); 
    }    
#else
UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::GetDeviceId"));
    return FString("");
 #endif
    
}

FString UTDAnalytics::GetDistinctId(const FString& AppId)
{
#if PLATFORM_ANDROID
    return thinkinganalytics::jni_ta_get_distinct_id(AppId);
#elif PLATFORM_IOS
    return TDAnalyticsCpp::ta_get_distinct_id(AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
        return FString(""); 
    }
    else
    {
        return Instance->ta_GetDistinctID(); 
    }     
#else
UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::GetDistinctId"));
    return FString("");
 #endif
}

void UTDAnalytics::SetSuperProperties(const FString& properties, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_set_superProperties(properties, AppId);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_set_superProperties(properties, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->ta_SetSuperProperties(properties); 
    }
#else
UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::SetSuperProperties"));

 #endif
}

void UTDAnalytics::SetSuperProperties(TSharedPtr<FJsonObject> Properties, const FString& AppId)
{
    FString PropertiesStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PropertiesStr);
    FJsonSerializer::Serialize(Properties.ToSharedRef(), Writer);
    
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_set_superProperties(PropertiesStr, AppId);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_set_superProperties(PropertiesStr, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->ta_SetSuperProperties(PropertiesStr); 
    }
#else
UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::SetSuperProperties"));

 #endif
}

void UTDAnalytics::SetTrackStatus(const FString& Status, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_set_trackStatus(Status, AppId);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_set_trackStatus(Status, AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
    }
    else
    {
        Instance->ta_SetTrackState(Status);
    }
#else
UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::SetTrackStatus"));

 #endif
}

FString UTDAnalytics::GetSuperProperties(const FString& AppId)
{
#if PLATFORM_ANDROID
    return thinkinganalytics::jni_ta_get_superProperties(AppId);
#elif PLATFORM_IOS
    return TDAnalyticsCpp::ta_get_superProperties(AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
        return FString("");
    }
    else
    {
        return Instance->ta_GetSuperProperties();
    }       
#else
UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::GetSuperProperties"));
    return FString("");
 #endif
}

FString UTDAnalytics::GetPresetProperties(const FString& AppId)
{
#if PLATFORM_ANDROID
    return thinkinganalytics::jni_ta_get_presetProperties(AppId);
#elif PLATFORM_IOS
    return TDAnalyticsCpp::ta_get_presetProperties(AppId);
#elif PLATFORM_MAC || PLATFORM_WINDOWS
    UTDAnalyticsPC* Instance = UTDAnalyticsPC::GetInstance(AppId);
    if ( Instance == nullptr )
    {
        UE_LOG(TDAnalytics, Warning, TEXT("There is no Instance!"));
        return FString("");
    }
    else
    {
        return Instance->ta_GetPresetProperties(); 
    }       
#else
UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::GetPresetProperties"));
    return FString("");
 #endif
}

FString UTDAnalytics::CreateLightInstance(const FString& AppId)
{
#if PLATFORM_ANDROID
    return thinkinganalytics::jni_ta_createLightInstance(AppId);
#elif PLATFORM_IOS
    return TDAnalyticsCpp::ta_createLightInstance(AppId);
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::CreateLightInstance"));
    return FString("");
#endif
}

void UTDAnalytics::EnableThirdPartySharing(const TArray<FString>& EventTypeList, const FString& AppId)
{
    int typeList = 0;
    for (int i = 0; i < EventTypeList.Num(); i++)
    {
        if (EventTypeList[i].Equals(FTAConstants::ThirdPartyShareTypeAPPSFLYER))
        {
            typeList = typeList | (1 << 0);
        }
        else if (EventTypeList[i].Equals(FTAConstants::ThirdPartyShareTypeIRONSOURCE))
        {
            typeList = typeList | (1 << 1);
        }
        else if (EventTypeList[i].Equals(FTAConstants::ThirdPartyShareTypeADJUST))
        {
            typeList = typeList | (1 << 2);
        }
        else if (EventTypeList[i].Equals(FTAConstants::ThirdPartyShareTypeBRANCH))
        {
            typeList = typeList | (1 << 3);
        }
        else if (EventTypeList[i].Equals(FTAConstants::ThirdPartyShareTypeTOPON))
        {
            typeList = typeList | (1 << 4);
        }
        else if (EventTypeList[i].Equals(FTAConstants::ThirdPartyShareTypeTRACKING))
        {
            typeList = typeList | (1 << 5);
        }
        else if (EventTypeList[i].Equals(FTAConstants::ThirdPartyShareTypeTRADPLUS))
        {
            typeList = typeList | (1 << 6);
        }
    }
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_enableThirdPartySharing(typeList, AppId);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_enableThirdPartySharing(typeList, AppId);
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::EnableThirdPartySharing"));
#endif 
}

void UTDAnalytics::EnableThirdPartySharingWithCustomProperties(const TArray<FString>& EventTypeList, TSharedPtr<FJsonObject> Properties, const FString& AppId)
{
int typeList = 0;
    for (int i = 0; i < EventTypeList.Num(); i++)
    {
        if (EventTypeList[i].Equals(FTAConstants::ThirdPartyShareTypeAPPSFLYER))
        {
            typeList = typeList | (1 << 0);
        }
        else if (EventTypeList[i].Equals(FTAConstants::ThirdPartyShareTypeIRONSOURCE))
        {
            typeList = typeList | (1 << 1);
        }
        else if (EventTypeList[i].Equals(FTAConstants::ThirdPartyShareTypeADJUST))
        {
            typeList = typeList | (1 << 2);
        }
        else if (EventTypeList[i].Equals(FTAConstants::ThirdPartyShareTypeBRANCH))
        {
            typeList = typeList | (1 << 3);
        }
        else if (EventTypeList[i].Equals(FTAConstants::ThirdPartyShareTypeTOPON))
        {
            typeList = typeList | (1 << 4);
        }
        else if (EventTypeList[i].Equals(FTAConstants::ThirdPartyShareTypeTRACKING))
        {
            typeList = typeList | (1 << 5);
        }
        else if (EventTypeList[i].Equals(FTAConstants::ThirdPartyShareTypeTRADPLUS))
        {
            typeList = typeList | (1 << 6);
        }
    }

    FString PropertiesStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PropertiesStr);
    FJsonSerializer::Serialize(Properties.ToSharedRef(), Writer);

#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_enableThirdPartySharingWithCustomProperties(typeList, PropertiesStr, AppId);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_enableThirdPartySharingWithCustomProperties(typeList, PropertiesStr, AppId);
#else
    UE_LOG(TDAnalytics, Warning, TEXT("Unsupported Platform. Calling UTDAnalytics::EnableThirdPartySharing"));
#endif
}

FString UTDAnalytics::GetDynamicProperties(const FString& AppId)
{
    FString jsonString = "";
    TADynamicSuperPropRetValDelegate* delegate = TADynamicSuperPropMethods.Find(*AppId);
    if(delegate != nullptr)
    {
        jsonString = delegate->Execute();
    }
    return jsonString;
}

void UTDAnalytics::SetDynamicProperties(TADynamicSuperPropRetValDelegate Del, const FString& AppId)
{
    TADynamicSuperPropMethods.Emplace(*AppId, Del);
}

void UTDAnalytics::TASetAutoTrackEventListener(TAAutoTrackEventRetValDelegate Del, const TArray<FString>& EventTypeList, const FString& AppId)
{
#if PLATFORM_ANDROID
    thinkinganalytics::jni_ta_setAutoTrackEventListener(Del, EventTypeList, AppId);
#elif PLATFORM_IOS
    TDAnalyticsCpp::ta_setAutoTrackEventListener(Del, EventTypeList, AppId);
#else
    UE_LOG(LogTemp, Warning, TEXT("Calling UTDAnalytics::EnableAutoTrack... The platform is not supported."));
#endif   
}