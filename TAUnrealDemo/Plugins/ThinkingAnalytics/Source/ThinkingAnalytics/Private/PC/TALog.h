// Copyright 2021 ThinkingData. All Rights Reserved.
#pragma once

#define CUR_FUNC_NAME (FString(__FUNCTION__))
#define CUR_LINE  (FString::FromInt(__LINE__))

#define CUR_LOG_POSITION (CUR_FUNC_NAME + "(" + CUR_LINE + ")")

//Print Log
#define TALogInfo(LogCat, OutStr)	UE_LOG(LogCat, Log, TEXT("%s"), *FString(OutStr))
#define TALogWarn(LogCat, OutStr)	UE_LOG(LogCat, Warning, TEXT("%s"), *FString(OutStr))
#define TALogError(LogCat, OutStr)	UE_LOG(LogCat, Error, TEXT("%s"), *FString(OutStr))

//Print Message On Screen
#define ScreenLog(Param1) (ScreenLogT(Param1, 5))
#define ScreenLogT(Param1, Param2)	(GEngine->AddOnScreenDebugMessage(-1, Param2, FColor::White, *( ": " + Param1)))
#define ScreenWarn(Param1) (ScreenWarnT(Param1, 5))
#define ScreenWarnT(Param1, Param2)	(GEngine->AddOnScreenDebugMessage(-1, Param2, FColor::Yellow, *( ": " + Param1)))
#define ScreenError(Param1) (ScreenErrorT(Param1, 5))
#define ScreenErrorT(Param1, Param2)	(GEngine->AddOnScreenDebugMessage(-1, Param2, FColor::Red, *(": " + Param1)))

static bool m_Enable;

class FTALog
{
public:
	
	static void Warning(const FString CurLogPosition, const FString& LogStr);
	static void Warning(const FString CurLogPosition, const int& LogStr);
	static void Error(const FString CurLogPosition, const FString& LogStr);
	static void Error(const FString CurLogPosition, const int& LogStr);
	static void Info(const FString CurLogPosition, const FString& LogStr);
	static void Info(const FString CurLogPosition, const int& LogStr);
	static void SetEnableLog(bool Enable);
};

