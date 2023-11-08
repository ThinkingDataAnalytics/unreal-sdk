// Copyright 2021 ThinkingData. All Rights Reserved.
#pragma once

#include "EventManager.generated.h"

static TMap<FString, TArray<TSharedPtr<FJsonObject>>> TAEventSendMap;


class FTaskHandle;

class UTASaveEvent;

class UTDAnalyticsPC;

UCLASS()
class UTAEventManager : public UObject
{

GENERATED_BODY()

public:

	UTAEventManager();

	void EnqueueUserEvent(const FString& InEventType, const FString& InProperties);

	void EnqueueTrackEvent(const FString& InEventName, const FString& InProperties, const FString& InDynamicProperties, const FString& InEventType, const FString& InAddProperties);

	void Flush();

	void BindInstance(UTDAnalyticsPC *Instance);

private:

	~UTAEventManager();

	UGameInstance* GetGameInstance();

	FTaskHandle* m_TaskHandle;

	UGameInstance* m_GameInstance;

	FRunnableThread* m_RunnableThread;

	UTDAnalyticsPC* m_Instance;

	FTimerHandle WorkHandle;

};