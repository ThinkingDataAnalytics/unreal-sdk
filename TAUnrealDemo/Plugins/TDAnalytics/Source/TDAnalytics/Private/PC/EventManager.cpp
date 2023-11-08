// Copyright 2021 ThinkingData. All Rights Reserved.

#include "EventManager.h"
#include "TaskHandle.h"
#include "TASaveEvent.h"
#include "TDAnalyticsPC.h"

#if WITH_EDITOR
#include "Editor/EditorEngine.h"
#include "Editor/UnrealEdEngine.h"
#else
#include "Engine/GameEngine.h"
#endif

UTAEventManager::UTAEventManager()
{

}

UTAEventManager::~UTAEventManager()
{
	// FTALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT(" ~UTAEventManager ")));
}

void UTAEventManager::EnqueueUserEvent(const FString& EventType, const FString& Properties)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Properties);
	FJsonSerializer::Deserialize(Reader, JsonObject);

	TSharedPtr<FJsonObject> m_DataJsonObject = MakeShareable(new FJsonObject);

	m_DataJsonObject->SetStringField(FTAConstants::KEY_TYPE, EventType);
	m_DataJsonObject->SetStringField(FTAConstants::KEY_TIME, FTAUtils::FormatTimeWithOffset(FDateTime::Now(), m_Instance->ta_GetDefaultTimeZone()));
	m_DataJsonObject->SetStringField(FTAConstants::KEY_DISTINCT_ID, m_Instance->ta_GetDistinctID());
	m_DataJsonObject->SetStringField(FTAConstants::KEY_DATA_ID, FTAUtils::GetGuid());

	if ( m_Instance->ta_GetAccountID() != "" )
    {
		m_DataJsonObject->SetStringField(FTAConstants::KEY_ACCOUNT_ID, m_Instance->ta_GetAccountID());
	}

	m_DataJsonObject->SetObjectField(FTAConstants::KEY_PROPERTIES, JsonObject);
	
	FString DataStr;
	TSharedRef<TJsonWriter<>> DataWriter = TJsonWriterFactory<>::Create(&DataStr);
	FJsonSerializer::Serialize(m_DataJsonObject.ToSharedRef(), DataWriter);
	m_TaskHandle->AddTask(DataStr);
	// FTALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("IsTimerActive1 = %s"), *(UKismetStringLibrary::Conv_BoolToString(m_GameInstance->GetTimerManager().IsTimerActive(WorkHandle)))));
	// FTALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("IsTimerPaused1 = %s"), *(UKismetStringLibrary::Conv_BoolToString(m_GameInstance->GetTimerManager().IsTimerPaused(WorkHandle)))));
}

void UTAEventManager::EnqueueTrackEvent(const FString& EventName, const FString& Properties, const FString& DynamicProperties, const FString& EventType, const FString& AddProperties)
{
	// FTALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT(" AddEvent %s"), *EventName));

	TSharedPtr<FJsonObject> m_PropertiesJsonObject = MakeShareable(new FJsonObject);
	TSharedPtr<FJsonObject> m_DataJsonObject = MakeShareable(new FJsonObject);

	if ( FTAUtils::IsInvalidName(EventName) )
    {
		FTALog::Warning(CUR_LOG_POSITION, TEXT("event name[ ") + EventName + TEXT(" ] is not valid !"));
	}

    TSharedPtr<FJsonObject> PresetPropertiesJsonObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> PresetPropertiesReader = TJsonReaderFactory<>::Create(m_Instance->ta_GetPresetProperties());
	FJsonSerializer::Deserialize(PresetPropertiesReader, PresetPropertiesJsonObject);
	for (auto& Elem : PresetPropertiesJsonObject->Values)
    {
		m_PropertiesJsonObject->SetField(Elem.Key, Elem.Value);
	}

	TSharedPtr<FJsonObject> SuperPropertiesJsonObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> SuperPropertiesReader = TJsonReaderFactory<>::Create(m_Instance->ta_GetSuperProperties());
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
	m_DataJsonObject->SetStringField(FTAConstants::KEY_TIME, FTAUtils::FormatTimeWithOffset(FDateTime::Now(), m_Instance->ta_GetDefaultTimeZone()));
	m_DataJsonObject->SetStringField(FTAConstants::KEY_DISTINCT_ID, m_Instance->ta_GetDistinctID());
	m_DataJsonObject->SetStringField(FTAConstants::KEY_DATA_ID, FTAUtils::GetGuid());

	if ( m_Instance->ta_GetAccountID() != "" )
    {
		m_DataJsonObject->SetStringField(FTAConstants::KEY_ACCOUNT_ID, m_Instance->ta_GetAccountID());
	}
	m_DataJsonObject->SetObjectField(FTAConstants::KEY_PROPERTIES, m_PropertiesJsonObject);

	FString DataStr;
	TSharedRef<TJsonWriter<>> DataWriter = TJsonWriterFactory<>::Create(&DataStr);
	FJsonSerializer::Serialize(m_DataJsonObject.ToSharedRef(), DataWriter);
	m_TaskHandle->AddTask(DataStr);
	// FTALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("IsTimerActive2 = %s"), *(UKismetStringLibrary::Conv_BoolToString(m_GameInstance->GetTimerManager().IsTimerActive(WorkHandle)))));
	// FTALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("IsTimerPaused2 = %s"), *(UKismetStringLibrary::Conv_BoolToString(m_GameInstance->GetTimerManager().IsTimerPaused(WorkHandle)))));
}

void UTAEventManager::BindInstance(UTDAnalyticsPC *Instance)
{
	m_Instance = Instance;
	//start timer
	//event num & time
	m_GameInstance = GetGameInstance();
	m_GameInstance->AddToRoot();
	m_GameInstance->GetTimerManager().SetTimer(WorkHandle, this, &UTAEventManager::Flush, 15.0f, true);

	m_TaskHandle = new FTaskHandle(Instance);
	m_RunnableThread = FRunnableThread::Create(m_TaskHandle, TEXT("TaskHandle"), 128 * 1024, TPri_AboveNormal, FPlatformAffinity::GetPoolThreadMask());
}

void UTAEventManager::Flush()
{
	//Empty
	if ( this->m_Instance->ta_GetTrackState().Equals(FTAConstants::TRACK_STATUS_NORMAL) )
	{
		m_TaskHandle->AddTask(TEXT(""));
	}
}

UGameInstance* UTAEventManager::GetGameInstance()
{
	UGameInstance* GameInstance = nullptr;
#if WITH_EDITOR
	UUnrealEdEngine* engine = Cast<UUnrealEdEngine>(GEngine);
	if (engine && engine->PlayWorld) GameInstance = engine->PlayWorld->GetGameInstance();
#else
	UGameEngine* engine = Cast<UGameEngine>(GEngine);
	if (engine) GameInstance = engine->GameInstance;
#endif
	return GameInstance;
}

