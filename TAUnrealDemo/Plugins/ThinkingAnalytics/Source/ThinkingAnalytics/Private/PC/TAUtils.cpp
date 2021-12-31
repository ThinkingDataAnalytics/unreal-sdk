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
