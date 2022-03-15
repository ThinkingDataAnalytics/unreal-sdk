// Copyright 2020 Thinking Data Ltd. All Rights Reserved.

#ifndef PLATFORM_IOS
#else
#if PLATFORM_IOS
#import <ThinkingSDK/ThinkingAnalyticsSDK.h>
//#import <ThinkingSDK/TDLogging.h>
#import "ThinkingAnalyticsCpp.h"
#include <sstream>

NSMutableDictionary* sInstances;
NSMutableArray*      sAppIds;


// convert

void convertToDictionary(const char *json, NSDictionary **properties_dict) 
{
    NSString *json_string = json != NULL ? [NSString stringWithUTF8String:json] : nil;
    if ( json_string) 
    {
        *properties_dict = [NSJSONSerialization JSONObjectWithData:[json_string dataUsingEncoding:NSUTF8StringEncoding] options:kNilOptions error:nil];
    }
}

const char *convertToString(NSDictionary *properties_dict) 
{
    if ( properties_dict && [NSJSONSerialization isValidJSONObject:properties_dict] ) 
    {
        NSData *jsonData = [NSJSONSerialization dataWithJSONObject:properties_dict options:0 error:NULL];;
        NSString *output = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
        if ( output ) 
        {
            return [output UTF8String];
        } 
        else 
        {
            return "";
        }
    } 
    else 
    {
        return "";
    }
}


// 多实例

NSMutableDictionary* getsInstances()
{
    if( sInstances == nil )
    {
        sInstances = [NSMutableDictionary new];
    }
    return  sInstances;
}

NSMutableArray* getAppIds()
{
    if(sAppIds == nil)
    {
        sAppIds = [NSMutableArray new];
    }
    return  sAppIds;
}

NSString* getCurrentAppId(NSString* appId)
{
    if( (appId == nil || appId.length == 0) && sAppIds.count > 0 )
    {
        return sAppIds.firstObject;
    }
    else if(appId.length > 0)
    {
        return appId;
    }
    return @"";
}

ThinkingAnalyticsSDK* getCurrentInstance(NSString *appid)
{
    ThinkingAnalyticsSDK *instance = nil;
    NSString *token = getCurrentAppId(appid);
    if(token.length > 0)
    {
        NSMutableDictionary* instances = getsInstances();
        instance = [sInstances objectForKey:token];
    }
    if(instance == nil)
    {
//        TDLogInfo(@"Instance does not exist");
        UE_LOG(LogTemp, Log, TEXT("@@@@@@@@@getCurrentInstance----->nil"));
    }
    return  instance;
}

ThinkingAnalyticsSDK* getCurrentInstance(FString appid)
{
    const char *app_id_t = TCHAR_TO_UTF8(*appid);
    NSString *app_id_string = app_id_t != NULL ? [NSString stringWithUTF8String:app_id_t] : nil;
    return getCurrentInstance(app_id_string);
}


// 初始化

void ThinkingAnalyticsCpp::ta_initialize(const UThinkingAnalyticsSettings *defaultSettings, FString version) 
{
    
    ta_initialize(*defaultSettings->AppID, *defaultSettings->ServerUrl, defaultSettings->Mode, defaultSettings->bEnableLog, *defaultSettings->TimeZone, version);
    
}

void ThinkingAnalyticsCpp::ta_initialize(FString appid, FString serverurl, TAMode mode, bool bEnableLog, FString timeZone, FString version) 
{

    if ( bEnableLog ) {
        [ThinkingAnalyticsSDK setLogLevel:TDLoggingLevelDebug];
    }

    const char *app_id = TCHAR_TO_UTF8(*appid);
    NSString *app_id_string = app_id != NULL ? [NSString stringWithUTF8String:app_id] : nil;
    
    NSString *version_string = version != NULL ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*version)] : nil;
    [ThinkingAnalyticsSDK setCustomerLibInfoWithLibName:@"Unreal" libVersion:version_string];

    ThinkingAnalyticsSDK *instance = sInstances[app_id_string];
    if(instance) return ;
    
    // 你敢信这样改了之后,url_string就能正常解析了,,,,
    NSString *url_string = nil;
    const char *url = TCHAR_TO_UTF8(*serverurl);
    FString url1(url);
    if ( url1.Len() > 0 ) 
    {
        url_string = [NSString stringWithUTF8String:TCHAR_TO_UTF8(*url1)];
    }
    
    FString TimeZoneSetting = *timeZone;
    const char *timezone_id = TCHAR_TO_UTF8(*TimeZoneSetting);
    NSString *timezone_id_string = timezone_id != NULL ? [NSString stringWithUTF8String:timezone_id] : nil;
    
    TDConfig *config = [[TDConfig alloc] init];
    uint8 mode1 = (uint8)mode;
    if ( mode1 == 1 ) 
    {
        // DEBUG
        config.debugMode = ThinkingAnalyticsDebug;
    } 
    else if ( mode1 == 2 ) 
    {
        // DEBUG_ONLY
        config.debugMode = ThinkingAnalyticsDebugOnly;
    }
    
    if(TimeZoneSetting.Len() > 0) 
    {
        NSTimeZone *nsTimezone = [NSTimeZone timeZoneWithName:timezone_id_string];
        if ( nsTimezone ) 
        {
            config.defaultTimeZone = nsTimezone;
        }
    }

    instance = [ThinkingAnalyticsSDK startWithAppId:app_id_string withUrl:url_string withConfig:config];
    NSMutableDictionary* instances = getsInstances();
    [instances setObject:instance forKey:app_id_string];
    NSMutableArray* appIds = getAppIds();
    [appIds addObject:app_id_string];
}

// in-appid for lightInstance'uuid
// common, in-appid == appid
void ThinkingAnalyticsCpp::ta_track(FString eventName, FString properties, FString dyldproperties, FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if( instance )
    {
        NSString *event_name_string = eventName != NULL ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*eventName)] : nil;
        NSMutableDictionary *resultDic = [NSMutableDictionary new];
        NSDictionary *properties_dict = nil;
        NSDictionary *properties_dict_dyld = nil;
        convertToDictionary(TCHAR_TO_UTF8(*properties), &properties_dict);
        convertToDictionary(TCHAR_TO_UTF8(*dyldproperties), &properties_dict_dyld);
        if ( properties_dict_dyld ) 
        {
            [resultDic addEntriesFromDictionary:properties_dict_dyld];
        }
        if ( properties_dict ) 
        {
            [resultDic addEntriesFromDictionary:properties_dict];
        }
        [instance track:event_name_string properties:resultDic];
    }
}

void ThinkingAnalyticsCpp::ta_track_first(FString eventName, FString properties, FString dyldproperties, FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if( instance )
    {
        NSString *event_name_string = eventName != NULL ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*eventName)] : nil;
        TDFirstEventModel *eventModel = [[TDFirstEventModel alloc] initWithEventName:event_name_string];
        NSDictionary *properties_dict = nil;
        convertToDictionary(TCHAR_TO_UTF8(*properties), &properties_dict);
        eventModel.properties = properties_dict;
        [instance trackWithEventModel:eventModel];
    }
}

void ThinkingAnalyticsCpp::ta_track_first_withId(FString eventName, FString properties, FString firstCheckId, FString dyldproperties, FString appid) 
{
    
    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if( instance )
    {
        NSString *event_name_string = eventName != NULL ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*eventName)] : nil;
        NSString *first_check_id_string = firstCheckId != NULL ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*firstCheckId)] : nil;
        TDFirstEventModel *eventModel = [[TDFirstEventModel alloc] initWithEventName:event_name_string firstCheckID:first_check_id_string];
        NSDictionary *properties_dict = nil;
        convertToDictionary(TCHAR_TO_UTF8(*properties), &properties_dict);
        eventModel.properties = properties_dict;
        [instance trackWithEventModel:eventModel];
    }
    
}

void ThinkingAnalyticsCpp::ta_track_update(FString eventName, FString properties, FString eventId, FString dyldproperties, FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if( instance )
    {
        NSString *event_name_string = eventName != NULL ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*eventName)] : nil;
        NSString *event_id_string = eventId != NULL ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*eventId)] : nil;
        TDUpdateEventModel *eventModel = [[TDUpdateEventModel alloc] initWithEventName:event_name_string eventID:event_id_string];
        NSDictionary *properties_dict = nil;
        convertToDictionary(TCHAR_TO_UTF8(*properties), &properties_dict);
        eventModel.properties = properties_dict;
        [instance trackWithEventModel:eventModel];
    }
    
}

void ThinkingAnalyticsCpp::ta_track_overwrite(FString eventName, FString properties, FString eventId, FString dyldproperties, FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if( instance )
    {
        NSString *event_name_string = eventName != NULL ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*eventName)] : nil;
        NSString *event_id_string = eventId != NULL ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*eventId)] : nil;
        TDOverwriteEventModel *eventModel = [[TDOverwriteEventModel alloc] initWithEventName:event_name_string eventID:event_id_string];
        NSDictionary *properties_dict = nil;
        convertToDictionary(TCHAR_TO_UTF8(*properties), &properties_dict);
        eventModel.properties = properties_dict;
        [instance trackWithEventModel:eventModel];
    }
}

void ThinkingAnalyticsCpp::ta_enable_autotrack(FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if( instance )
    {
        [instance enableAutoTrack: 35];
    }
}

void ThinkingAnalyticsCpp::ta_enable_autotrack_with_type(FString appid, TArray<FString> EventTypeList)
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if( instance )
    {
        uint32 typeList = 0;
        for (uint32 i = 0; i < EventTypeList.Num(); i++)
        {
            if (EventTypeList[i].Equals(FTAConstants::AUTOTRACK_EVENTTYPE_START))
            {
                typeList = typeList | (1 << 0);
            }
            else if (EventTypeList[i].Equals(FTAConstants::AUTOTRACK_EVENTTYPE_END))
            {
                typeList = typeList | (1 << 1);
            }
            else if (EventTypeList[i].Equals(FTAConstants::AUTOTRACK_EVENTTYPE_CLICK))
            {
                typeList = typeList | (1 << 2);
            }
            else if (EventTypeList[i].Equals(FTAConstants::AUTOTRACK_EVENTTYPE_VIEW_SCREEN))
            {
                typeList = typeList | (1 << 3);
            }
            else if (EventTypeList[i].Equals(FTAConstants::AUTOTRACK_EVENTTYPE_CRASH))
            {
                typeList = typeList | (1 << 4);
            }
            else if (EventTypeList[i].Equals(FTAConstants::AUTOTRACK_EVENTTYPE_INSTALL))
            {
                typeList = typeList | (1 << 5);
            }
        }
        [instance enableAutoTrack: typeList];
    }
}

void ThinkingAnalyticsCpp::ta_enable_autotrack_with_type_and_prop(FString appid, TArray<FString> EventTypeList, FString properties)
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if( instance )
    {
        uint32 typeList = 0;
        for (uint32 i = 0; i < EventTypeList.Num(); i++)
        {
            if (EventTypeList[i].Equals(FTAConstants::AUTOTRACK_EVENTTYPE_START))
            {
                typeList = typeList | (1 << 0);
            }
            else if (EventTypeList[i].Equals(FTAConstants::AUTOTRACK_EVENTTYPE_END))
            {
                typeList = typeList | (1 << 1);
            }
            else if (EventTypeList[i].Equals(FTAConstants::AUTOTRACK_EVENTTYPE_CLICK))
            {
                typeList = typeList | (1 << 2);
            }
            else if (EventTypeList[i].Equals(FTAConstants::AUTOTRACK_EVENTTYPE_VIEW_SCREEN))
            {
                typeList = typeList | (1 << 3);
            }
            else if (EventTypeList[i].Equals(FTAConstants::AUTOTRACK_EVENTTYPE_CRASH))
            {
                typeList = typeList | (1 << 4);
            }
            else if (EventTypeList[i].Equals(FTAConstants::AUTOTRACK_EVENTTYPE_INSTALL))
            {
                typeList = typeList | (1 << 5);
            }
        }
        NSDictionary *properties_dict = nil;
        convertToDictionary(TCHAR_TO_UTF8(*properties), &properties_dict);
        [instance enableAutoTrack: typeList properties:properties_dict];
    }
}

void ThinkingAnalyticsCpp::ta_identify(FString distinctId, FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if( instance )
    {
        NSString *id_string = distinctId != NULL ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*distinctId)] : nil;
        [instance identify:id_string];
    }
}

void ThinkingAnalyticsCpp::ta_calibrate_time(long timestamp) 
{
    [ThinkingAnalyticsSDK calibrateTime:timestamp];
}

void ThinkingAnalyticsCpp::ta_calibrate_time_ntp(FString urlstring) 
{
    if ( urlstring != NULL ) 
    {
        CFStringRef cfstr = FPlatformString::TCHARToCFString(*urlstring);
        NSString *nsstr = (NSString *)cfstr;
        [ThinkingAnalyticsSDK calibrateTimeWithNtp:nsstr];
    }
}


void ThinkingAnalyticsCpp::ta_time_event(FString eventName, FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if( instance )
    {
        NSString *event_name_string = eventName != NULL ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*eventName)] : nil;
        [instance timeEvent:event_name_string];
    }
}

void ThinkingAnalyticsCpp::ta_login(FString accountId, FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if( instance )
    {
        NSString *id_string = accountId != NULL ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*accountId)] : nil;
        [instance login:id_string];
    }
}

void ThinkingAnalyticsCpp::ta_logout(FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if( instance )
    {
        [instance logout];
    }
}
void ThinkingAnalyticsCpp::ta_flush(FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if( instance )
    {
        [instance flush];
    }
}

void ThinkingAnalyticsCpp::ta_user_set(FString properties, FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    NSDictionary *properties_dict = nil;
    convertToDictionary(TCHAR_TO_UTF8(*properties), &properties_dict);
    if ( properties_dict && instance ) 
    {
        [instance user_set:properties_dict];
    }
}

void ThinkingAnalyticsCpp::ta_user_set_once(FString properties, FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    NSDictionary *properties_dict = nil;
    convertToDictionary(TCHAR_TO_UTF8(*properties), &properties_dict);
    if ( properties_dict && instance ) 
    {
        [instance user_setOnce:properties_dict];
    }
}

void ThinkingAnalyticsCpp::ta_user_unset(FString property, FString appid) 
{


    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if( instance )
    {
        NSString *property_name_string = property != NULL ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*property)] : nil;
        [instance user_unset:property_name_string];
    }
}

void ThinkingAnalyticsCpp::ta_user_append(FString properties, FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    NSDictionary *properties_dict = nil;
    convertToDictionary(TCHAR_TO_UTF8(*properties), &properties_dict);
    if ( properties_dict && instance ) 
    {
        [instance user_append:properties_dict];
    }
}

void ThinkingAnalyticsCpp::ta_user_add(FString properties, FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    NSDictionary *properties_dict = nil;
    convertToDictionary(TCHAR_TO_UTF8(*properties), &properties_dict);
    if ( properties_dict && instance ) 
    {
        [instance user_add:properties_dict];
    }
}

void ThinkingAnalyticsCpp::ta_user_delete(FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if ( instance ) 
    {
        [instance user_delete];
    }
}

void ThinkingAnalyticsCpp::ta_opt_out_tracking(FString appid)
{
    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if ( instance ) 
    {
        [instance optOutTracking];
    }
}

void ThinkingAnalyticsCpp::ta_opt_in_tracking(FString appid) 
{
    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if ( instance ) 
    {
        [instance optInTracking];
    }
}

void ThinkingAnalyticsCpp::ta_enable_tracking(BOOL enabled, FString appid)
{
    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if ( instance ) 
    {
        if ( enabled ) 
        {
            [instance enableTracking:YES];
        } 
        else 
        {
            [instance enableTracking:NO];
        }
    }
}

FString ThinkingAnalyticsCpp::ta_get_distinct_id(FString appid)
{
    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if ( instance ) 
    {
       NSString *distinctId = [instance getDistinctId];
       const char *str = [distinctId cStringUsingEncoding:NSUTF8StringEncoding];
       FString str1(str);
       return str1;
    }
    return FString();
}

FString ThinkingAnalyticsCpp::ta_get_device_id(FString appid)
{
    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
     if ( instance ) 
     {
       NSString *deviceId = [instance getDeviceId];
       const char *str = [deviceId cStringUsingEncoding:NSUTF8StringEncoding];
       FString str1(str);
       return str1;
    }
    return FString();
}

FString ThinkingAnalyticsCpp::ta_get_superProperties(FString appid) 
{
    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if ( instance ) 
    {
        NSDictionary *properties_dict = [instance currentSuperProperties];
        const char *str = convertToString(properties_dict);
        FString str1 = UTF8_TO_TCHAR(str);
        return str1;
    } 
    else
    {
        return FString();
    }
}


FString ThinkingAnalyticsCpp::ta_get_presetProperties(FString appid) 
{
    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if ( instance ) 
    {
        NSDictionary *properties_dict = [[instance getPresetProperties] toEventPresetProperties];
        const char *str = convertToString(properties_dict);
        FString str1 = UTF8_TO_TCHAR(str);
        return str1;
    } 
    else 
    {
        return FString();
    }
}

void ThinkingAnalyticsCpp::ta_set_superProperties(FString properties, FString appid) 
{
    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if ( instance ) 
    {
        NSDictionary *properties_dict = nil;
        convertToDictionary(TCHAR_TO_UTF8(*properties), &properties_dict);
        if ( properties_dict ) 
        {
            [instance setSuperProperties:properties_dict];
        }
    }
}

FString ThinkingAnalyticsCpp::ta_createLightInstance(FString appid)
{
    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if( instance )
    {
        const char *appid1 =  [[instance valueForKey:@"appid"] cStringUsingEncoding:NSUTF8StringEncoding];
        FString result(appid1);
        ThinkingAnalyticsSDK *lightInstance =  [instance createLightInstance];
        NSString *uuid = [NSUUID UUID].UUIDString;
        const char *uuid1 =  [uuid cStringUsingEncoding:NSUTF8StringEncoding];
        FString result1(uuid1);
    
        NSMutableDictionary* instances = getsInstances();
        [instances setObject:lightInstance forKey:uuid];
        NSMutableArray* appIds = getAppIds();
        [appIds addObject:uuid];
        return result1;
    }
    else
    {
        return "";
    }
}

FString ThinkingAnalyticsCpp::ta_getCurrentAppId(FString appId)
{
    if ( appId.Len() > 0 ) 
    {
        return appId;
    } 
    else 
    {
        const char *app_id_t = TCHAR_TO_UTF8(*appId);
        NSString *app_id_string = app_id_t != NULL ? [NSString stringWithUTF8String:app_id_t] : nil;
        NSString *token = getCurrentAppId(app_id_string);
        const char *appid1 =  [token cStringUsingEncoding:NSUTF8StringEncoding];
        FString result(appid1);
        return result;
    }
}

#endif
#endif
