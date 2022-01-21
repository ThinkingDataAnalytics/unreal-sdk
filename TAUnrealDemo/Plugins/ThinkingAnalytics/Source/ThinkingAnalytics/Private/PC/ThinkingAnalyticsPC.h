// Copyright 2021 ThinkingData. All Rights Reserved.
#pragma once

#include "TASaveGame.h"
#include "TAUtils.h"
#include "TALog.h"
#include "TAConstants.h"
#include "RequestHelper.h"
#include "ThinkingAnalyticsSettings.h"

#include "Policies/CondensedJsonPrintPolicy.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Kismet/GameplayStatics.h"

#include "ThinkingAnalyticsPC.generated.h"


static TMap<FString, UThinkingAnalyticsPC*> ThinkingAnalyticsSingletons;

static FString DefaultAppID;

UCLASS()
class UThinkingAnalyticsPC : public UObject
{

GENERATED_UCLASS_BODY()

void ObtainEventInfoAndDoPost(const FString& EventName, const FString& Properties, const FString& DynamicProperties, const FString& EventType, const FString& AddProperties, const TSharedPtr<FJsonObject> m_PresetPropertiesJsonObject);

public:

	static UThinkingAnalyticsPC* GetInstance(const FString& AppID);
	
	static void Initialize(const UThinkingAnalyticsSettings *DefaultSettings, FString Version);

	static void Initialize(const FString& AppID, const FString& ServerUrl, TAMode Mode, bool EnableLog, const FString& TimeZone, FString Version);

	static void SetEnableLog(bool Enable);

	FString InstanceAppID;

	FString ta_GetDeviceID();

	FString ta_GetDistinctID();

	FString ta_GetSuperProperties();

	FString ta_GetPresetProperties();

	void ta_Logout();

	void UserDelete(); 

	void UserSet(const FString& Properties);

	void UserSetOnce(const FString& Properties);

	void UserAdd(const FString& Properties);

	void UserUnset(const FString& Property);

	void UserAppend(const FString& Properties);

	void EnableTracking(bool EnableTrack);

	void ta_Login(const FString& AccountID);

	void ta_Identify(const FString& DistinctID);

	void ta_SetSuperProperties(const FString& properties);

	void UserOperations(const FString& EventType, const FString& Properties);

	void Track(const FString& EventName, const FString& Properties, const FString& DynamicProperties);

	void TrackFirst(const FString& EventName, const FString& Properties, const FString& DynamicProperties);

	void TrackFirstWithId(const FString& EventName, const FString& Properties, const FString& FirstCheckId, const FString& DynamicProperties);

	void TrackUpdate(const FString& EventName, const FString& Properties, const FString& EventId, const FString& DynamicProperties);

	void TrackOverwrite(const FString& EventName, const FString& Properties, const FString& EventId, const FString& DynamicProperties);

	friend class HandleAutoDeleteAsyncTask;
private:

	TAMode InstanceMode;

	UTASaveGame* m_SaveGame;

	FString InstanceServerUrl;

	FString m_AccountID;

	FString m_DistinctID;

	FString m_LibVersion;

	FString m_SuperProperties;
	
	bool m_EnableTrack;

	~UThinkingAnalyticsPC();

	UTASaveGame* ReadValue();

	void SaveValue(UTASaveGame *SaveGame);

	void DoPost(TSharedPtr<FJsonObject> DataObject);

	void Init(const FString& AppID, const FString& ServerUrl, TAMode Mode, FString Version);

	TSharedPtr<FJsonObject> GetPresetPropertiesJsonObject();

};	
