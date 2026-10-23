// Copyright 2021 ThinkingData. All Rights Reserved.
#pragma once

#include "Engine/GameInstance.h"
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

	void StartFlushTimer();

	void StopFlushTimer();

private:

	~UTAEventManager();

	UGameInstance* GetGameInstance();

#if WITH_EDITOR
	void HandlePrePIEEnded(const bool bIsSimulating);
	FDelegateHandle PrePIEEndedHandle;
#endif

	FTaskHandle* m_TaskHandle;

	TWeakObjectPtr<UGameInstance> m_GameInstance;

	FRunnableThread* m_RunnableThread;

	UTDAnalyticsPC* m_Instance;

	FTimerHandle WorkHandle;

};