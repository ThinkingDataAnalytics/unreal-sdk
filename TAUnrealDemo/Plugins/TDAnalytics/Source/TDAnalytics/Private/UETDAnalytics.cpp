// Copyright 2020 Thinking Data Ltd. All Rights Reserved.

#include "UETDAnalytics.h"
#include "Core.h"
#include "Json.h"
#include <string>
#include "ISettingsModule.h"

#include "TDAnalytics.h"
#include "TDAnalyticsProvider.h"
#include "TDAnalyticsSettings.h"

#define LOCTEXT_NAMESPACE "FTDAnalyticsModule"
DEFINE_LOG_CATEGORY_STATIC(LogTDAnalytics, Display, All);
IMPLEMENT_MODULE(FAnalyticsTDAnalytics, TDAnalytics)


TSharedPtr<IAnalyticsProvider> FAnalyticsProviderTDAnalytics::Provider;

void FAnalyticsTDAnalytics::StartupModule()
{
#if WITH_EDITOR
    // Register settings: `Settings -> Project Settings -> Plugins -> TDAnalytics
    if ( ISettingsModule* settingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings") )
    {
        settingsModule->RegisterSettings("Project", "Plugins", "TDAnalytics",
                                         LOCTEXT("RuntimeSettingsName", "TDAnalytics"),
                                         LOCTEXT("RuntimeSettingsDescription", "Configure bundle(plugin)"),
                                         GetMutableDefault<UTDAnalyticsSettings>());
    }
#endif
}

void FAnalyticsTDAnalytics::ShutdownModule()
{
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
    
    // Free the dll handle
}


TSharedPtr<IAnalyticsProvider> FAnalyticsTDAnalytics::CreateAnalyticsProvider(const FAnalyticsProviderConfigurationDelegate& GetConfigValue) const
{
    return FAnalyticsProviderTDAnalytics::Create();
}



FAnalyticsProviderTDAnalytics::FAnalyticsProviderTDAnalytics() :
bHasSessionStarted(false)
{
}

FAnalyticsProviderTDAnalytics::~FAnalyticsProviderTDAnalytics()
{
}

bool FAnalyticsProviderTDAnalytics::StartSession(const TArray<FAnalyticsEventAttribute>& Attributes)
{
    UE_LOG(LogTDAnalytics, Warning, TEXT("FAnalyticsProviderTDAnalytics::StartSession Success"));
    if ( !bHasSessionStarted )
    {
        UTDAnalytics::Initialize();
        bHasSessionStarted = true;
        UE_LOG(LogTDAnalytics, Display, TEXT("FAnalyticsProviderTDAnalytics::StartSession Success"));
    }
    return bHasSessionStarted;
}

void FAnalyticsProviderTDAnalytics::EndSession()
{
    if ( bHasSessionStarted )
    {
        bHasSessionStarted = false;
        UE_LOG(LogTDAnalytics, Display, TEXT("FAnalyticsProviderTDAnalytics::EndSession Success"));
    }
}

void FAnalyticsProviderTDAnalytics::FlushEvents()
{
    UTDAnalytics::Flush();
}


void FAnalyticsProviderTDAnalytics::SetUserID(const FString& InUserID)
{
    UTDAnalytics::Login(InUserID);
}

FString FAnalyticsProviderTDAnalytics::GetUserID() const
{
    return "Unsupported";
}


FString FAnalyticsProviderTDAnalytics::GetSessionID() const
{
    return "Unsupported";
}

bool FAnalyticsProviderTDAnalytics::SetSessionID(const FString& InSessionID)
{
    UE_LOG(LogTDAnalytics, Warning, TEXT("FAnalyticsProviderTDAnalytics::SetSessionID: Ignoring"));
    return true;
}

void FAnalyticsProviderTDAnalytics::RecordEvent(const FString& EventName, const TArray<FAnalyticsEventAttribute>& Attributes)
{
    if ( !bHasSessionStarted )
    {
        UE_LOG(LogTDAnalytics, Warning, TEXT("FAnalyticsProviderTDAnalytics::RecordEvent called while a session is not started. Ignoring."));
        return;
    }
    
    if (EventName.Len() <= 0) {
        UE_LOG(LogTDAnalytics, Warning, TEXT("FAnalyticsProviderTDAnalytics::RecordEvent called with invalid EventName. Ignoring"));
        return;
    }
    
    FString outStr;
    TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&outStr);
    /** Write JSON message */
    JsonWriter->WriteObjectStart();
    
    const int32 AttrCount = Attributes.Num();
    if (AttrCount > 0)
    {
        // Send an event for each attribute
        for (auto Attr : Attributes)
        {
            JsonWriter->WriteValue(*Attr.GetName(), *Attr.GetValue());
            // switch (Attr.AttrType) {
            //     case FAnalyticsEventAttribute::AttrTypeEnum::String:
            //         JsonWriter->WriteValue(*Attr.GetName(), *Attr.GetValue());
            //         break;
            //     case FAnalyticsEventAttribute::AttrTypeEnum::Boolean:
            //         JsonWriter->WriteValue(*Attr.GetName(), Attr.GetValue());
            //         break;
            //     case FAnalyticsEventAttribute::AttrTypeEnum::Number:
            //         JsonWriter->WriteValue(*Attr.GetName(), Attr.GetValue());
            //         break;
            //     default:
            //         JsonWriter->WriteValue(*Attr.GetName(), Attr.GetValue());
            //         break;
            // }
        }
    }
    JsonWriter->WriteObjectEnd();
    JsonWriter->Close();
    
    UTDAnalytics::Track(EventName, outStr);
}

void FAnalyticsProviderTDAnalytics::RecordItemPurchase(const FString& ItemId, const FString& Currency, int PerItemCost, int ItemQuantity)
{
    UE_LOG(LogTDAnalytics, Warning, TEXT("FAnalyticsProviderTDAnalytics::RecordItemPurchage: Ignoring"));
}

void FAnalyticsProviderTDAnalytics::RecordCurrencyPurchase(const FString& GameCurrencyType, int GameCurrencyAmount, const FString& RealCurrencyType, float RealMoneyCost, const FString& PaymentProvider)
{
    UE_LOG(LogTDAnalytics, Warning, TEXT("FAnalyticsProviderTDAnalytics::RecordCurrencyPurchage: Ignoring"));
}

void FAnalyticsProviderTDAnalytics::RecordCurrencyGiven(const FString& GameCurrencyType, int GameCurrencyAmount)
{
    UE_LOG(LogTDAnalytics, Warning, TEXT("FAnalyticsProviderTDAnalytics::RecordCurrencyGiven: Ignoring"));
}

void FAnalyticsProviderTDAnalytics::SetAge(int InAge)
{
    UE_LOG(LogTDAnalytics, Warning, TEXT("FAnalyticsProviderTDAnalytics::SetAge: Ignoring"));
}

void FAnalyticsProviderTDAnalytics::SetLocation(const FString& InLocation)
{
    UE_LOG(LogTDAnalytics, Warning, TEXT("FAnalyticsProviderTDAnalytics::SetLocation: Ignoring"));
}

void FAnalyticsProviderTDAnalytics::SetGender(const FString& InGender)
{
    UE_LOG(LogTDAnalytics, Warning, TEXT("FAnalyticsProviderTDAnalytics::SetGender: Ignoring"));
}

void FAnalyticsProviderTDAnalytics::SetBuildInfo(const FString& InBuildInfo)
{
    UE_LOG(LogTDAnalytics, Warning, TEXT("FAnalyticsProviderTDAnalytics::SetBuildInfo: Ignoring"));
}

void FAnalyticsProviderTDAnalytics::RecordError(const FString& Error, const TArray<FAnalyticsEventAttribute>& Attributes)
{
    UE_LOG(LogTDAnalytics, Warning, TEXT("FAnalyticsProviderTDAnalytics::RecordError: Ignoring"));
}

void FAnalyticsProviderTDAnalytics::RecordProgress(const FString& ProgressType, const FString& ProgressName, const TArray<FAnalyticsEventAttribute>& Attributes)
{
    UE_LOG(LogTDAnalytics, Warning, TEXT("FAnalyticsProviderTDAnalytics::RecordProgress: Ignoring"));
}

void FAnalyticsProviderTDAnalytics::RecordItemPurchase(const FString& ItemId, int ItemQuantity, const TArray<FAnalyticsEventAttribute>& Attributes)
{
    UE_LOG(LogTDAnalytics, Warning, TEXT("FAnalyticsProviderTDAnalytics::RecordItemPurchage: Ignoring"));
}


void FAnalyticsProviderTDAnalytics::RecordCurrencyPurchase(const FString& GameCurrencyType, int GameCurrencyAmount, const TArray<FAnalyticsEventAttribute>& Attributes)
{
    UE_LOG(LogTDAnalytics, Warning, TEXT("FAnalyticsProviderTDAnalytics::RecordCurrencyPurchage: Ignoring"));
}


void FAnalyticsProviderTDAnalytics::RecordCurrencyGiven(const FString& GameCurrencyType, int GameCurrencyAmount, const TArray<FAnalyticsEventAttribute>& Attributes)
{
    UE_LOG(LogTDAnalytics, Warning, TEXT("FAnalyticsProviderTDAnalytics::RecordCurrencyGiven: Ignoring"));
}
#undef LOCTEXT_NAMESPACE
