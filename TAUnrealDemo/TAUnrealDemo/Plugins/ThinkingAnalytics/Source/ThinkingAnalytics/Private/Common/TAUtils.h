// Copyright 2021 ThinkingData. All Rights Reserved.
#pragma once

#include "TALog.h"

#include "ThinkingAnalytics.h"
#include "Misc/App.h"
#include "Misc/Compression.h"
#include "Misc/CompressionFlags.h"
#include "Misc/Base64.h"
#include "Misc/DateTime.h"
#include "HAL/UnrealMemory.h"
#include "Internationalization/Regex.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "Misc/Guid.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "SocketSubsystem.h"
#include "IAnalyticsProviderET.h"
#include "EngineAnalytics.h"
#include "GeneralProjectSettings.h"
#include "Kismet/KismetTextLibrary.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/Culture.h"

class FTAUtils 
{
public:

	static bool IsInvalidName(const FString& EventName);

	static bool CheckProperty(const FString& Properties);

	static uint32 GetScreenWidth();

	static uint32 GetScreenHeight();

	static int32 GetZoneOffset();

	static FString GetGuid();

	static FString GetOS();

	static FString GetOSVersion();

	static FString GetGPUInfo();

	static FString GetCPUInfo();

	static FString GetLocalIPAddress();

	// static FString GetMachineId();

	static FString GetCurrentTimeStamp();

	static FString GetMachineAccountId();

	static FString GetEngineVersion();

	static FString GetProjectVersion();

	static FString GetSystemLanguage();

	static FString FormatTime(FDateTime DateTime);

	static FString EncodeData(const FString& UnprocessedData);

	static FString GetAverageFps();

	static FString GetMemoryStats();

	static FString GetDiskStats();

	static FString GetProjectFileCreateTime();

private:

 	static FString Pattern;
 	
 	static TArray<FString>* DEFAULT_KEYS;
};

