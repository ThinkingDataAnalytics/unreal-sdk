// Copyright 2022 ThinkingData. All Rights Reserved.
#include "TASaveEvent.h"

UTASaveEvent::UTASaveEvent()
{
    UserIndex = FTAConstants::USER_INDEX_EVENT;
}


void UTASaveEvent::AddEvent(TSharedPtr<FJsonObject> EventJson)
{
	FString DataStr;
	TSharedRef<TJsonWriter<>> DataWriter = TJsonWriterFactory<>::Create(&DataStr);
	FJsonSerializer::Serialize(EventJson.ToSharedRef(), DataWriter);

	if ( !EventJsonContent.IsEmpty() )
	{
		EventJsonContent += TEXT("#tad");
	}
	EventJsonContent += DataStr;
}

TArray<TSharedPtr<FJsonObject>> UTASaveEvent::GetEvents(uint32 Count)
{
	TArray<TSharedPtr<FJsonObject>> r_Array;
	if ( EventJsonContent.IsEmpty() )
	{
		return r_Array; 
	}

	TArray<FString> TempArray;
	EventJsonContent.ParseIntoArray(TempArray, TEXT("#tad"), false);

	if ( (uint32)TempArray.Num() < Count )
	{
		for (int i = 0; i < TempArray.Num(); i++)
		{
			TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(TempArray[i]);
			FJsonSerializer::Deserialize(Reader, JsonObject);
			r_Array.Add(JsonObject);
		}
	}
	else
	{
		for (int i = 0; (uint32)i < Count; i++)
		{
			TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(TempArray[i]);
			FJsonSerializer::Deserialize(Reader, JsonObject);
			r_Array.Add(JsonObject);
		}
	}

	
	// FTALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("GetEvents %s"), *EventJsonContent));
	return r_Array;
}

void UTASaveEvent::RemoveEvents(uint32 Count)
{
	TArray<FString> TempArray;
	EventJsonContent.ParseIntoArray(TempArray, TEXT("#tad"), false);

	uint32 RealCount = TempArray.Num();
	if ( RealCount > Count )
	{
		RealCount = Count;
	}
	TempArray.RemoveAt(0, RealCount, true);

	FString TempContent;
	for (int i = 0; i < TempArray.Num(); i++)
	{
		TempContent += TempArray[i];
		if (i < TempArray.Num() - 1)
		{
			TempContent += TEXT("#tad");
		}
	}
	EventJsonContent = TempContent;
}

uint32 UTASaveEvent::Num()
{
	if ( EventJsonContent.IsEmpty() )
	{
		return 0; 
	}
	else
	{
		TArray<FString> TempArray;
		EventJsonContent.ParseIntoArray(TempArray, TEXT("#tad"), false);
		return TempArray.Num();
	}
}