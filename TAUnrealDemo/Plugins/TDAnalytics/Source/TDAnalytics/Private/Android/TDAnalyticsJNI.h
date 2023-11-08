// Copyright 2020 Thinking Data Ltd. All Rights Reserved.

#pragma once

#include <vector>
#include <string>
#include "TDAnalyticsSettings.h"

DECLARE_DELEGATE_RetVal_TwoParams(FString, TAAutoTrackEventRetValDelegate, FString, FString);
static TMap<FString, TAAutoTrackEventRetValDelegate> TAAutoTrackEventAndroidMethods;

namespace thinkinganalytics 
{
    extern "C"
    {
        FString jni_ta_get_distinct_id(FString appid);

        FString jni_ta_get_device_id(FString appid);

        FString jni_ta_get_superProperties(FString appid);

        FString jni_ta_get_presetProperties(FString appid);

        FString jni_ta_createLightInstance(FString appid);

        FString jni_ta_getCurrentAppId(FString appId);

        void jni_ta_enable_log();

        void jni_ta_calibrate_time(long timestamp);

        void jni_ta_logout(FString appid);

        void jni_ta_flush(FString appid);

        void jni_ta_initialize(const UTDAnalyticsSettings *defaultSettings, FString version);

        void jni_ta_track(FString eventName, FString properties, FString dyldproperties, FString appid);

        void jni_ta_track_first(FString eventName, FString properties, FString dyldproperties, FString appid);

        void jni_ta_track_first_withId(FString eventName, FString properties, FString firstCheckId, FString dyldproperties, FString appid);

        void jni_ta_initializeInstance(FString appid, FString serverurl, TAMode mode, bool bEnableLog, FString timeZone, FString version);

        void jni_ta_initializeEncryptInstance(FString appid, FString serverurl, TAMode mode, bool bEnableLog, FString timeZone, FString version, bool bEnableEncrypt, FString EncryptPublicKey, int EncryptVersion, FString SymmetricEncryption, FString AsymmetricEncryption);

        void jni_ta_track_update(FString eventName, FString properties, FString eventId, FString dyldproperties, FString appid);

        void jni_ta_track_overwrite(FString eventName, FString properties, FString eventId, FString dyldproperties, FString appid);

        void jni_ta_time_event(FString eventName, FString appid);

        void jni_ta_identify(FString distinctId, FString appid);

        void jni_ta_login(FString accountId, FString appid);

        void jni_ta_user_set(FString properties, FString appid);

        void jni_ta_user_set_once(FString properties, FString appid);

        void jni_ta_user_unset(FString property, FString appid);

        void jni_ta_user_append(FString properties, FString appid);

        void jni_ta_user_unique_append(FString properties, FString appid);

        void jni_ta_user_add(FString properties, FString appid);

        void jni_ta_user_delete(FString appid);

        void jni_ta_enable_autotrack(FString appid);

        void jni_ta_enable_autotrack_with_type(FString appid, TArray<FString> EventTypeList);

        void jni_ta_enable_autotrack_with_type_and_prop(FString appid, TArray<FString> EventTypeList, FString Properties);

        void jni_ta_opt_out_tracking(FString appid);

        void jni_ta_opt_in_tracking(FString appid);

        void jni_ta_enable_tracking(bool enabled, FString appid);

        void jni_ta_calibrate_time_ntp(FString urlstring);

        void jni_ta_set_superProperties(FString properties, FString appid);

        void jni_ta_set_trackStatus(FString status, FString appid);

        void jni_ta_setAutoTrackEventListener(TAAutoTrackEventRetValDelegate Del, const TArray<FString> EventTypeList, const FString& AppId);

        void jni_ta_enableThirdPartySharing(int types, FString appid);

        void jni_ta_enableThirdPartySharingWithCustomProperties(int typeList, FString properties, FString appId);
    }
}