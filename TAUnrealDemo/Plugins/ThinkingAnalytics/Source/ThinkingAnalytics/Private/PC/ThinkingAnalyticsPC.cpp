// Copyright 2021 ThinkingData. All Rights Reserved.
#include "ThinkingAnalyticsPC.h"

UThinkingAnalyticsPC::UThinkingAnalyticsPC(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

UThinkingAnalyticsPC::~UThinkingAnalyticsPC()
{
}

void UThinkingAnalyticsPC::Initialize(const UThinkingAnalyticsSettings *DefaultSettings, FString Version)
{
	Initialize(DefaultSettings->AppID, DefaultSettings->ServerUrl, DefaultSettings->Mode, DefaultSettings->bEnableLog, DefaultSettings->TimeZone, Version);
}

void UThinkingAnalyticsPC::Initialize(const FString& AppID, const FString& ServerUrl, TAMode Mode, bool EnableLog, const FString& TimeZone, FString Version)
{
	SetEnableLog(EnableLog);
	if ( AppID.IsEmpty() )
    {
		return;
	}
	if ( ThinkingAnalyticsSingletons.Find(AppID) == nullptr )
    {
		UThinkingAnalyticsPC* Instance = NewObject<UThinkingAnalyticsPC>();
		Instance->Init(AppID, ServerUrl, Mode, TimeZone, Version);
		Instance->AddToRoot();
		ThinkingAnalyticsSingletons.Emplace(AppID, Instance);

		Instance->m_SaveConfig = Instance->ReadValue();
		Instance->m_DistinctID = Instance->m_SaveConfig->m_DistinctID;
		Instance->m_AccountID = Instance->m_SaveConfig->m_AccountID;
		Instance->m_TrackState = Instance->m_SaveConfig->m_TrackState;
		Instance->m_SuperProperties = Instance->m_SaveConfig->m_SuperProperties;
		Instance->m_SaveConfig->AddToRoot();
		Instance->InitPresetProperties();
		Instance->m_EventManager = NewObject<UTAEventManager>();
		Instance->m_EventManager->BindInstance(Instance);
		Instance->m_EventManager->AddToRoot();
		FTALog::Warning(CUR_LOG_POSITION, TEXT("UThinkingAnalyticsPC Initialize Success !"));
	}
    else
    {
		FTALog::Warning(CUR_LOG_POSITION, TEXT("Do not repeat initialization !"));
	}
}

void UThinkingAnalyticsPC::Init(const FString& AppID, const FString& ServerUrl, TAMode Mode, const FString& TimeZone, FString Version)
{
	m_TimeZone_Offset = FTAUtils::GetZoneOffsetWithTimeZone(TimeZone);
	this->InstanceAppID = AppID;
	if ( DefaultAppID.IsEmpty() )
    {
		DefaultAppID = AppID;
		FTALog::Warning(CUR_LOG_POSITION, TEXT("DefaultAppID : ") + DefaultAppID);
	}

	this->InstanceServerUrl = ServerUrl;
	this->InstanceMode = Mode;
	this->m_LibVersion = Version;
}

void UThinkingAnalyticsPC::Track(const FString& EventName, const FString& Properties, const FString& DynamicProperties)
{
	// FTALog::Warning(CUR_LOG_POSITION, TEXT("Track param: ") + this->InstanceAppID + TEXT(". ") + this->InstanceServerUrl);
	if ( this->m_TrackState.Equals(FTAConstants::TRACK_STATUS_STOP) || this->m_TrackState.Equals(FTAConstants::TRACK_STATUS_PAUSE) )
	{
		return;
	}
	this->m_EventManager->EnqueueTrackEvent(EventName, Properties, DynamicProperties, FString(FTAConstants::EVENTTYPE_TRACK), TEXT(""));
}

void UThinkingAnalyticsPC::TrackFirst(const FString& EventName, const FString& Properties, const FString& DynamicProperties)
{
	if ( this->m_TrackState.Equals(FTAConstants::TRACK_STATUS_STOP) || this->m_TrackState.Equals(FTAConstants::TRACK_STATUS_PAUSE) )
	{
		return;
	}
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField(FTAConstants::KEY_FIRST_CHECK_ID, ta_GetDeviceID());

	FString JsonStr;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonStr);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	this->m_EventManager->EnqueueTrackEvent(EventName, Properties, DynamicProperties, FString(FTAConstants::EVENTTYPE_TRACK_FIRST), JsonStr);
}

void UThinkingAnalyticsPC::TrackFirstWithId(const FString& EventName, const FString& Properties, const FString& FirstCheckId, const FString& DynamicProperties)
{
	if ( this->m_TrackState.Equals(FTAConstants::TRACK_STATUS_STOP) || this->m_TrackState.Equals(FTAConstants::TRACK_STATUS_PAUSE) )
	{
		return;
	}
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField(FTAConstants::KEY_FIRST_CHECK_ID, FirstCheckId);

	FString JsonStr;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonStr);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	this->m_EventManager->EnqueueTrackEvent(EventName, Properties, DynamicProperties, FString(FTAConstants::EVENTTYPE_TRACK_FIRST), JsonStr);
}

void UThinkingAnalyticsPC::TrackUpdate(const FString& EventName, const FString& Properties, const FString& EventId, const FString& DynamicProperties)
{
	if ( this->m_TrackState.Equals(FTAConstants::TRACK_STATUS_STOP) || this->m_TrackState.Equals(FTAConstants::TRACK_STATUS_PAUSE) )
	{
		return;
	}
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField(FTAConstants::KEY_EVENT_ID, EventId);

	FString JsonStr;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonStr);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	this->m_EventManager->EnqueueTrackEvent(EventName, Properties, DynamicProperties, FString(FTAConstants::EVENTTYPE_TRACK_UPDATE), JsonStr);
}

void UThinkingAnalyticsPC::TrackOverwrite(const FString& EventName, const FString& Properties, const FString& EventId, const FString& DynamicProperties)
{
	if ( this->m_TrackState.Equals(FTAConstants::TRACK_STATUS_STOP) || this->m_TrackState.Equals(FTAConstants::TRACK_STATUS_PAUSE) )
	{
		return;
	}
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField(FTAConstants::KEY_EVENT_ID, EventId);

	FString JsonStr;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonStr);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	this->m_EventManager->EnqueueTrackEvent(EventName, Properties, DynamicProperties, FString(FTAConstants::EVENTTYPE_TRACK_OVERWRITE), JsonStr);
}

void UThinkingAnalyticsPC::UserSet(const FString& Properties)
{
	if ( this->m_TrackState.Equals(FTAConstants::TRACK_STATUS_STOP) || this->m_TrackState.Equals(FTAConstants::TRACK_STATUS_PAUSE) )
	{
		return;
	}
	this->m_EventManager->EnqueueUserEvent(FString(FTAConstants::EVENTTYPE_USER_SET), Properties);
}

void UThinkingAnalyticsPC::UserSetOnce(const FString& Properties)
{
	if ( this->m_TrackState.Equals(FTAConstants::TRACK_STATUS_STOP) || this->m_TrackState.Equals(FTAConstants::TRACK_STATUS_PAUSE) )
	{
		return;
	}
	this->m_EventManager->EnqueueUserEvent(FString(FTAConstants::EVENTTYPE_USER_SET_ONCE), Properties);
}

void UThinkingAnalyticsPC::UserAdd(const FString& Properties)
{
	if ( this->m_TrackState.Equals(FTAConstants::TRACK_STATUS_STOP) || this->m_TrackState.Equals(FTAConstants::TRACK_STATUS_PAUSE) )
	{
		return;
	}
	this->m_EventManager->EnqueueUserEvent(FString(FTAConstants::EVENTTYPE_USER_ADD), Properties);
}

void UThinkingAnalyticsPC::UserUnset(const FString& Property)
{
	if ( this->m_TrackState.Equals(FTAConstants::TRACK_STATUS_STOP) || this->m_TrackState.Equals(FTAConstants::TRACK_STATUS_PAUSE) )
	{
		return;
	}
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetNumberField(Property, 0);
	FString JsonStr;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonStr);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	this->m_EventManager->EnqueueUserEvent(FString(FTAConstants::EVENTTYPE_USER_UNSET), JsonStr);
}

void UThinkingAnalyticsPC::UserAppend(const FString& Properties)
{
	if ( this->m_TrackState.Equals(FTAConstants::TRACK_STATUS_STOP) || this->m_TrackState.Equals(FTAConstants::TRACK_STATUS_PAUSE) )
	{
		return;
	}
	this->m_EventManager->EnqueueUserEvent(FString(FTAConstants::EVENTTYPE_USER_APPEND), Properties);
}

void UThinkingAnalyticsPC::UserUniqueAppend(const FString& Properties)
{
	if ( this->m_TrackState.Equals(FTAConstants::TRACK_STATUS_STOP) || this->m_TrackState.Equals(FTAConstants::TRACK_STATUS_PAUSE) )
	{
		return;
	}
	this->m_EventManager->EnqueueUserEvent(FString(FTAConstants::EVENTTYPE_USER_UNIQUE_APPEND), Properties);
}

void UThinkingAnalyticsPC::UserDelete()
{
	if ( this->m_TrackState.Equals(FTAConstants::TRACK_STATUS_STOP) || this->m_TrackState.Equals(FTAConstants::TRACK_STATUS_PAUSE) )
	{
		return;
	}
	this->m_EventManager->EnqueueUserEvent(FString(FTAConstants::EVENTTYPE_USER_DEL), TEXT(""));
}


void UThinkingAnalyticsPC::ta_Login(const FString& AccountID)
{
	this->m_AccountID = AccountID;
	this->m_SaveConfig->SetAccountID(this->m_AccountID);
	SaveValue(this->m_SaveConfig);
}

void UThinkingAnalyticsPC::ta_Logout()
{
	this->m_AccountID = "";
	this->m_SaveConfig->SetAccountID("");
	SaveValue(this->m_SaveConfig);
	FTALog::Warning(CUR_LOG_POSITION, TEXT("Logout Account !"));
}

void UThinkingAnalyticsPC::ta_Identify(const FString& DistinctID)
{
	this->m_DistinctID = DistinctID;
	this->m_SaveConfig->SetDistinctID(this->m_DistinctID);
	SaveValue(this->m_SaveConfig);
}

UThinkingAnalyticsPC* UThinkingAnalyticsPC::GetInstance(const FString& AppID)
{
	if ( &ThinkingAnalyticsSingletons == nullptr )
    {	
		FTALog::Warning(CUR_LOG_POSITION, TEXT("ThinkingAnalyticsSingletons is null !"));
	}
    else
    {
		UThinkingAnalyticsPC** Instance = ThinkingAnalyticsSingletons.Find(AppID);
		if ( Instance == nullptr )
    	{
			FTALog::Warning(CUR_LOG_POSITION, TEXT("Try Use DefaultInstance !"));
			UThinkingAnalyticsPC** DefaultInstance = ThinkingAnalyticsSingletons.Find(DefaultAppID);
			if ( DefaultInstance == nullptr )
    		{
				FTALog::Warning(CUR_LOG_POSITION, TEXT("No DefaultInstance !"));
			}
		    else
		    {
				return *DefaultInstance;
			}
		}
   	 	else
    	{
    		return *Instance;
		}
	}
	
	return nullptr;
}

void UThinkingAnalyticsPC::SaveValue(UTASaveConfig *SaveConfig)
{
	if( !SaveConfig )
    {
    	FTALog::Warning(CUR_LOG_POSITION, TEXT("SaveValue CreateSaveGameObject Success !"));
    	SaveConfig = Cast<UTASaveConfig>(UGameplayStatics::CreateSaveGameObject(UTASaveConfig::StaticClass()));
    }
	UGameplayStatics::SaveGameToSlot(SaveConfig, this->InstanceAppID, FTAConstants::USER_INDEX_CONFIG);
}

UTASaveConfig* UThinkingAnalyticsPC::ReadValue()
{
	UTASaveConfig* SaveConfig = Cast<UTASaveConfig>(UGameplayStatics::LoadGameFromSlot(this->InstanceAppID, FTAConstants::USER_INDEX_CONFIG));
	if( !SaveConfig )
    {
    	SaveConfig = Cast<UTASaveConfig>(UGameplayStatics::CreateSaveGameObject(UTASaveConfig::StaticClass()));
		this->m_DistinctID = ta_GetDeviceID();
		SaveConfig->SetDistinctID(this->m_DistinctID);
		UGameplayStatics::SaveGameToSlot(SaveConfig, this->InstanceAppID, FTAConstants::USER_INDEX_CONFIG);
    	FTALog::Warning(CUR_LOG_POSITION, TEXT("ReadValue CreateSaveGameObject Success !"));
	}
	FTALog::Warning(CUR_LOG_POSITION, TEXT("ReadValue Success !"));
	return SaveConfig;
}

FString UThinkingAnalyticsPC::ta_GetDistinctID()
{
	return this->m_DistinctID;
}

FString UThinkingAnalyticsPC::ta_GetDeviceID()
{
	return FTAUtils::GetMachineAccountId();
}

void UThinkingAnalyticsPC::ta_SetSuperProperties(const FString& properties)
{
	FString FinalProperties = FTAUtils::MergePropertiesWithOffset(properties, this->m_SuperProperties, m_TimeZone_Offset);
	this->m_SuperProperties = FinalProperties;
	this->m_SaveConfig->SetSuperProperties(this->m_SuperProperties);
	SaveValue(this->m_SaveConfig);
}

void UThinkingAnalyticsPC::ta_SetTrackState(const FString& State)
{
	if ( State.Equals(FTAConstants::TRACK_STATUS_PAUSE) )
	{
		if ( this->m_TrackState.Equals(FTAConstants::TRACK_STATUS_NORMAL) )
		{
			this->m_EventManager->Flush();
		}
	}
	else if ( State.Equals(FTAConstants::TRACK_STATUS_STOP) )
	{
		if ( this->m_TrackState.Equals(FTAConstants::TRACK_STATUS_NORMAL) )
		{
			this->m_EventManager->Flush();
		}
		this->m_AccountID = "";
		this->m_DistinctID = ta_GetDeviceID();
		this->m_SuperProperties = "";

		this->m_SaveConfig->SetAccountID(this->m_AccountID);
		this->m_SaveConfig->SetDistinctID(this->m_DistinctID);
		this->m_SaveConfig->SetSuperProperties(this->m_SuperProperties);
	}
	else if ( State.Equals(FTAConstants::TRACK_STATUS_SAVE_ONLY ))
	{

	}
	else
	{
		//normal
	}
	this->m_TrackState = State;
	this->m_SaveConfig->SetTrackState(this->m_TrackState);
	SaveValue(this->m_SaveConfig);

	FTALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("current state %s"), *this->m_TrackState));
}

FString UThinkingAnalyticsPC::ta_GetSuperProperties()
{
	return this->m_SuperProperties;
}

FString UThinkingAnalyticsPC::ta_GetTrackState()
{
	return this->m_TrackState;
}

FString UThinkingAnalyticsPC::ta_GetPresetProperties()
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> SuperPropertiesReader = TJsonReaderFactory<>::Create(this->m_PresetProperties);
	FJsonSerializer::Deserialize(SuperPropertiesReader, JsonObject);
	JsonObject->SetStringField(FTAConstants::KEY_RAM, FTAUtils::GetMemoryStats());
	JsonObject->SetStringField(FTAConstants::KEY_DISK, FTAUtils::GetDiskStats());
	JsonObject->SetStringField(FTAConstants::KEY_FPS, FTAUtils::GetAverageFps());

	FString JsonStr;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonStr);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	return JsonStr;
}

void UThinkingAnalyticsPC::InitPresetProperties()
{
	TSharedPtr<FJsonObject> m_DataJsonObject = MakeShareable(new FJsonObject);
	m_DataJsonObject->SetStringField(FTAConstants::KEY_LIB, TEXT("Unreal"));
	m_DataJsonObject->SetStringField(FTAConstants::KEY_LIB_VERSION, this->m_LibVersion);
	m_DataJsonObject->SetNumberField(FTAConstants::KEY_SCREEN_WIDTH, FTAUtils::GetScreenWidth());
	m_DataJsonObject->SetNumberField(FTAConstants::KEY_SCREEN_HEIGHT, FTAUtils::GetScreenHeight());
	m_DataJsonObject->SetStringField(FTAConstants::KEY_OS, FTAUtils::GetOS());
	m_DataJsonObject->SetStringField(FTAConstants::KEY_OS_VERSION, FTAUtils::GetOSVersion());
	m_DataJsonObject->SetStringField(FTAConstants::KEY_APP_VERSION, FTAUtils::GetProjectVersion());
	m_DataJsonObject->SetStringField(FTAConstants::KEY_DEVICE_ID, ta_GetDeviceID());
	m_DataJsonObject->SetNumberField(FTAConstants::KEY_ZONE_OFFSET, m_TimeZone_Offset);
	m_DataJsonObject->SetStringField(FTAConstants::KEY_SYSTEM_LANGUAGE, FTAUtils::GetSystemLanguage());
	m_DataJsonObject->SetStringField(FTAConstants::KEY_INSTALL_TIME, FTAUtils::GetProjectFileCreateTime(m_TimeZone_Offset));
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&m_PresetProperties);
	FJsonSerializer::Serialize(m_DataJsonObject.ToSharedRef(), Writer);
}

void UThinkingAnalyticsPC::EnableTracking(bool EnableTrack)
{
	if ( EnableTrack )
	{
		ta_SetTrackState(FTAConstants::TRACK_STATUS_NORMAL);
	}
	else
	{
		ta_SetTrackState(FTAConstants::TRACK_STATUS_SAVE_ONLY);
	}
}

void UThinkingAnalyticsPC::SetEnableLog(bool Enable)
{
	FTALog::SetEnableLog(Enable);
}

float UThinkingAnalyticsPC::ta_GetDefaultTimeZone()
{
	return this->m_TimeZone_Offset;
}

FString UThinkingAnalyticsPC::ta_GetAccountID()
{
	return this->m_AccountID;
}

FString UThinkingAnalyticsPC::ta_GetServerUrl()
{
	return this->InstanceServerUrl;
}

TAMode UThinkingAnalyticsPC::ta_GetMode()
{
	return this->InstanceMode;
}

void UThinkingAnalyticsPC::ta_Flush()
{
	if ( this->m_TrackState.Equals(FTAConstants::TRACK_STATUS_NORMAL) )
	{
		this->m_EventManager->Flush();
	}
	else
	{
		FTALog::Warning(CUR_LOG_POSITION, TEXT("disabled"));
	}
}






