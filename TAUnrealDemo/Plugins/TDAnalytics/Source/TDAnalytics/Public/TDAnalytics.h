// Copyright 2020 Thinking Data Inc. All Rights Reserved.

#pragma once

#include <string>
#include <map>
#include <vector>

#include "UObject/Object.h"
#include "TDAnalyticsSettings.h"
#include "TDAnalytics.generated.h"

/**
 * \mainpage
 * \section main_introduction Introduction
 * # ThinkingData SDK for Unreal
 * ![output](http://image.thinkingdata.cn/images/common/logo-en.png)
 * 
 * This is the [ThinkingData](https://www.thinkingdata.io)™ SDK for Unreal. Documentation is available on our help center in the following languages:
 * 
 * - [English](https://docs.thinkingdata.cn/ta-manual/latest/en/installation/installation_menu/client_sdk/game_engine_sdk_installation/unreal_sdk_installation/unreal_sdk_installation.html)
 * - [中文](https://docs.thinkingdata.cn/ta-manual/latest/installation/installation_menu/client_sdk/game_engine_sdk_installation/unreal_sdk_installation/unreal_sdk_installation.html)
 * - [日本語](https://docs.thinkingdata.cn/ta-manual/latest/ja/installation/installation_menu/client_sdk/game_engine_sdk_installation/unreal_sdk_installation/unreal_sdk_installation.html)
 * 
 */

typedef FString(*GetDynamicSuperProperties)();

DECLARE_DELEGATE_RetVal(FString, TADynamicSuperPropRetValDelegate);
DECLARE_DELEGATE_RetVal_TwoParams(FString, TAAutoTrackEventRetValDelegate, FString, FString);

static TMap<FString, TADynamicSuperPropRetValDelegate> TADynamicSuperPropMethods;

UCLASS()
class TDANALYTICS_API UTDAnalytics : public UObject
{
    GENERATED_UCLASS_BODY()
    
private:
    
    static FString GetDynamicProperties(const FString& AppId = "");

    static void SetDynamicProperties(TADynamicSuperPropRetValDelegate Del, const FString& AppId = "");

    static void TASetAutoTrackEventListener(TAAutoTrackEventRetValDelegate Del, const TArray<FString>& EventTypeList, const FString& AppId = "");


public:
    // 初始化方法
    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void Initialize();
    
    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void InitializeInstance(const FString& appid, const FString& serverurl, TAMode mode, bool bEnableLog, const FString& timeZone);

    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void InitializeEncryptInstance(const FString& appid, const FString& serverurl, TAMode mode, bool bEnableLog, const FString& timeZone, bool bEnableEncrypt, const FString& EncryptPublicKey, int EncryptVersion, const FString& SymmetricEncryption, const FString& AsymmetricEncryption);
    
    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void CalibrateTime(int64 timestamp);

    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void EnableAutoTrack(const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void EnableAutoTrackWithType(const TArray<FString>& EventTypeList, const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void EnableAutoTrackWithTypeAndProperties(const TArray<FString>& EventTypeList, const FString& Properties, const FString& AppId = "");

    // UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void EnableAutoTrackWithTypeAndProperties(const TArray<FString>& EventTypeList, TSharedPtr<FJsonObject> Properties, const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void CalibrateTimeWithNtp(const FString& urlString);
    
    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void Track(const FString& EventName, const FString& Properties, const FString& AppId = "");

    // UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void Track(const FString& EventName, TSharedPtr<FJsonObject> Properties, const FString& AppId = "");
    
    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void TrackFirst(const FString& EventName, const FString& Properties, const FString& AppId = "");

    // UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void TrackFirst(const FString& EventName, TSharedPtr<FJsonObject> Properties, const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void TrackFirstWithId(const FString& EventName, const FString& Properties, const FString& FirstCheckId, const FString& AppId = "");

    // UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void TrackFirstWithId(const FString& EventName, TSharedPtr<FJsonObject> Properties, const FString& FirstCheckId, const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void TrackUpdate(const FString& EventName, const FString& Properties, const FString& EventId, const FString& AppId = "");

    // UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void TrackUpdate(const FString& EventName, TSharedPtr<FJsonObject> Properties, const FString& EventId, const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void TrackOverwrite(const FString& EventName, const FString& Properties, const FString& EventId, const FString& AppId = "");

    // UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void TrackOverwrite(const FString& EventName, TSharedPtr<FJsonObject> Properties, const FString& EventId, const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void TimeEvent(const FString& EventName, const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void UserSet(const FString& Properties, const FString& AppId = "");

    // UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void UserSet(TSharedPtr<FJsonObject> Properties, const FString& AppId = "");
    
    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void UserSetOnce(const FString& Properties, const FString& AppId = "");

    // UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void UserSetOnce(TSharedPtr<FJsonObject> Properties, const FString& AppId = "");
    
    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void UserAdd(const FString& Property, const float Value, const FString& AppId = "");
    
    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void UserUnset(const FString& Property, const FString& AppId = "");
    
    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void UserAppend(const FString& Properties, const FString& AppId = "");

    // UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void UserAppend(TSharedPtr<FJsonObject> Properties, const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void UserUniqueAppend(const FString& Properties, const FString& AppId = "");

    // UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void UserUniqueAppend(TSharedPtr<FJsonObject> Properties, const FString& AppId = "");
    
    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void UserDelete(const FString& AppId = "");
    
    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void SetDistinctId(const FString& DistinctId, const FString& AppId = "");
    
    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void Login(const FString& AccountId, const FString& AppId = "");
    
    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void Logout(const FString& AppId = "");
    
    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void Flush(const FString& AppId = "");
    
    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void OptOutTracking(const FString& AppId = "");
    
    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void OptInTracking(const FString& AppId = "");
    
    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void EnableTracking(bool bIsEnable, const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static FString GetDeviceId(const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static FString GetDistinctId(const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void SetSuperProperties(const FString& properties, const FString& AppId = "");

    // UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void SetSuperProperties(TSharedPtr<FJsonObject> Properties, const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void SetTrackStatus(const FString& Status = "NORMAL", const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static FString GetSuperProperties(const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static FString GetPresetProperties(const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static FString CreateLightInstance(const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void EnableThirdPartySharing(const TArray<FString>& EventTypeList, const FString& AppId = "");

    // UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static void EnableThirdPartySharingWithCustomProperties(const TArray<FString>& EventTypeList, TSharedPtr<FJsonObject> Properties, const FString& AppId = "");

    template <class UserClass>
    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static inline void SetDynamicSuperProperties(UserClass* TarObj, typename TMemFunPtrType<false, UserClass, FString()>::Type InMethod, const FString& AppId = "")
    {
        TADynamicSuperPropRetValDelegate Del;
        Del.BindUObject(TarObj, InMethod);
        SetDynamicProperties(Del, AppId);
    }

    template <class UserClass>
    UFUNCTION(BlueprintCallable, Category = "TDAnalytics")
    static inline void SetAutoTrackEventListener(UserClass* TarObj, typename TMemFunPtrType<false, UserClass, FString(FString, FString)>::Type InMethod, const TArray<FString>& EventTypeList, const FString& AppId = "")
    {
        TAAutoTrackEventRetValDelegate Del;
        Del.BindUObject(TarObj, InMethod);
        TASetAutoTrackEventListener(Del, EventTypeList, AppId);
    }
};
