// Copyright 2020 Thinking Data Ltd. All Rights Reserved.

#ifndef PLATFORM_IOS
#else
#if PLATFORM_IOS
#import <ThinkingSDK/ThinkingAnalyticsSDK.h>
//#import <ThinkingSDK/TDLogging.h>
#import "TDAnalyticsCpp.h"
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
    NSString *app_id_string = (*appid != nullptr && !appid.IsEmpty()) ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*appid)] : nil;
    return getCurrentInstance(app_id_string);
}

void TDAnalyticsCpp::ta_initialize(const UTDAnalyticsSettings *defaultSettings, FString version) 
{
    if ( defaultSettings->bEnableEncrypt )
    {
        ta_initializeEncryptInstance(*defaultSettings->AppID, *defaultSettings->ServerUrl, defaultSettings->Mode, defaultSettings->bEnableLog, *defaultSettings->TimeZone, version, *defaultSettings->EncryptPublicKey, defaultSettings->EncryptVersion, *defaultSettings->SymmetricEncryption, *defaultSettings->AsymmetricEncryption);
    }
    else
    {
        ta_initialize(*defaultSettings->AppID, *defaultSettings->ServerUrl, defaultSettings->Mode, defaultSettings->bEnableLog, *defaultSettings->TimeZone, version);
    }
}

void TDAnalyticsCpp::ta_initialize(FString appid, FString serverurl, TAMode mode, bool bEnableLog, FString timeZone, FString version) 
{
    if ( bEnableLog ) {
        [ThinkingAnalyticsSDK setLogLevel:TDLoggingLevelDebug];
    }

    NSString *app_id_string = (*appid != nullptr && !appid.IsEmpty()) ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*appid)] : nil;
    NSString *version_string = (*version != nullptr && !version.IsEmpty()) ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*version)] : nil;
    [ThinkingAnalyticsSDK setCustomerLibInfoWithLibName:@"Unreal" libVersion:version_string];

    ThinkingAnalyticsSDK *instance = sInstances[app_id_string];
    if(instance) return ;
    
    NSString *url_string  = (*serverurl != nullptr && !serverurl.IsEmpty()) ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*serverurl)] : nil;    
    NSString *timezone_id_string = (*timeZone != nullptr && !timeZone.IsEmpty()) ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*timeZone)] : nil;
    
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
    
    if(timezone_id_string != nil) 
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

void TDAnalyticsCpp::ta_initializeEncryptInstance(FString appid, FString serverurl, TAMode mode, bool bEnableLog, FString timeZone, FString version, FString EncryptPublicKey, int EncryptVersion, FString SymmetricEncryption, FString AsymmetricEncryption)
{
    if ( bEnableLog ) {
        [ThinkingAnalyticsSDK setLogLevel:TDLoggingLevelDebug];
    }

    NSString *app_id_string = (*appid != nullptr && !appid.IsEmpty()) ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*appid)] : nil;
    NSString *version_string = (*version != nullptr && !version.IsEmpty()) ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*version)] : nil;
    [ThinkingAnalyticsSDK setCustomerLibInfoWithLibName:@"Unreal" libVersion:version_string];

    ThinkingAnalyticsSDK *instance = sInstances[app_id_string];
    if(instance) return ;
    
    NSString *url_string = (*serverurl != nullptr && !serverurl.IsEmpty()) ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*serverurl)] : nil;    
    NSString *timezone_id_string = (*timeZone != nullptr && !timeZone.IsEmpty()) ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*timeZone)] : nil;
    
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
    
    if(timezone_id_string != nil) 
    {
        NSTimeZone *nsTimezone = [NSTimeZone timeZoneWithName:timezone_id_string];
        if ( nsTimezone ) 
        {
            config.defaultTimeZone = nsTimezone;
        }
    }

    config.enableEncrypt = YES;
    NSString *encryptPublicKeyStr = (*EncryptPublicKey != nullptr && !EncryptPublicKey.IsEmpty()) ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*EncryptPublicKey)] : nil;
    NSString *asymmetricEncryptionStr = (*AsymmetricEncryption != nullptr && !AsymmetricEncryption.IsEmpty()) ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*AsymmetricEncryption)] : nil;
    NSString *symmetricEncryptionStr = (*SymmetricEncryption != nullptr && !SymmetricEncryption.IsEmpty()) ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*SymmetricEncryption)] : nil;

    config.secretKey = [[TDSecretKey alloc] initWithVersion:EncryptVersion publicKey:encryptPublicKeyStr asymmetricEncryption:asymmetricEncryptionStr symmetricEncryption:symmetricEncryptionStr];

    instance = [ThinkingAnalyticsSDK startWithAppId:app_id_string withUrl:url_string withConfig:config];
    NSMutableDictionary* instances = getsInstances();
    [instances setObject:instance forKey:app_id_string];
    NSMutableArray* appIds = getAppIds();
    [appIds addObject:app_id_string];
}

// in-appid for lightInstance'uuid
// common, in-appid == appid
void TDAnalyticsCpp::ta_track(FString eventName, FString properties, FString dyldproperties, FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if( instance )
    {
        NSString *event_name_string = (*eventName != nullptr && !eventName.IsEmpty()) ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*eventName)] : nil;
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

void TDAnalyticsCpp::ta_track_first(FString eventName, FString properties, FString dyldproperties, FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if( instance )
    {
        NSString *event_name_string = (*eventName != nullptr && !eventName.IsEmpty()) ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*eventName)] : nil;
        TDFirstEventModel *eventModel = [[TDFirstEventModel alloc] initWithEventName:event_name_string];
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
        eventModel.properties = resultDic;
        [instance trackWithEventModel:eventModel];
    }
}

void TDAnalyticsCpp::ta_track_first_withId(FString eventName, FString properties, FString firstCheckId, FString dyldproperties, FString appid) 
{
    
    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if( instance )
    {
        NSString *event_name_string = (*eventName != nullptr && !eventName.IsEmpty()) ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*eventName)] : nil;
        NSString *first_check_id_string = (*firstCheckId != nullptr && !firstCheckId.IsEmpty()) ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*firstCheckId)] : nil;
        TDFirstEventModel *eventModel = [[TDFirstEventModel alloc] initWithEventName:event_name_string firstCheckID:first_check_id_string];
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
        eventModel.properties = resultDic;
        [instance trackWithEventModel:eventModel];
    }
    
}

void TDAnalyticsCpp::ta_track_update(FString eventName, FString properties, FString eventId, FString dyldproperties, FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if( instance )
    {
        NSString *event_name_string = (*eventName != nullptr && !eventName.IsEmpty()) ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*eventName)] : nil;
        NSString *event_id_string = (*eventId != nullptr && !eventId.IsEmpty()) ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*eventId)] : nil;
        TDUpdateEventModel *eventModel = [[TDUpdateEventModel alloc] initWithEventName:event_name_string eventID:event_id_string];
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
        eventModel.properties = resultDic;
        [instance trackWithEventModel:eventModel];
    }
    
}

void TDAnalyticsCpp::ta_track_overwrite(FString eventName, FString properties, FString eventId, FString dyldproperties, FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if( instance )
    {
        NSString *event_name_string = (*eventName != nullptr && !eventName.IsEmpty()) ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*eventName)] : nil;
        NSString *event_id_string = (*eventId != nullptr && !eventId.IsEmpty()) ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*eventId)] : nil;
        TDOverwriteEventModel *eventModel = [[TDOverwriteEventModel alloc] initWithEventName:event_name_string eventID:event_id_string];
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
        eventModel.properties = resultDic;
        [instance trackWithEventModel:eventModel];
    }
}

void TDAnalyticsCpp::ta_enable_autotrack(FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if( instance )
    {
        [instance enableAutoTrack: 35];
    }
}

void TDAnalyticsCpp::ta_enable_autotrack_with_type(FString appid, TArray<FString> EventTypeList)
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

void TDAnalyticsCpp::ta_enable_autotrack_with_type_and_prop(FString appid, TArray<FString> EventTypeList, FString properties)
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

void TDAnalyticsCpp::ta_setAutoTrackEventListener(TAAutoTrackEventRetValDelegate Del, TArray<FString> EventTypeList, FString AppId)
{
    TAAutoTrackEventIOSMethods.Emplace(*AppId, Del);

    ThinkingAnalyticsSDK *instance = getCurrentInstance(AppId);
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
        
        [instance enableAutoTrack: typeList callback:^NSDictionary * _Nonnull(ThinkingAnalyticsAutoTrackEventType eventType, NSDictionary * _Nonnull properties) {
            FString jsonString = "";
            FString eventName = "";
            TAAutoTrackEventRetValDelegate* delegate = TAAutoTrackEventIOSMethods.Find(*AppId);
            if(delegate != nullptr)
            {
                if (eventType == ThinkingAnalyticsEventTypeAppStart)
                {
                    eventName = FTAConstants::AUTOTRACK_EVENTTYPE_START;
                }
                else if (eventType == ThinkingAnalyticsEventTypeAppInstall)
                {
                    eventName = FTAConstants::AUTOTRACK_EVENTTYPE_INSTALL;
                }
                else if (eventType == ThinkingAnalyticsEventTypeAppEnd)
                {
                    eventName = FTAConstants::AUTOTRACK_EVENTTYPE_END;
                }
                else if (eventType == ThinkingAnalyticsEventTypeAppViewScreen)
                {
                    eventName = FTAConstants::AUTOTRACK_EVENTTYPE_VIEW_SCREEN;
                }
                else if (eventType == ThinkingAnalyticsEventTypeAppClick)
                {
                    eventName = FTAConstants::AUTOTRACK_EVENTTYPE_CLICK;
                }
                else if (eventType == ThinkingAnalyticsEventTypeAppViewCrash)
                {
                    eventName = FTAConstants::AUTOTRACK_EVENTTYPE_CRASH;
                }

                jsonString = delegate->Execute(eventName, UTF8_TO_TCHAR(convertToString(properties)));
            }
            NSDictionary *properties_dict = nil;
            convertToDictionary(TCHAR_TO_UTF8(*jsonString), &properties_dict);
            return properties_dict;
        }];
    }
}

void TDAnalyticsCpp::ta_identify(FString distinctId, FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if( instance )
    {
        NSString *id_string = (*distinctId != nullptr && !distinctId.IsEmpty()) ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*distinctId)] : nil;
        [instance identify:id_string];
    }
}

void TDAnalyticsCpp::ta_calibrate_time(long timestamp) 
{
    [ThinkingAnalyticsSDK calibrateTime:timestamp];
}

void TDAnalyticsCpp::ta_calibrate_time_ntp(FString urlstring) 
{
    if ( *urlstring != nullptr && !urlstring.IsEmpty() )
    {
        CFStringRef cfstr = FPlatformString::TCHARToCFString(*urlstring);
        NSString *nsstr = (NSString *)cfstr;
        [ThinkingAnalyticsSDK calibrateTimeWithNtp:nsstr];
    }
}


void TDAnalyticsCpp::ta_time_event(FString eventName, FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if( instance )
    {
        NSString *event_name_string = (*eventName != nullptr && !eventName.IsEmpty()) ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*eventName)] : nil;
        [instance timeEvent:event_name_string];
    }
}

void TDAnalyticsCpp::ta_login(FString accountId, FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if( instance )
    {
        NSString *id_string = (*accountId != nullptr && !accountId.IsEmpty()) ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*accountId)] : nil;
        [instance login:id_string];
    }
}

void TDAnalyticsCpp::ta_logout(FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if( instance )
    {
        [instance logout];
    }
}
void TDAnalyticsCpp::ta_flush(FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if( instance )
    {
        [instance flush];
    }
}

void TDAnalyticsCpp::ta_user_set(FString properties, FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    NSDictionary *properties_dict = nil;
    convertToDictionary(TCHAR_TO_UTF8(*properties), &properties_dict);
    if ( properties_dict && instance ) 
    {
        [instance user_set:properties_dict];
    }
}

void TDAnalyticsCpp::ta_user_set_once(FString properties, FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    NSDictionary *properties_dict = nil;
    convertToDictionary(TCHAR_TO_UTF8(*properties), &properties_dict);
    if ( properties_dict && instance ) 
    {
        [instance user_setOnce:properties_dict];
    }
}

void TDAnalyticsCpp::ta_user_unset(FString property, FString appid) 
{


    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if( instance )
    {
        NSString *property_name_string = (*property != nullptr && !property.IsEmpty()) ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*property)] : nil;
        [instance user_unset:property_name_string];
    }
}

void TDAnalyticsCpp::ta_user_append(FString properties, FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    NSDictionary *properties_dict = nil;
    convertToDictionary(TCHAR_TO_UTF8(*properties), &properties_dict);
    if ( properties_dict && instance ) 
    {
        [instance user_append:properties_dict];
    }
}

void TDAnalyticsCpp::ta_user_unique_append(FString properties, FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    NSDictionary *properties_dict = nil;
    convertToDictionary(TCHAR_TO_UTF8(*properties), &properties_dict);
    if ( properties_dict && instance ) 
    {
        [instance user_uniqAppend:properties_dict];
    }
}

void TDAnalyticsCpp::ta_user_add(FString properties, FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    NSDictionary *properties_dict = nil;
    convertToDictionary(TCHAR_TO_UTF8(*properties), &properties_dict);
    if ( properties_dict && instance ) 
    {
        [instance user_add:properties_dict];
    }
}

void TDAnalyticsCpp::ta_user_delete(FString appid) 
{

    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if ( instance ) 
    {
        [instance user_delete];
    }
}

void TDAnalyticsCpp::ta_opt_out_tracking(FString appid)
{
    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if ( instance ) 
    {
        [instance optOutTracking];
    }
}

void TDAnalyticsCpp::ta_opt_in_tracking(FString appid) 
{
    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if ( instance ) 
    {
        [instance optInTracking];
    }
}

void TDAnalyticsCpp::ta_enable_tracking(BOOL enabled, FString appid)
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

FString TDAnalyticsCpp::ta_get_distinct_id(FString appid)
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

FString TDAnalyticsCpp::ta_get_device_id(FString appid)
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

FString TDAnalyticsCpp::ta_get_superProperties(FString appid) 
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


FString TDAnalyticsCpp::ta_get_presetProperties(FString appid) 
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

void TDAnalyticsCpp::ta_set_superProperties(FString properties, FString appid) 
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

void TDAnalyticsCpp::ta_set_trackStatus(FString status, FString appid)
{
    ThinkingAnalyticsSDK *instance = getCurrentInstance(appid);
    if ( instance )
    {
        if (status.Equals(FTAConstants::TRACK_STATUS_PAUSE))
        {
            [instance setTrackStatus:TATrackStatusPause];
        }
        else if (status.Equals(FTAConstants::TRACK_STATUS_STOP))
        {
            [instance setTrackStatus:TATrackStatusStop];
        }
        else if (status.Equals(FTAConstants::TRACK_STATUS_SAVE_ONLY))
        {
            [instance setTrackStatus:TATrackStatusSaveOnly];
        }
        else
        {
            [instance setTrackStatus:TATrackStatusNormal];
        }
    }
}

FString TDAnalyticsCpp::ta_createLightInstance(FString appid)
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

FString TDAnalyticsCpp::ta_getCurrentAppId(FString appId)
{
    if ( appId.Len() > 0 ) 
    {
        return appId;
    } 
    else 
    {
        NSString *app_id_string = (*appId != nullptr && !appId.IsEmpty()) ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*appId)] : nil;        
        NSString *token = getCurrentAppId(app_id_string);
        const char *appid1 =  [token cStringUsingEncoding:NSUTF8StringEncoding];
        FString result(appid1);
        return result;
    }
}

void TDAnalyticsCpp::ta_enableThirdPartySharing(int types, FString appId)
{
    ThinkingAnalyticsSDK *instance = getCurrentInstance(appId);
    if( instance )
    {
        [instance enableThirdPartySharing:types];
    }
}

void TDAnalyticsCpp::ta_enableThirdPartySharingWithCustomProperties(int typeList, FString properties, FString appId)
{
    ThinkingAnalyticsSDK *instance = getCurrentInstance(appId);
    if( instance )
    {
        NSDictionary *properties_dict = nil;
        convertToDictionary(TCHAR_TO_UTF8(*properties), &properties_dict);
        [instance enableThirdPartySharing:typeList customMap:properties_dict];
    }
}
#endif
#endif
