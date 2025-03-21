// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TDAnalyticsSettings.h"
#include "TDAnalyticsWindow.generated.h"

class TDWindowsSetting;

UCLASS()
class UTDAnalyticsWindow : public UObject
{
GENERATED_UCLASS_BODY()
public:
	static void Initialize(const UTDAnalyticsSettings* DefaultSettings, FString Version);
	static void Initialize(const FString& AppID, const FString& ServerUrl, TAMode Mode, bool EnableLog, const FString& TimeZone, FString Version);
	static void SetDistinctId(const FString& DistinctID);
	static FString GetDistinctId();
	static void Login(const FString& AccountID);
	static void Logout();
	static void Track(const FString& EventName, const FString& Properties);
	static void TimeEvent(const FString& EventName);
	static void UserSet(const FString& Properties);
	static void UserSetOnce(const FString& Properties);
	static void UserAdd(const FString& Properties);
	static void UserUnset(const FString& Property);
	static void UserDelete();
	static void UserAppend(const FString& Properties);
	static void UserUniqueAppend(const FString& Properties);
	static FString GetDeviceID();
	static void Flush();
	static void CalibrateTime(int64 timestamp);
	static void SetSuperProperties(const FString& Properties);
	static void ClearSuperProperties();
	static void UnsetSuperProperty(const FString& Property);
	static FString GetSuperProperties();
	static void TrackFirst(const FString& EventName, const FString& Properties, const FString& FirstCheckId);
	static void TrackUpdate(const FString& EventName, const FString& Properties, const FString& EventId);
	static void TrackOverwrite(const FString& EventName, const FString& Properties, const FString& EventId);
	static FString GetPresetProperties();
	static void InitWithConfig(TDWindowsSetting Setting);
private:
	~UTDAnalyticsWindow();
	//static void InitWithConfig(const FString& AppID, const FString& ServerUrl, TAMode Mode, bool EnableLog, const FString& TimeZone, bool EnableEncrypt, int EncryptVersion, const FString& EncryptPublicKey);
};

class TDWindowsSetting {
public:
	FString AppId;
	FString ServerUrl;
	TAMode Mode;
	bool EnableLog;
	FString Version;
	bool EnableEncrypt;
	int EncryptVersion;
	FString EncryptPublicKey;
};
