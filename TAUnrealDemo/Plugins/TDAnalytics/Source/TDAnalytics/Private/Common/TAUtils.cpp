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
	TimeStr += *FString::Printf(TEXT("%03d"), MillisecondPart);
	return TimeStr;
}

FString FTAUtils::FormatTimeWithOffset(FDateTime DateTime, float Zone_Offset)
{
	//FTALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("==== %f"), Zone_Offset));
	int64 DateTimeUnixTimeStamp = DateTime.ToUnixTimestamp();
	float CurrentOffset = (DateTimeUnixTimeStamp - FDateTime::UtcNow().ToUnixTimestamp()) / 3600.0;
	//FTALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("=--= %f"), CurrentOffset));
	int64 FormatDateTimeUnixTimeStamp = DateTimeUnixTimeStamp + 3600 * (Zone_Offset - CurrentOffset);
	return FDateTime::FromUnixTimestamp(FormatDateTimeUnixTimeStamp).ToString(TEXT("%Y-%m-%d %H:%M:%S.")) += *FString::Printf(TEXT("%03d"), DateTime.GetMillisecond());
}

void FTAUtils::FormatCustomTimeWithOffset(FString& DateTimeStr, float Zone_Offset)
{
	FRegexPattern *DefaultTimeFormatPattern = new FRegexPattern(TEXT("\\d{4}\\.\\d{2}\\.\\d{2}-\\d{2}\\.\\d{2}\\.\\d{2}\\.\\d{3}"));
	FRegexMatcher *DefaultTimeFormatMatcher = new FRegexMatcher(*DefaultTimeFormatPattern, DateTimeStr);

	while ( DefaultTimeFormatMatcher->FindNext() )
	{
		FString NeedReplaceTimeStr = DefaultTimeFormatMatcher->GetCaptureGroup(0);
		FDateTime *TDateTime = new FDateTime(FCString::Atoi(*NeedReplaceTimeStr.Mid(0,4)),
			FCString::Atoi(*NeedReplaceTimeStr.Mid(5,2)),
			FCString::Atoi(*NeedReplaceTimeStr.Mid(8,2)),
			FCString::Atoi(*NeedReplaceTimeStr.Mid(11,2)),
			FCString::Atoi(*NeedReplaceTimeStr.Mid(14,2)),
			FCString::Atoi(*NeedReplaceTimeStr.Mid(17,2)),
			FCString::Atoi(*NeedReplaceTimeStr.Mid(20,3))
			);
		FString NewTimeStr = FormatTimeWithOffset(*TDateTime, Zone_Offset);

		DateTimeStr = DateTimeStr.Replace(*NeedReplaceTimeStr, *NewTimeStr, ESearchCase::IgnoreCase);
		//FTALog::Warning(CUR_LOG_POSITION, DateTimeStr);
	}
}

FString FTAUtils::FormatTime(FDateTime DateTime)
{
	//2021-01-01 12:12:21.002
	return DateTime.ToString(TEXT("%Y-%m-%d %H:%M:%S.")) += *FString::Printf(TEXT("%03d"), DateTime.GetMillisecond());
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
	   IPAddress = LocalIP->ToString(false);
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

float FTAUtils::GetZoneOffset()
{
	FDateTime UtcTime = FDateTime::UtcNow();
	FDateTime Time = FDateTime::Now();
	return (Time.ToUnixTimestamp() - UtcTime.ToUnixTimestamp()) / 3600.0;
}

float FTAUtils::GetZoneOffsetWithTimeZone(const FString& TimeZone)
{
	if ( TimeZone.StartsWith("UTC", ESearchCase::IgnoreCase) || TimeZone.StartsWith("GMT", ESearchCase::IgnoreCase) )
	{
		const FString& MidStr = TimeZone.Mid(3);
		FString HourPartStr;
		FString MinPartStr;
		if ( MidStr.Split(":", &HourPartStr, &MinPartStr) && HourPartStr.Len() == 2 && MinPartStr.Len() == 2 )
		{
			int MinPart = FCString::Atoi(*MinPartStr);
			int HourPart = FCString::Atoi(*HourPartStr);
			if ( HourPartStr.StartsWith("-", ESearchCase::IgnoreCase) )
			{
				MinPart = 0 - MinPart;
			}
			return HourPart + MinPart / 60.0;
		}
	}
	return GetZoneOffset();
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

FString FTAUtils::GetProjectFileCreateTime(float Zone_Offset)
{
	FString Result = "";

#if WITH_EDITOR
	Result = FormatTimeWithOffset(FDateTime::Now(), Zone_Offset);
#elif PLATFORM_MAC
	FString GamePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
	const int32 Length = GamePath.Len();
	const int32 index = GamePath.Find("/Contents/UE");
	FString SubPath = GamePath.Mid(0, index);
	FFileStatData FileData = IFileManager::Get().GetStatData(*SubPath);
	if(FileData.bIsValid)
	{
		Result = FTAUtils::FormatTimeWithOffset(FileData.CreationTime, Zone_Offset);
	}
#elif PLATFORM_WINDOWS
	FString GamePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
	FFileStatData FileData = IFileManager::Get().GetStatData(*GamePath);
	if(FileData.bIsValid)
	{
		Result = FormatTimeWithOffset(FileData.CreationTime, Zone_Offset);
	}
#endif
	return Result;
}

FString FTAUtils::MergePropertiesWithOffset(const FString& FirstProp, const FString& SecondProp, float Zone_Offset)
{
	FString RetStr;

	TSharedPtr<FJsonObject> FirstDataObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> FirstReader = TJsonReaderFactory<>::Create(FirstProp);
	FJsonSerializer::Deserialize(FirstReader, FirstDataObject);

	TSharedPtr<FJsonObject> SecondDataObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> SecondReader = TJsonReaderFactory<>::Create(SecondProp);
	FJsonSerializer::Deserialize(SecondReader, SecondDataObject);

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RetStr);
#if ENGINE_MAJOR_VERSION >= 5
	FJsonObject::Duplicate(FirstDataObject, SecondDataObject);
	FJsonSerializer::Serialize(SecondDataObject.ToSharedRef(), Writer);
#else
	FJsonSerializer::Serialize(FirstDataObject.ToSharedRef(), Writer);
#endif // ENGINE_MAJOR_VERSION >= 5
	FormatCustomTimeWithOffset(RetStr, Zone_Offset);

	return RetStr;
}