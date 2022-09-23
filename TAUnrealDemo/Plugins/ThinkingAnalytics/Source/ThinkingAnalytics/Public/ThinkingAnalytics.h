// Copyright 2020 Thinking Data Inc. All Rights Reserved.

#pragma once

#include <string>
#include <map>
#include <vector>

#include "UObject/Object.h"
#include "ThinkingAnalyticsSettings.h"
#include "ThinkingAnalytics.generated.h"


typedef FString(*GetDynamicSuperProperties)();

DECLARE_DELEGATE_RetVal(FString, TADynamicSuperPropRetValDelegate);
DECLARE_DELEGATE_RetVal_TwoParams(FString, TAAutoTrackEventRetValDelegate, FString, FString);

static TMap<FString, TADynamicSuperPropRetValDelegate> TADynamicSuperPropMethods;

UCLASS()
class THINKINGANALYTICS_API UThinkingAnalytics : public UObject
{
    GENERATED_UCLASS_BODY()
    
private:
    
    static FString GetDynamicProperties(const FString& AppId = "");

    static void SetDynamicProperties(TADynamicSuperPropRetValDelegate Del, const FString& AppId = "");

    static void TASetAutoTrackEventListener(TAAutoTrackEventRetValDelegate Del, const TArray<FString>& EventTypeList, const FString& AppId = "");


public:
    
    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void Initialize();
    
    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void InitializeInstance(const FString& appid, const FString& serverurl, TAMode mode, bool bEnableLog, const FString& timeZone);

    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void InitializeEncryptInstance(const FString& appid, const FString& serverurl, TAMode mode, bool bEnableLog, const FString& timeZone, bool bEnableEncrypt, const FString& EncryptPublicKey, int EncryptVersion, const FString& SymmetricEncryption, const FString& AsymmetricEncryption);
    
    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void CalibrateTime(int64 timestamp);

    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void EnableAutoTrack(const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void EnableAutoTrackWithType(const TArray<FString>& EventTypeList, const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void EnableAutoTrackWithTypeAndProperties(const TArray<FString>& EventTypeList, const FString& Properties, const FString& AppId = "");

    // UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void EnableAutoTrackWithTypeAndProperties(const TArray<FString>& EventTypeList, TSharedPtr<FJsonObject> Properties, const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void CalibrateTimeWithNtp(const FString& urlString);
    
    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void Track(const FString& EventName, const FString& Properties, const FString& AppId = "");

    // UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void Track(const FString& EventName, TSharedPtr<FJsonObject> Properties, const FString& AppId = "");
    
    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void TrackFirst(const FString& EventName, const FString& Properties, const FString& AppId = "");

    // UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void TrackFirst(const FString& EventName, TSharedPtr<FJsonObject> Properties, const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void TrackFirstWithId(const FString& EventName, const FString& Properties, const FString& FirstCheckId, const FString& AppId = "");

    // UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void TrackFirstWithId(const FString& EventName, TSharedPtr<FJsonObject> Properties, const FString& FirstCheckId, const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void TrackUpdate(const FString& EventName, const FString& Properties, const FString& EventId, const FString& AppId = "");

    // UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void TrackUpdate(const FString& EventName, TSharedPtr<FJsonObject> Properties, const FString& EventId, const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void TrackOverwrite(const FString& EventName, const FString& Properties, const FString& EventId, const FString& AppId = "");

    // UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void TrackOverwrite(const FString& EventName, TSharedPtr<FJsonObject> Properties, const FString& EventId, const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void TimeEvent(const FString& EventName, const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void UserSet(const FString& Properties, const FString& AppId = "");

    // UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void UserSet(TSharedPtr<FJsonObject> Properties, const FString& AppId = "");
    
    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void UserSetOnce(const FString& Properties, const FString& AppId = "");

    // UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void UserSetOnce(TSharedPtr<FJsonObject> Properties, const FString& AppId = "");
    
    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void UserAdd(const FString& Property, const float Value, const FString& AppId = "");
    
    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void UserUnset(const FString& Property, const FString& AppId = "");
    
    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void UserAppend(const FString& Properties, const FString& AppId = "");

    // UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void UserAppend(TSharedPtr<FJsonObject> Properties, const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void UserUniqueAppend(const FString& Properties, const FString& AppId = "");

    // UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void UserUniqueAppend(TSharedPtr<FJsonObject> Properties, const FString& AppId = "");
    
    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void UserDelete(const FString& AppId = "");
    
    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void Identify(const FString& DistinctId, const FString& AppId = "");
    
    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void Login(const FString& AccountId, const FString& AppId = "");
    
    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void Logout(const FString& AppId = "");
    
    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void Flush(const FString& AppId = "");
    
    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void OptOutTracking(const FString& AppId = "");
    
    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void OptInTracking(const FString& AppId = "");
    
    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void EnableTracking(bool bIsEnable, const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static FString GetDeviceId(const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static FString GetDistinctId(const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void SetSuperProperties(const FString& properties, const FString& AppId = "");

    // UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void SetSuperProperties(TSharedPtr<FJsonObject> Properties, const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void SetTrackStatus(const FString& Status = "NORMAL", const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static FString GetSuperProperties(const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static FString GetPresetProperties(const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static FString CreateLightInstance(const FString& AppId = "");

    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void EnableThirdPartySharing(const TArray<FString>& EventTypeList, const FString& AppId = "");

    // UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static void EnableThirdPartySharingWithCustomProperties(const TArray<FString>& EventTypeList, TSharedPtr<FJsonObject> Properties, const FString& AppId = "");

    template <class UserClass>
    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static inline void SetDynamicSuperProperties(UserClass* TarObj, typename TMemFunPtrType<false, UserClass, FString()>::Type InMethod, const FString& AppId = "")
    {
        TADynamicSuperPropRetValDelegate Del;
        Del.BindUObject(TarObj, InMethod);
        SetDynamicProperties(Del, AppId);
    }

    template <class UserClass>
    UFUNCTION(BlueprintCallable, Category = "ThinkingAnalytics")
    static inline void SetAutoTrackEventListener(UserClass* TarObj, typename TMemFunPtrType<false, UserClass, FString(FString, FString)>::Type InMethod, const TArray<FString>& EventTypeList, const FString& AppId = "")
    {
        TAAutoTrackEventRetValDelegate Del;
        Del.BindUObject(TarObj, InMethod);
        TASetAutoTrackEventListener(Del, EventTypeList, AppId);
    }
};
