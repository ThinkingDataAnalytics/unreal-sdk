// Copyright 2021 ThinkingData. All Rights Reserved.
#pragma once

#include "TALog.h"
#include "TAUtils.h"

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/KismetStringLibrary.h"

class FRequestHelper
{
public:

	FRequestHelper();
	void CallHttpRequest(const FString& ServerUrl, const FString& Data, bool IsDebug);	

private:
	
		void RequestComplete(FHttpRequestPtr RequestPtr, FHttpResponsePtr ResponsePtr, bool IsSuccess);
};