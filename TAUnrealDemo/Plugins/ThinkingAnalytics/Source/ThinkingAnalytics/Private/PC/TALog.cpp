// Copyright 2021 ThinkingData. All Rights Reserved.
#include "TALog.h"

DEFINE_LOG_CATEGORY_STATIC(TA_PC, Log, All);

void FTALog::Warning(const FString CurLogPosition, const FString& LogStr)
{
    if ( m_Enable )
    {
        TALogWarn(TA_PC, FString::Printf(TEXT("%s --- %s"), *CurLogPosition, *LogStr));
        ScreenWarn(FString::Printf(TEXT("%s --- %s"), *CurLogPosition, *LogStr));
    }  
}

void FTALog::Warning(const FString CurLogPosition, const int& LogStr)
{
    if ( m_Enable )
    {
        TALogWarn(TA_PC, FString::Printf(TEXT("%s --- %d"), *CurLogPosition, LogStr));
        ScreenWarn(FString::Printf(TEXT("%s --- %d"), *CurLogPosition, LogStr));
    }       
}

void FTALog::Error(const FString CurLogPosition, const FString& LogStr)
{
    if ( m_Enable )
    {
        TALogError(TA_PC, FString::Printf(TEXT("%s --- %s"), *CurLogPosition, *LogStr));
        ScreenLog(FString::Printf(TEXT("%s --- %s"), *CurLogPosition, *LogStr));
  	}
}

void FTALog::Error(const FString CurLogPosition, const int& LogStr)
{
    if ( m_Enable )
    {
        TALogError(TA_PC, FString::Printf(TEXT("%s --- %d"), *CurLogPosition, LogStr));
        ScreenLog(FString::Printf(TEXT("%s --- %d"), *CurLogPosition, LogStr));
    }   
}

void FTALog::Info(const FString CurLogPosition, const FString& LogStr)
{
    if ( m_Enable )
    {
        TALogInfo(TA_PC, FString::Printf(TEXT("%s --- %s"), *CurLogPosition, *LogStr));
        ScreenLog(FString::Printf(TEXT("%s --- %s"), *CurLogPosition, *LogStr));
    }
}

void FTALog::Info(const FString CurLogPosition, const int& LogStr)
{
    if ( m_Enable )
    {
        TALogInfo(TA_PC, FString::Printf(TEXT("%s --- %d"), *CurLogPosition, LogStr));
        ScreenLog(FString::Printf(TEXT("%s --- %d"), *CurLogPosition, LogStr));
    }   
}

void FTALog::SetEnableLog(bool Enable)
{
   	m_Enable = Enable;	
}    
