#include "TaskHandle.h"

bool FTaskHandle::Init()
{
	FTALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("Init")));
	return true;
}

uint32 FTaskHandle::Run()
{
	FTALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("Run")));
	while(true)
	{
		//lock
		FScopeLock SetLock(&SetCritical);
		if ( !IsPaused && !Working )
		{
			// FTALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("Run  %d"), TaskArray.Num()));
			// 0.001s
#if PLATFORM_HAS_BSD_TIME
			FGenericPlatformProcess::Sleep(0.001f);
#endif // PLATFORM_HAS_BSD_TIME
			if ( TaskArray.Num() > 0 )
			{
				FString DataStr = TaskArray[0];
				// FTALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("Flush==%s=="), *DataStr));
				if ( DataStr.IsEmpty() )
				{	
					// FTALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("Flush")));
					Flush();
				}
				else
				{
					TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
					TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(DataStr);
					FJsonSerializer::Deserialize(Reader, JsonObject);
					// FTALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("SaveToLocal")));
					SaveToLocal(JsonObject);
				}
			}
			else
			{
				IsPaused = true;
			}
		}
	}
	return 0;
}

void FTaskHandle::Stop()
{
	FTALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("Stop")));
}

void FTaskHandle::Exit()
{
	FTALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("Exit")));
}

void FTaskHandle::AddTask(FString EventJsonStr)
{
	FTALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("AddTask %d"), TaskArray.Num()));
	TaskArray.Add(EventJsonStr);
	IsPaused = false;
}

FTaskHandle::FTaskHandle(UTDAnalyticsPC* Instance)
{
	Working = false;
	IsPaused = true;
	m_Instance = Instance;
	m_Instance->AddToRoot();
	m_SaveName = m_Instance->InstanceAppID + FTAConstants::KEY_SAVE_EVENT_SUFFIX;

	//lock
	FScopeLock SetLock(&SetCritical);
	m_SaveEvent = Cast<UTASaveEvent>(UGameplayStatics::LoadGameFromSlot(m_SaveName, FTAConstants::USER_INDEX_EVENT));

	if ( !m_SaveEvent )
	{
		m_SaveEvent = Cast<UTASaveEvent>(UGameplayStatics::CreateSaveGameObject(UTASaveEvent::StaticClass()));
		UGameplayStatics::SaveGameToSlot(m_SaveEvent, m_SaveName, FTAConstants::USER_INDEX_EVENT);
	}

	m_SaveEvent->AddToRoot();
}

void FTaskHandle::Flush()
{
	//lock
	FScopeLock SetLock(&SetCritical);
	if ( !Working )
	{
		if ( !m_Instance->ta_GetTrackState().Equals(FTAConstants::TRACK_STATUS_NORMAL) )
		{
			TaskArray.RemoveAt(0);
			return;
		}
		Working = true;
		TaskArray.RemoveAt(0);
		if ( m_Instance->ta_GetMode() == TAMode::NORMAL )
	    {
	    	FlushFromLocalNormal();
	    }
	    else if ( m_Instance->ta_GetMode() == TAMode::DEBUG )
	    {
	    	FlushFromLocalDebug(nullptr);
	    }
	}
}

void FTaskHandle::SaveToLocal(TSharedPtr<FJsonObject> EventJson)
{
	//lock
	FScopeLock SetLock(&SetCritical);

	FString InDataStr;
	TSharedRef<TJsonWriter<>> InDataWriter = TJsonWriterFactory<>::Create(&InDataStr);
	FJsonSerializer::Serialize(EventJson.ToSharedRef(), InDataWriter);

	FTAUtils::FormatCustomTimeWithOffset(InDataStr, m_Instance->ta_GetDefaultTimeZone());

	TSharedPtr<FJsonObject> FinalDataObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> FinalReader = TJsonReaderFactory<>::Create(InDataStr);
	FJsonSerializer::Deserialize(FinalReader, FinalDataObject);

	if ( m_Instance->ta_GetMode() == TAMode::DEBUG_ONLY )
	{
		FlushFromLocalDebug(FinalDataObject);
		TaskArray.RemoveAt(0);
	}
	else
	{
		m_SaveEvent->AddEvent(FinalDataObject);
		UGameplayStatics::SaveGameToSlot(m_SaveEvent, m_SaveName, FTAConstants::USER_INDEX_EVENT);
		uint32 CurrentNum = m_SaveEvent->Num();
		if ( CurrentNum >= 20 )
		{
			Flush();
		}
		else
		{
			TaskArray.RemoveAt(0);
		}
		FString Data;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Data);
		FJsonSerializer::Serialize(FinalDataObject.ToSharedRef(), Writer);
		FTALog::Warning(CUR_LOG_POSITION, TEXT("SaveToLocal Success !") + Data);
	}
}

void FTaskHandle::FlushFromLocalNormal()
{
	FTALog::Warning(CUR_LOG_POSITION, TEXT("FlushFromLocalNormal !"));
	//lock
	FScopeLock SetLock(&SetCritical);

	TArray<TSharedPtr<FJsonObject>> SendArray = m_SaveEvent->GetEvents(50);
	if ( SendArray.Num() <= 0)
	{
		Working = false;
    	FTALog::Warning(CUR_LOG_POSITION, TEXT("FlushFromLocalNormal >>> local is Empty"));
		return;
	}

	FRequestHelper* Helper = new FRequestHelper();

	FString ServerUrl = m_Instance->ta_GetServerUrl();
	int32 SyncPoint = ServerUrl.Find(TEXT("sync"), ESearchCase::CaseSensitive, ESearchDir::FromEnd);
	if ( SyncPoint != -1 )
    {
		ServerUrl = ServerUrl.Left(SyncPoint);
	}

	ServerUrl += "/sync";
	TSharedPtr<FJsonObject> DataJsonObject = MakeShareable(new FJsonObject);
	TArray< TSharedPtr<FJsonValue> > DataArray;

	for (int i = 0; i < SendArray.Num(); ++i)
	{
		TSharedPtr<FJsonValueObject> DataValue = MakeShareable(new FJsonValueObject(SendArray[i]));
		DataArray.Add(DataValue);
	}

	DataJsonObject->SetArrayField(FTAConstants::KEY_DATA, DataArray);
	DataJsonObject->SetStringField(FTAConstants::KEY_APP_ID, m_Instance->InstanceAppID);
	DataJsonObject->SetStringField(FTAConstants::KEY_FLUSH_TIME, FTAUtils::GetCurrentTimeStamp());

	FString Data;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Data);
	FJsonSerializer::Serialize(DataJsonObject.ToSharedRef(), Writer);
	Helper->CallHttpRequest(ServerUrl, Data, false, this, SendArray.Num());
}

void FTaskHandle::FlushFromLocalDebug(TSharedPtr<FJsonObject> DebugJson)
{
	//lock
	FScopeLock SetLock(&SetCritical);

	TSharedPtr<FJsonObject> m_DebugJson = DebugJson;

	if ( m_DebugJson == nullptr)
	{
		TArray<TSharedPtr<FJsonObject>> SendArray = m_SaveEvent->GetEvents(1);
		if ( SendArray.Num()<=0 )
		{
			Working = false;
	    	FTALog::Warning(CUR_LOG_POSITION, TEXT("FlushFromLocalDebug >>> local is Empty"));
			return;
		}
		m_DebugJson = SendArray[0];
	}
	

	FRequestHelper* Helper = new FRequestHelper();

	FString ServerUrl = m_Instance->ta_GetServerUrl();
	int32 SyncPoint = ServerUrl.Find(TEXT("sync"), ESearchCase::CaseSensitive, ESearchDir::FromEnd);

	if ( SyncPoint != -1 )
    {
		ServerUrl = ServerUrl.Left(SyncPoint);
	}
	// 
	FString m_DataStr;
	FString ServerData;
	ServerUrl += "/data_debug";
	ServerData += "appid=";
	ServerData += m_Instance->InstanceAppID;
	ServerData += "&deviceId=";
	ServerData += m_Instance->ta_GetDeviceID();
	if ( m_DebugJson != nullptr )
	{
		ServerData += "&dryRun=1";
	}
	ServerData += "&source=client&data=";

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&m_DataStr);
	FJsonSerializer::Serialize(m_DebugJson.ToSharedRef(), Writer);

	ServerData += FGenericPlatformHttp::UrlEncode(m_DataStr);
	Helper->CallHttpRequest(ServerUrl, ServerData, true, this, 1);
}

void FTaskHandle::RequestCallback(FString Msg, int32 Code, bool IsSuccess, uint32 EventNum)
{
	//lock
	FScopeLock SetLock(&SetCritical);
	if ( Code == 200 )
	{
		if ( m_Instance->ta_GetMode() != TAMode::DEBUG_ONLY )
		{
			m_SaveEvent->RemoveEvents(EventNum);
			UGameplayStatics::SaveGameToSlot(m_SaveEvent, m_SaveName, FTAConstants::USER_INDEX_EVENT);
			FTALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("code = %d"), Code));
		}
		Working = false;
		if ( m_SaveEvent->Num() > 0 )
		{
			Flush();
		}
	}
	else
	{
		FTALog::Error(CUR_LOG_POSITION, *FString::Printf(TEXT("success = %s , code = %s , msg = %s"), *(UKismetStringLibrary::Conv_BoolToString(IsSuccess)), *FString::FromInt(Code), *Msg));
		// TaskArray.Insert(TEXT(""), 0);
		Working = false;
	}
}