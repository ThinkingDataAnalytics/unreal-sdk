// Copyright 2020 Thinking Data Ltd. All Rights Reserved.

#import <vector>
#import <string>
#include "TDAnalyticsSettings.h"
#include "../Common/TAConstants.h"

DECLARE_DELEGATE_RetVal_TwoParams(FString, TAAutoTrackEventRetValDelegate, FString, FString);
static TMap<FString, TAAutoTrackEventRetValDelegate> TAAutoTrackEventIOSMethods;

class TDAnalyticsCpp 
{
public:

    static FString ta_get_superProperties(FString appid = "");

    static FString ta_get_presetProperties(FString appid = "");

    static FString ta_createLightInstance(FString appid = "");

    static FString ta_getCurrentAppId(FString appId);
    
    static void ta_calibrate_time(long timestamp);

    static void ta_initialize(FString appid, FString serverurl, TAMode mode, bool bEnableLog, FString timeZone, FString version);

    static void ta_initializeEncryptInstance(FString appid, FString serverurl, TAMode mode, bool bEnableLog, FString timeZone, FString version, FString EncryptPublicKey, int EncryptVersion, FString SymmetricEncryption, FString AsymmetricEncryption);

    static void ta_initialize(const UTDAnalyticsSettings *defaultSettings, FString version);

    static void ta_track(FString eventName, FString properties, FString dyldproperties, FString appid = "");

    static void ta_track_first(FString eventName, FString properties, FString dyldproperties, FString appid = "");

    static void ta_track_first_withId(FString eventName, FString properties, FString firstCheckId, FString dyldproperties, FString appid = "");

    static void ta_track_update(FString EventName, FString Properties, FString EventId, FString dyldproperties, FString appid = "");

    static void ta_track_overwrite(FString EventName, FString Properties, FString EventId, FString dyldproperties, FString appid = "");

    static void ta_time_event(FString eventName, FString appid = "");

    static void ta_identify(FString distinctId, FString appid = "");

    static void ta_login(FString accountId, FString appid = "");

    static void ta_logout(FString appid = "");

    static void ta_flush(FString appid = "");

    static void ta_user_set(FString properties, FString appid = "");

    static void ta_user_set_once(FString properties, FString appid = "");

    static void ta_user_unset(FString property, FString appid = "");

    static void ta_user_append(FString properties, FString appid = "");

    static void ta_user_unique_append(FString properties, FString appid = "");

    static void ta_user_add(FString properties, FString appid = "");

    static void ta_user_delete(FString appid = "");

    static void ta_enable_autotrack(FString appid = "");

    static void ta_enable_autotrack_with_type(FString appid, TArray<FString> EventTypeList);

    static void ta_enable_autotrack_with_type_and_prop(FString appid, TArray<FString> EventTypeList, FString properties);

    static void ta_opt_out_tracking(FString appid = "");

    static void ta_opt_in_tracking(FString appid = "");

    static void ta_enable_tracking(BOOL enabled, FString appid = "");

    static void ta_calibrate_time_ntp(FString urlstring);

    static void ta_set_superProperties(FString properties, FString appid = "");

    static void ta_set_trackStatus(FString status, FString appid= "");

    static FString ta_get_distinct_id(FString appid = "");

    static FString ta_get_device_id(FString appid = "");

    static void ta_setAutoTrackEventListener(TAAutoTrackEventRetValDelegate Del, TArray<FString> EventTypeList, FString AppId = "");

    static void ta_enableThirdPartySharing(int types, FString appId = "");

    static void ta_enableThirdPartySharingWithCustomProperties(int typeList, FString properties, FString appId = "");
};
