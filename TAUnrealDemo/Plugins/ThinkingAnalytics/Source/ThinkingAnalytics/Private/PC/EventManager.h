// Copyright 2021 ThinkingData. All Rights Reserved.
#pragma once

#include "EventManager.generated.h"

static TMap<FString, TArray<TSharedPtr<FJsonObject>>> TAEventSendMap;

//用于编译时循环引用错误
class FTaskHandle;

class UTASaveEvent;

class UThinkingAnalyticsPC;

UCLASS()
class UTAEventManager : public UObject
{

GENERATED_BODY()

public:

	UTAEventManager();

	void EnqueueUserEvent(const FString& InEventType, const FString& InProperties);

	void EnqueueTrackEvent(const FString& InEventName, const FString& InProperties, const FString& InDynamicProperties, const FString& InEventType, const FString& InAddProperties);

	void Flush();

	void BindInstance(UThinkingAnalyticsPC *Instance);

private:

	~UTAEventManager();

	UGameInstance* GetGameInstance();

	FTaskHandle* m_TaskHandle;

	UGameInstance* m_GameInstance;

	FRunnableThread* m_RunnableThread;

	UThinkingAnalyticsPC* m_Instance;

	FTimerHandle WorkHandle;

};