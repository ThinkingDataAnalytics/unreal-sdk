// Copyright 2021 ThinkingData. All Rights Reserved.
#pragma once

#include "EventManager.h"
#include "../Common/TALog.h"
#include "../Common/TAUtils.h"
#include "TaskHandle.h"

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

class FRequestHelper
{
public:

	FRequestHelper();

	void CallHttpRequest(const FString& ServerUrl, const FString& Data, bool IsDebug, FTaskHandle* TaskHandle, uint32 EventNum);

private:
	
	FTaskHandle* m_TaskHandle;

	uint32 m_EventNum;

	void RequestComplete(FHttpRequestPtr RequestPtr, FHttpResponsePtr ResponsePtr, bool IsSuccess);
};