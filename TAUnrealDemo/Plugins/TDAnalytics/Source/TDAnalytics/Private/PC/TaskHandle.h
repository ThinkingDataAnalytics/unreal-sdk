// Copyright 2021 ThinkingData. All Rights Reserved.
#pragma once

#include "RequestHelper.h"
#include "TDAnalyticsPC.h"
#include "../Common/TALog.h"
#include "../Common/TAUtils.h"
#include "TASaveEvent.h"
#include "Kismet/KismetStringLibrary.h"

class FTaskHandle : public FRunnable
{
public:

	FTaskHandle(UTDAnalyticsPC* Instance);

	virtual bool Init() override;

	virtual uint32 Run() override;

	virtual void Stop() override;

	virtual void Exit() override;

	void AddTask(FString EventJsonStr);

	void RequestCallback(FString Msg, int32 Code, bool IsSuccess, uint32 EventNum);

private:

	UTDAnalyticsPC* m_Instance;

	TArray<FString> TaskArray;

	FCriticalSection SetCritical;

	UTASaveEvent* m_SaveEvent;
	
	FString m_SaveName;

	bool Working;

	bool IsPaused;

	void Flush();

	void SaveToLocal(TSharedPtr<FJsonObject> EventJson);

	void FlushFromLocalNormal();

	void FlushFromLocalDebug(TSharedPtr<FJsonObject> DebugJson);
};