// Copyright 2021 ThinkingData. All Rights Reserved.
#include "ThinkingAnalyticsPC.h"

class HandleAutoDeleteAsyncTask : public FNonAbandonableTask
{
    friend class FAutoDeleteAsyncTask<HandleAutoDeleteAsyncTask>;

    FString EventName;
   	FString Properties;
    FString DynamicProperties;
    FString EventType;
	FString AddProperties;
	UThinkingAnalyticsPC* m_Instance;

    HandleAutoDeleteAsyncTask(const FString& InEventName, const FString& InProperties, const FString& InDynamicProperties, const FString& InEventType, const FString& InAddProperties, UThinkingAnalyticsPC *Instance)
    	: EventName(InEventName), Properties(InProperties), DynamicProperties(InDynamicProperties), EventType(InEventType), AddProperties(InAddProperties), m_Instance(Instance)
    {
    }

   	HandleAutoDeleteAsyncTask(const FString& InEventType, const FString& InProperties, UThinkingAnalyticsPC *Instance)
        : Properties(InProperties), EventType(InEventType), m_Instance(Instance)
    {
    }

    ~HandleAutoDeleteAsyncTask()
    {
    }

    void DoWork()
    {
        // ... do the work here
        if ( EventName == "" )
    	{
            m_Instance->UserOperations(EventType, Properties);
        }
	    else
	    {
            m_Instance->ObtainEventInfoAndDoPost(EventName, Properties, DynamicProperties, EventType, AddProperties);
        }
    }

    FORCEINLINE TStatId GetStatId() const{
        RETURN_QUICK_DECLARE_CYCLE_STAT(HandleAutoDeleteAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
    }
};


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
	if ( AppID == "" )
    {
		return;
	}
	if ( ThinkingAnalyticsSingletons.Find(*AppID) == nullptr )
    {
		UThinkingAnalyticsPC* Instance = NewObject<UThinkingAnalyticsPC>();
		Instance->Init(AppID, ServerUrl, Mode, Version);
		Instance->AddToRoot();
		ThinkingAnalyticsSingletons.Emplace(*AppID, Instance);

		Instance->m_SaveGame = Instance->ReadValue();
		Instance->m_DistinctID = Instance->m_SaveGame->m_DistinctID;
		Instance->m_AccountID = Instance->m_SaveGame->m_AccountID;
		Instance->m_SuperProperties = Instance->m_SaveGame->m_SuperProperties;
		Instance->m_SaveGame->AddToRoot();
		Instance->InitPresetProperties();
		Instance->m_EnableTrack = true;

		FTALog::Warning(CUR_LOG_POSITION, TEXT("UThinkingAnalyticsPC Initialize Success !"));

	}
    else
    {
		FTALog::Warning(CUR_LOG_POSITION, TEXT("Do not repeat initialization !"));
	}
}

void UThinkingAnalyticsPC::Init(const FString& AppID, const FString& ServerUrl, TAMode Mode, FString Version)
{
	this->InstanceAppID = AppID;
	if ( DefaultAppID == "" )
    {
		DefaultAppID = *AppID;
		FTALog::Warning(CUR_LOG_POSITION, TEXT("DefaultAppID : ") + DefaultAppID);
	}

	this->InstanceServerUrl = ServerUrl;
	this->InstanceMode = Mode;
	this->m_LibVersion = Version;
}

void UThinkingAnalyticsPC::Track(const FString& EventName, const FString& Properties, const FString& DynamicProperties)
{
	FTALog::Warning(CUR_LOG_POSITION, TEXT("Track param: ") + this->InstanceAppID + TEXT(". ") + this->InstanceServerUrl);
	(new FAutoDeleteAsyncTask<HandleAutoDeleteAsyncTask>(EventName, Properties, DynamicProperties, FString(FTAConstants::EVENTTYPE_TRACK), TEXT(""), this))->StartBackgroundTask();
}

void UThinkingAnalyticsPC::ObtainEventInfoAndDoPost(const FString& EventName, const FString& Properties, const FString& DynamicProperties, const FString& EventType, const FString& AddProperties)
{
	TSharedPtr<FJsonObject> m_PropertiesJsonObject = MakeShareable(new FJsonObject);
	TSharedPtr<FJsonObject> m_DataJsonObject = MakeShareable(new FJsonObject);

	if ( FTAUtils::IsInvalidName(EventName) )
    {
		FTALog::Warning(CUR_LOG_POSITION, TEXT("event name[ ") + EventName + TEXT(" ] is not valid !"));
	}

    TSharedPtr<FJsonObject> PresetPropertiesJsonObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> PresetPropertiesReader = TJsonReaderFactory<>::Create(ta_GetPresetProperties());
	FJsonSerializer::Deserialize(PresetPropertiesReader, PresetPropertiesJsonObject);
	for (auto& Elem : PresetPropertiesJsonObject->Values)
    {
		m_PropertiesJsonObject->SetField(Elem.Key, Elem.Value);
	}

	TSharedPtr<FJsonObject> SuperPropertiesJsonObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> SuperPropertiesReader = TJsonReaderFactory<>::Create(ta_GetSuperProperties());
	FJsonSerializer::Deserialize(SuperPropertiesReader, SuperPropertiesJsonObject);

	TSharedPtr<FJsonObject> DynamicPropertiesJsonObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> DynamicPropertiesReader = TJsonReaderFactory<>::Create(DynamicProperties);
	FJsonSerializer::Deserialize(DynamicPropertiesReader, DynamicPropertiesJsonObject);

	TSharedPtr<FJsonObject> PropertiesJsonObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> PropertiesReader = TJsonReaderFactory<>::Create(Properties);
	FJsonSerializer::Deserialize(PropertiesReader, PropertiesJsonObject);

	for (auto& Elem : SuperPropertiesJsonObject->Values)
    {
		m_PropertiesJsonObject->SetField(Elem.Key, Elem.Value);
	}

	for (auto& Elem : DynamicPropertiesJsonObject->Values)
    {
		m_PropertiesJsonObject->SetField(Elem.Key, Elem.Value);
	}

	for (auto& Elem : PropertiesJsonObject->Values)
    {
		m_PropertiesJsonObject->SetField(Elem.Key, Elem.Value);
	}

	if ( (EventType == FTAConstants::EVENTTYPE_TRACK_FIRST) || (EventType == FTAConstants::EVENTTYPE_TRACK_UPDATE) || (EventType == FTAConstants::EVENTTYPE_TRACK_OVERWRITE) )
    {
		TSharedPtr<FJsonObject> AddPropertiesJsonObject = MakeShareable(new FJsonObject);
		TSharedRef<TJsonReader<>> AddPropertiesReader = TJsonReaderFactory<>::Create(AddProperties);
		FJsonSerializer::Deserialize(AddPropertiesReader, AddPropertiesJsonObject);
		for (auto& Elem : AddPropertiesJsonObject->Values)
    	{
			m_DataJsonObject->SetField(Elem.Key, Elem.Value);
		}
	}

	if ( (EventType == FTAConstants::EVENTTYPE_TRACK_UPDATE) || (EventType == FTAConstants::EVENTTYPE_TRACK_OVERWRITE) )
    {
		m_DataJsonObject->SetStringField(FTAConstants::KEY_TYPE, EventType);
	}
    else
    {
		m_DataJsonObject->SetStringField(FTAConstants::KEY_TYPE, FTAConstants::EVENTTYPE_TRACK);
	}

	m_DataJsonObject->SetStringField(FTAConstants::KEY_EVENT_NAME, EventName);
	m_DataJsonObject->SetStringField(FTAConstants::KEY_TIME, FTAUtils::FormatTime(FDateTime::Now()));
	m_DataJsonObject->SetStringField(FTAConstants::KEY_DISTINCT_ID, this->m_DistinctID);
	m_DataJsonObject->SetStringField(FTAConstants::KEY_DATA_ID, FTAUtils::GetGuid());

	if ( this->m_AccountID != "" )
    {
		m_DataJsonObject->SetStringField(FTAConstants::KEY_ACCOUNT_ID, this->m_AccountID);
	}
	m_DataJsonObject->SetObjectField(FTAConstants::KEY_PROPERTIES, m_PropertiesJsonObject);
	DoPost(m_DataJsonObject);
}

void UThinkingAnalyticsPC::TrackFirst(const FString& EventName, const FString& Properties, const FString& DynamicProperties)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField(FTAConstants::KEY_FIRST_CHECK_ID, ta_GetDeviceID());

	FString JsonStr;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonStr);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	(new FAutoDeleteAsyncTask<HandleAutoDeleteAsyncTask>(EventName, Properties, DynamicProperties, FString(FTAConstants::EVENTTYPE_TRACK_FIRST), JsonStr, this))->StartBackgroundTask();
}

void UThinkingAnalyticsPC::TrackFirstWithId(const FString& EventName, const FString& Properties, const FString& FirstCheckId, const FString& DynamicProperties)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField(FTAConstants::KEY_FIRST_CHECK_ID, FirstCheckId);

	FString JsonStr;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonStr);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	(new FAutoDeleteAsyncTask<HandleAutoDeleteAsyncTask>(EventName, Properties, DynamicProperties, FString(FTAConstants::EVENTTYPE_TRACK_FIRST), JsonStr, this))->StartBackgroundTask();
}

void UThinkingAnalyticsPC::TrackUpdate(const FString& EventName, const FString& Properties, const FString& EventId, const FString& DynamicProperties)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField(FTAConstants::KEY_EVENT_ID, EventId);

	FString JsonStr;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonStr);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	(new FAutoDeleteAsyncTask<HandleAutoDeleteAsyncTask>(EventName, Properties, DynamicProperties, FString(FTAConstants::EVENTTYPE_TRACK_UPDATE), JsonStr, this))->StartBackgroundTask();
}

void UThinkingAnalyticsPC::TrackOverwrite(const FString& EventName, const FString& Properties, const FString& EventId, const FString& DynamicProperties)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField(FTAConstants::KEY_EVENT_ID, EventId);

	FString JsonStr;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonStr);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	(new FAutoDeleteAsyncTask<HandleAutoDeleteAsyncTask>(EventName, Properties, DynamicProperties, FString(FTAConstants::EVENTTYPE_TRACK_OVERWRITE), JsonStr, this))->StartBackgroundTask();
}

void UThinkingAnalyticsPC::UserSet(const FString& Properties)
{
	(new FAutoDeleteAsyncTask<HandleAutoDeleteAsyncTask>(FString(FTAConstants::EVENTTYPE_USER_SET), Properties, this))->StartBackgroundTask();
}

void UThinkingAnalyticsPC::UserSetOnce(const FString& Properties)
{
	(new FAutoDeleteAsyncTask<HandleAutoDeleteAsyncTask>(FString(FTAConstants::EVENTTYPE_USER_SET_ONCE), Properties, this))->StartBackgroundTask();
}

void UThinkingAnalyticsPC::UserAdd(const FString& Properties)
{
	(new FAutoDeleteAsyncTask<HandleAutoDeleteAsyncTask>(FString(FTAConstants::EVENTTYPE_USER_ADD), Properties, this))->StartBackgroundTask();
}

void UThinkingAnalyticsPC::UserUnset(const FString& Property)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetNumberField(Property, 0);
	FString JsonStr;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonStr);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	(new FAutoDeleteAsyncTask<HandleAutoDeleteAsyncTask>(FString(FTAConstants::EVENTTYPE_USER_UNSET), JsonStr, this))->StartBackgroundTask();
}

void UThinkingAnalyticsPC::UserAppend(const FString& Properties)
{
	(new FAutoDeleteAsyncTask<HandleAutoDeleteAsyncTask>(FString(FTAConstants::EVENTTYPE_USER_APPEND), Properties, this))->StartBackgroundTask();
}

void UThinkingAnalyticsPC::UserUniqueAppend(const FString& Properties)
{
	(new FAutoDeleteAsyncTask<HandleAutoDeleteAsyncTask>(FString(FTAConstants::EVENTTYPE_USER_UNIQUE_APPEND), Properties, this))->StartBackgroundTask();
}

void UThinkingAnalyticsPC::UserDelete()
{
	(new FAutoDeleteAsyncTask<HandleAutoDeleteAsyncTask>(FString(FTAConstants::EVENTTYPE_USER_DEL), TEXT(""), this))->StartBackgroundTask();
}

void UThinkingAnalyticsPC::UserOperations(const FString& EventType, const FString& Properties)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Properties);
	FJsonSerializer::Deserialize(Reader, JsonObject);

	TSharedPtr<FJsonObject> m_DataJsonObject = MakeShareable(new FJsonObject);

	m_DataJsonObject->SetStringField(FTAConstants::KEY_TYPE, EventType);
	m_DataJsonObject->SetStringField(FTAConstants::KEY_TIME, FTAUtils::FormatTime(FDateTime::Now()));
	m_DataJsonObject->SetStringField(FTAConstants::KEY_DISTINCT_ID, this->m_DistinctID);
	m_DataJsonObject->SetStringField(FTAConstants::KEY_DATA_ID, FTAUtils::GetGuid());
	m_DataJsonObject->SetStringField(FTAConstants::KEY_DISTINCT_ID, this->m_DistinctID);

	if ( this->m_AccountID != "" )
    {
		m_DataJsonObject->SetStringField(FTAConstants::KEY_ACCOUNT_ID, this->m_AccountID);
	}

	m_DataJsonObject->SetObjectField(FTAConstants::KEY_PROPERTIES, JsonObject);
	DoPost(m_DataJsonObject);
}

void UThinkingAnalyticsPC::DoPost(TSharedPtr<FJsonObject> InDataObject)
{
	if ( !(this->m_EnableTrack) )
    {
    	FTALog::Warning(CUR_LOG_POSITION, TEXT("Instance is Disable !"));
		return;
	}
	FString ServerData;
	FString ServerUrl;
	FString m_DataStr;
	FRequestHelper* Helper = new FRequestHelper();

	if ( this->InstanceMode == TAMode::NORMAL )
    {
		ServerUrl = this->InstanceServerUrl;
		int32 SyncPoint = ServerUrl.Find(TEXT("sync"), ESearchCase::CaseSensitive, ESearchDir::FromEnd);
		if ( SyncPoint != -1 )
    	{
			ServerUrl = ServerUrl.Left(SyncPoint);
		}
		ServerUrl += "/sync";
		TSharedPtr<FJsonObject> DataJsonObject = MakeShareable(new FJsonObject);

		TArray< TSharedPtr<FJsonValue> > DataArray;
		TSharedPtr<FJsonValueObject> DataValue = MakeShareable(new FJsonValueObject(InDataObject));
		DataArray.Add(DataValue);

		DataJsonObject->SetArrayField(FTAConstants::KEY_DATA, DataArray);
		DataJsonObject->SetStringField(FTAConstants::KEY_APP_ID, this->InstanceAppID);
		DataJsonObject->SetStringField(FTAConstants::KEY_FLUSH_TIME, FTAUtils::GetCurrentTimeStamp());

		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ServerData);
		FJsonSerializer::Serialize(DataJsonObject.ToSharedRef(), Writer);

		Helper->CallHttpRequest(ServerUrl, ServerData, false);
	}
    else
    {
		ServerUrl = this->InstanceServerUrl;
		int32 SyncPoint = ServerUrl.Find(TEXT("sync"), ESearchCase::CaseSensitive, ESearchDir::FromEnd);

		if ( SyncPoint != -1 )
    	{
			ServerUrl = ServerUrl.Left(SyncPoint);
		}

		ServerUrl += "/sync_data";
		ServerData += "appid=";
		ServerData += this->InstanceAppID;
		ServerData += "&deviceId=";
		ServerData += ta_GetDeviceID();
		ServerData += "&source=client&data=";
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&m_DataStr);
		FJsonSerializer::Serialize(InDataObject.ToSharedRef(), Writer);

		FTALog::Warning(CUR_LOG_POSITION, TEXT("m_DataStr : ") + m_DataStr);
		ServerData += FGenericPlatformHttp::UrlEncode(m_DataStr);
		Helper->CallHttpRequest(ServerUrl, ServerData, true);
	}
}


void UThinkingAnalyticsPC::ta_Login(const FString& AccountID)
{
	this->m_AccountID = AccountID;
	this->m_SaveGame->SetAccountID(this->m_AccountID);
	SaveValue(this->m_SaveGame);
}

void UThinkingAnalyticsPC::ta_Logout()
{
	this->m_AccountID = "";
	this->m_SaveGame->SetAccountID("");
	SaveValue(this->m_SaveGame);
	FTALog::Warning(CUR_LOG_POSITION, TEXT("Logout Account !"));
}

void UThinkingAnalyticsPC::ta_Identify(const FString& DistinctID)
{
	this->m_DistinctID = DistinctID;
	this->m_SaveGame->SetDistinctID(this->m_DistinctID);
	SaveValue(this->m_SaveGame);
}

UThinkingAnalyticsPC* UThinkingAnalyticsPC::GetInstance(const FString& AppID)
{
	if ( &ThinkingAnalyticsSingletons == nullptr )
    {	
		FTALog::Warning(CUR_LOG_POSITION, TEXT("ThinkingAnalyticsSingletons is null !"));
	}
    else
    {
		UThinkingAnalyticsPC** Instance = ThinkingAnalyticsSingletons.Find(*AppID);
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

void UThinkingAnalyticsPC::SaveValue(UTASaveGame *SaveGame)
{
		if( UTASaveGame* SaveGameInstance = Cast<UTASaveGame>(UGameplayStatics::CreateSaveGameObject(UTASaveGame::StaticClass())) )
	    {
			FTALog::Warning(CUR_LOG_POSITION, TEXT("CreateSaveGameObject Success !"));
			SaveGameInstance->AddAll(SaveGame);
			UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->m_AppID, FTAConstants::USER_INDEX);
		}
	    else
	    {
			FTALog::Warning(CUR_LOG_POSITION, TEXT("CreateSaveGameObject Failed !"));
		}
}

UTASaveGame* UThinkingAnalyticsPC::ReadValue()
{
	UTASaveGame* SaveGame = Cast<UTASaveGame>(UGameplayStatics::LoadGameFromSlot(this->InstanceAppID, 0));
	if( !SaveGame )
    {
		FTALog::Warning(CUR_LOG_POSITION, TEXT("LoadGameFromSlot Failed , NewObject !"));
		SaveGame = NewObject<UTASaveGame>();
		SaveGame->SetAppID(this->InstanceAppID);
		this->m_DistinctID = ta_GetDeviceID();
		SaveGame->SetDistinctID(this->m_DistinctID);
		SaveValue(SaveGame);
	}

	return SaveGame;
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
	this->m_SuperProperties = properties;
	this->m_SaveGame->SetSuperProperties(this->m_SuperProperties);
	SaveValue(this->m_SaveGame);
}

FString UThinkingAnalyticsPC::ta_GetSuperProperties()
{
	return this->m_SuperProperties;
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
	m_DataJsonObject->SetStringField(FTAConstants::KEY_LIB_VERSION, this->m_LibVersion);
	m_DataJsonObject->SetNumberField(FTAConstants::KEY_SCREEN_WIDTH, FTAUtils::GetScreenWidth());
	m_DataJsonObject->SetNumberField(FTAConstants::KEY_SCREEN_HEIGHT, FTAUtils::GetScreenHeight());
	m_DataJsonObject->SetStringField(FTAConstants::KEY_OS, FTAUtils::GetOS());
	m_DataJsonObject->SetStringField(FTAConstants::KEY_OS_VERSION, FTAUtils::GetOSVersion());
	m_DataJsonObject->SetStringField(FTAConstants::KEY_APP_VERSION, FTAUtils::GetProjectVersion());
	m_DataJsonObject->SetStringField(FTAConstants::KEY_DEVICE_ID, ta_GetDeviceID());
	m_DataJsonObject->SetNumberField(FTAConstants::KEY_ZONE_OFFSET, FTAUtils::GetZoneOffset());
	m_DataJsonObject->SetStringField(FTAConstants::KEY_SYSTEM_LANGUAGE, FTAUtils::GetSystemLanguage());
	m_DataJsonObject->SetStringField(FTAConstants::KEY_INSTALL_TIME, FTAUtils::GetProjectFileCreateTime());
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&m_PresetProperties);
	FJsonSerializer::Serialize(m_DataJsonObject.ToSharedRef(), Writer);
}

void UThinkingAnalyticsPC::EnableTracking(bool EnableTrack)
{
	this->m_EnableTrack = EnableTrack;
}

void UThinkingAnalyticsPC::SetEnableLog(bool Enable)
{
	FTALog::SetEnableLog(Enable);
}







