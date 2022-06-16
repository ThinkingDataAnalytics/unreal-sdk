// Copyright 2021 ThinkingData. All Rights Reserved.
#include "TAUtils.h"

FString FTAUtils::Pattern = TEXT("^[a-zA-Z][a-zA-Z\\d_]{0,49}$");

TArray<FString>* FTAUtils::DEFAULT_KEYS = new TArray<FString>{TEXT("#bundle_id"),TEXT("#duration")};


FString FTAUtils::EncodeData(const FString& UnprocessedStr)
{
	// Compatible with Chinese
	FTCHARToUTF8 ToUtf8Converter(UnprocessedStr.GetCharArray().GetData());
	auto UnprocessedDataLen = ToUtf8Converter.Length();
	auto UnprocessedData = ToUtf8Converter.Get();
	int32 CompressBufferLen = FCompression::CompressMemoryBound(NAME_Gzip, UnprocessedDataLen);
	void* CompressBuffer = FMemory::Malloc(CompressBufferLen);
	bool Result = FCompression::CompressMemory(NAME_Gzip, CompressBuffer, CompressBufferLen, UnprocessedData, 
		UnprocessedDataLen, ECompressionFlags::COMPRESS_BiasSpeed);

	FString CompressedStr; 
	if ( Result )
	{
		CompressedStr = FBase64::Encode((uint8*)CompressBuffer, CompressBufferLen);
	}
	else
	{
		FTALog::Warning(CUR_LOG_POSITION, TEXT("EncodeData Error !"));
	}
	FMemory::Free(CompressBuffer);
	return CompressedStr;
}

FString FTAUtils::GetCurrentTimeStamp()
{
	FDateTime TDateTime = FDateTime::Now();
	int64 SecondTimestamp = TDateTime.ToUnixTimestamp();
	int32 MillisecondPart = TDateTime.GetMillisecond();
	FString TimeStr = *FString::Printf(TEXT("%llu"), SecondTimestamp);
	TimeStr += *FString::Printf(TEXT("%lld"), MillisecondPart);
	return TimeStr;
}

FString FTAUtils::FormatTime(FDateTime DateTime)
{
	//2021-01-01 12:12:21.222
	return DateTime.ToString(TEXT("%Y-%m-%d %H:%M:%S.")) += *FString::Printf(TEXT("%lld"), DateTime.GetMillisecond());
}

FString FTAUtils::GetGuid()
{
	FGuid Guid;
	FGenericPlatformMisc::CreateGuid(Guid);
	return Guid.ToString();
}

bool FTAUtils::IsInvalidName(const FString& EventName)
{
	if ( &EventName == nullptr )
	{
		return true;
	}
	FRegexPattern MatherPatter(Pattern);
	FRegexMatcher Matcher(MatherPatter, EventName);
	while ( !Matcher.FindNext() )
	{
		return true;
	}

	return false;
}

FString FTAUtils::GetOS()
{
	return FPlatformProperties::IniPlatformName();
}

FString FTAUtils::GetOSVersion()
{
	return FPlatformMisc::GetOSVersion();
}

FString FTAUtils::GetGPUInfo()
{
	return FPlatformMisc::GetPrimaryGPUBrand();
}

FString FTAUtils::GetCPUInfo()
{
	return FPlatformMisc::GetCPUBrand();
}

FString FTAUtils::GetLocalIPAddress()
{
	FString IPAddress("NONE");
	bool CanBind = false;
	TSharedRef<FInternetAddr> LocalIP = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, CanBind);
	if( LocalIP->IsValid() )
	{
	   IPAddress = LocalIP->ToString(false); //如果想附加端口就写 ture
	}

	return IPAddress;
}

// FString FTAUtils::GetMachineId()
// {
// 	return FPlatformMisc::GetMachineId().ToString();
// }

FString FTAUtils::GetMachineAccountId()
{
	return FPlatformMisc::GetLoginId();
}

FString FTAUtils::GetEngineVersion()
{
	return FEngineAnalytics::GetProvider().GetAppVersion();
}

FString FTAUtils::GetProjectVersion()
{
	const UGeneralProjectSettings& ProjectSettings = *GetDefault<UGeneralProjectSettings>();
	return ProjectSettings.ProjectVersion;
}

uint32 FTAUtils::GetScreenWidth()
{
	return GSystemResolution.ResX;
}

uint32 FTAUtils::GetScreenHeight()
{
	return GSystemResolution.ResY;
}

int32 FTAUtils::GetZoneOffset()
{
	FDateTime UtcTime = FDateTime::UtcNow();
	FDateTime Time = FDateTime::Now();
	return (Time.ToUnixTimestamp() - UtcTime.ToUnixTimestamp()) / 3600;
}

FString FTAUtils::GetSystemLanguage()
{
	return FInternationalization::Get().GetCurrentLanguage()->GetName();
}

FString FTAUtils::GetAverageFps()
{	
	extern ENGINE_API float GAverageFPS;

	FString Result = *FString::Printf(TEXT("%.1f"), GAverageFPS);
    return Result;
}

FString FTAUtils::GetMemoryStats()
{	
	FPlatformMemoryStats PlatformMemoryStats = FPlatformMemory::GetStats();
	FString Result = *FString::Printf(TEXT("%.1f"), PlatformMemoryStats.AvailablePhysical / (1024.0f * 1024.0f * 1024.0f));
	Result += "/";
	Result += *FString::Printf(TEXT("%.1f"), PlatformMemoryStats.TotalPhysicalGB / (1.0f));
	return Result;
}

FString FTAUtils::GetDiskStats()
{
	uint64 TotalDiskSpace = 0;
	uint64 FreeDiskSpace = 0;
	FString Result = "";
	if (FPlatformMisc::GetDiskTotalAndFreeSpace("/", TotalDiskSpace, FreeDiskSpace))
	{	
		Result += *FString::Printf(TEXT("%d"), (uint32)(FreeDiskSpace / uint64(1024 * 1024 * 1024)));
		Result += "/";
		Result += *FString::Printf(TEXT("%d"), (uint32)(TotalDiskSpace / uint64(1024 * 1024 * 1024)));
	}
	return Result;
}

FString FTAUtils::GetProjectFileCreateTime()
{
	FString Result = "";
#if PLATFORM_MAC
	FString GamePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
	const int32 Length = GamePath.Len();
	const int32 index = GamePath.Find("/Contents/UE");
	FString SubPath = GamePath.Mid(0, index);
	FFileStatData FileData = IFileManager::Get().GetStatData(*SubPath);
	if(FileData.bIsValid)
	{
		Result = FTAUtils::FormatTime(FileData.CreationTime);
	}
#elif PLATFORM_WINDOWS
	FString GamePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
	FFileStatData FileData = IFileManager::Get().GetStatData(*GamePath);
	if(FileData.bIsValid)
	{
		Result = FormatTime(FileData.CreationTime);
	}
#endif
	return Result;
}