// Copyright 2021 ThinkingData. All Rights Reserved.
#pragma once

#include "TASaveConfig.h"
#include "../Common/TAUtils.h"
#include "../Common/TALog.h"
#include "../Common/TAConstants.h"
#include "RequestHelper.h"
#include "EventManager.h"
#include "TDAnalyticsSettings.h"

#include "Policies/CondensedJsonPrintPolicy.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Kismet/GameplayStatics.h"

#include "TDAnalyticsPC.generated.h"


static TMap<FString, UTDAnalyticsPC*> TDAnalyticsSingletons;

static FString DefaultAppID;

UCLASS()
class UTDAnalyticsPC : public UObject
{

GENERATED_UCLASS_BODY()

public:

	static UTDAnalyticsPC* GetInstance(const FString& AppID);
	
	static void Initialize(const UTDAnalyticsSettings *DefaultSettings, FString Version);

	static void Initialize(const FString& AppID, const FString& ServerUrl, TAMode Mode, bool EnableLog, const FString& TimeZone, FString Version);

	static void SetEnableLog(bool Enable);

	FString InstanceAppID;

	FString ta_GetDeviceID();

	FString ta_GetDistinctID();

	FString ta_GetAccountID();

	FString ta_GetServerUrl();

	TAMode ta_GetMode();

	float ta_GetDefaultTimeZone();

	FString ta_GetSuperProperties();

	FString ta_GetTrackState();

	FString ta_GetPresetProperties();

	void ta_Logout();

	void ta_Flush();

	void UserDelete(); 

	void UserSet(const FString& Properties);

	void UserSetOnce(const FString& Properties);

	void UserAdd(const FString& Properties);

	void UserUnset(const FString& Property);

	void UserAppend(const FString& Properties);

	void UserUniqueAppend(const FString& Properties);

	void EnableTracking(bool EnableTrack);

	void ta_Login(const FString& AccountID);

	void ta_Identify(const FString& DistinctID);

	void ta_SetSuperProperties(const FString& properties);

	void ta_SetTrackState(const FString& State);

	void Track(const FString& EventName, const FString& Properties, const FString& DynamicProperties);

	void TrackFirst(const FString& EventName, const FString& Properties, const FString& DynamicProperties);

	void TrackFirstWithId(const FString& EventName, const FString& Properties, const FString& FirstCheckId, const FString& DynamicProperties);

	void TrackUpdate(const FString& EventName, const FString& Properties, const FString& EventId, const FString& DynamicProperties);

	void TrackOverwrite(const FString& EventName, const FString& Properties, const FString& EventId, const FString& DynamicProperties);

private:

	TAMode InstanceMode;

	UTASaveConfig* m_SaveConfig;

	UTAEventManager* m_EventManager;

	FString InstanceServerUrl;

	FString m_AccountID;

	FString m_DistinctID;

	FString m_LibVersion;

	FString m_SuperProperties;

	FString m_PresetProperties;

	FString m_TrackState;

	float m_TimeZone_Offset;

	~UTDAnalyticsPC();

	UTASaveConfig* ReadValue();

	void InitPresetProperties();

	void SaveValue(UTASaveConfig *SaveConfig);

	void Init(const FString& AppID, const FString& ServerUrl, TAMode Mode, const FString& TimeZone, FString Version);


};	
