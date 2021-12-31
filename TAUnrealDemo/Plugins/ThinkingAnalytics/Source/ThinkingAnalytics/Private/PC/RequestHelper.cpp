// Copyright 2021 ThinkingData. All Rights Reserved.
#include "RequestHelper.h"


FRequestHelper::FRequestHelper()
{
}

void FRequestHelper::CallHttpRequest(const FString& ServerUrl, const FString& Data, bool IsDebug)
{
	FTALog::Warning(CUR_LOG_POSITION, TEXT("CallHttpRequest IsDebug: ") + UKismetStringLibrary::Conv_BoolToString(IsDebug));
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    if ( IsDebug )
    {
    	Request->SetHeader("Content-Type", "application/x-www-form-urlencoded;charset=utf-8");
    	Request->SetContentAsString(Data);
    }
    else
    {
		Request->SetHeader("Content-Type", "text/plain");
        FTALog::Warning(CUR_LOG_POSITION, TEXT("Data : ") + Data);
		Request->SetContentAsString(FTAUtils::EncodeData(Data));    
    }
    Request->SetVerb("POST");
    Request->SetURL(ServerUrl);
    
    Request->OnProcessRequestComplete().BindRaw(this, &FRequestHelper::RequestComplete);
    Request->ProcessRequest();
}

void FRequestHelper::RequestComplete(FHttpRequestPtr RequestPtr, FHttpResponsePtr ResponsePtr, bool IsSuccess)
{
	FTALog::Warning(CUR_LOG_POSITION, TEXT("is success = ") + (UKismetStringLibrary::Conv_BoolToString(IsSuccess)));
    FTALog::Warning(CUR_LOG_POSITION, TEXT("is responseCode = ") + (FString::FromInt(ResponsePtr->GetResponseCode())));
    FTALog::Warning(CUR_LOG_POSITION, TEXT("is content = ") + (ResponsePtr->GetContentAsString()));
    if ( !EHttpResponseCodes::IsOk(ResponsePtr->GetResponseCode()) ) return;
   // 获得返回的json数据
   // TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ResponsePtr->GetContentAsString());
   // 把FString转成TCHAR
   // TCHAR* serializedChar = ResponsePtr->GetContentAsString().GetCharArray().GetData();
}