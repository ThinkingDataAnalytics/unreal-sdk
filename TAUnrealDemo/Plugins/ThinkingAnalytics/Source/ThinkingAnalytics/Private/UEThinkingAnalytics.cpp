// Copyright 2020 Thinking Data Ltd. All Rights Reserved.

#include "UEThinkingAnalytics.h"
#include "Core.h"
#include "Json.h"
#include <string>
#include "ISettingsModule.h"

#include "ThinkingAnalytics.h"
#include "ThinkingAnalyticsProvider.h"
#include "ThinkingAnalyticsSettings.h"

#define LOCTEXT_NAMESPACE "FThinkingAnalyticsModule"
DEFINE_LOG_CATEGORY_STATIC(LogThinkingAnalytics, Display, All);
IMPLEMENT_MODULE(FAnalyticsThinkingAnalytics, ThinkingAnalytics)


TSharedPtr<IAnalyticsProvider> FAnalyticsProviderThinkingAnalytics::Provider;

void FAnalyticsThinkingAnalytics::StartupModule()
{
#if WITH_EDITOR
    // Register settings: `Settings -> Project Settings -> Plugins -> Thinking Analytics
    if ( ISettingsModule* settingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings") )
    {
        settingsModule->RegisterSettings("Project", "Plugins", "Thinking Analytics",
                                         LOCTEXT("RuntimeSettingsName", "Thinking Analytics"),
                                         LOCTEXT("RuntimeSettingsDescription", "Configure bundle(plugin)"),
                                         GetMutableDefault<UThinkingAnalyticsSettings>());
    }
#endif
}

void FAnalyticsThinkingAnalytics::ShutdownModule()
{
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
    
    // Free the dll handle
}


TSharedPtr<IAnalyticsProvider> FAnalyticsThinkingAnalytics::CreateAnalyticsProvider(const FAnalyticsProviderConfigurationDelegate& GetConfigValue) const
{
    return FAnalyticsProviderThinkingAnalytics::Create();
}



FAnalyticsProviderThinkingAnalytics::FAnalyticsProviderThinkingAnalytics() :
bHasSessionStarted(false)
{
}

FAnalyticsProviderThinkingAnalytics::~FAnalyticsProviderThinkingAnalytics()
{
}

bool FAnalyticsProviderThinkingAnalytics::StartSession(const TArray<FAnalyticsEventAttribute>& Attributes)
{
    UE_LOG(LogThinkingAnalytics, Warning, TEXT("FAnalyticsProviderThinkingAnalytics::StartSession Success"));
    if ( !bHasSessionStarted )
    {
        UThinkingAnalytics::Initialize();
        bHasSessionStarted = true;
        UE_LOG(LogThinkingAnalytics, Display, TEXT("FAnalyticsProviderThinkingAnalytics::StartSession Success"));
    }
    return bHasSessionStarted;
}

void FAnalyticsProviderThinkingAnalytics::EndSession()
{
    if ( bHasSessionStarted )
    {
        bHasSessionStarted = false;
        UE_LOG(LogThinkingAnalytics, Display, TEXT("FAnalyticsProviderThinkingAnalytics::EndSession Success"));
    }
}

void FAnalyticsProviderThinkingAnalytics::FlushEvents()
{
    UThinkingAnalytics::Flush();
}


void FAnalyticsProviderThinkingAnalytics::SetUserID(const FString& InUserID)
{
    UThinkingAnalytics::Login(InUserID);
}

FString FAnalyticsProviderThinkingAnalytics::GetUserID() const
{
    return "Unsupported";
}


FString FAnalyticsProviderThinkingAnalytics::GetSessionID() const
{
    return "Unsupported";
}

bool FAnalyticsProviderThinkingAnalytics::SetSessionID(const FString& InSessionID)
{
    UE_LOG(LogThinkingAnalytics, Warning, TEXT("FAnalyticsProviderThinkingAnalytics::SetSessionID: Ignoring"));
    return true;
}

void FAnalyticsProviderThinkingAnalytics::RecordEvent(const FString& EventName, const TArray<FAnalyticsEventAttribute>& Attributes)
{
    if ( !bHasSessionStarted )
    {
        UE_LOG(LogThinkingAnalytics, Warning, TEXT("FAnalyticsProviderThinkingAnalytics::RecordEvent called while a session is not started. Ignoring."));
        return;
    }
    
    if (EventName.Len() <= 0) {
        UE_LOG(LogThinkingAnalytics, Warning, TEXT("FAnalyticsProviderThinkingAnalytics::RecordEvent called with invalid EventName. Ignoring"));
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
    
    UThinkingAnalytics::Track(EventName, outStr);
}

void FAnalyticsProviderThinkingAnalytics::RecordItemPurchase(const FString& ItemId, const FString& Currency, int PerItemCost, int ItemQuantity)
{
    UE_LOG(LogThinkingAnalytics, Warning, TEXT("FAnalyticsProviderThinkingAnalytics::RecordItemPurchage: Ignoring"));
}

void FAnalyticsProviderThinkingAnalytics::RecordCurrencyPurchase(const FString& GameCurrencyType, int GameCurrencyAmount, const FString& RealCurrencyType, float RealMoneyCost, const FString& PaymentProvider)
{
    UE_LOG(LogThinkingAnalytics, Warning, TEXT("FAnalyticsProviderThinkingAnalytics::RecordCurrencyPurchage: Ignoring"));
}

void FAnalyticsProviderThinkingAnalytics::RecordCurrencyGiven(const FString& GameCurrencyType, int GameCurrencyAmount)
{
    UE_LOG(LogThinkingAnalytics, Warning, TEXT("FAnalyticsProviderThinkingAnalytics::RecordCurrencyGiven: Ignoring"));
}

void FAnalyticsProviderThinkingAnalytics::SetAge(int InAge)
{
    UE_LOG(LogThinkingAnalytics, Warning, TEXT("FAnalyticsProviderThinkingAnalytics::SetAge: Ignoring"));
}

void FAnalyticsProviderThinkingAnalytics::SetLocation(const FString& InLocation)
{
    UE_LOG(LogThinkingAnalytics, Warning, TEXT("FAnalyticsProviderThinkingAnalytics::SetLocation: Ignoring"));
}

void FAnalyticsProviderThinkingAnalytics::SetGender(const FString& InGender)
{
    UE_LOG(LogThinkingAnalytics, Warning, TEXT("FAnalyticsProviderThinkingAnalytics::SetGender: Ignoring"));
}

void FAnalyticsProviderThinkingAnalytics::SetBuildInfo(const FString& InBuildInfo)
{
    UE_LOG(LogThinkingAnalytics, Warning, TEXT("FAnalyticsProviderThinkingAnalytics::SetBuildInfo: Ignoring"));
}

void FAnalyticsProviderThinkingAnalytics::RecordError(const FString& Error, const TArray<FAnalyticsEventAttribute>& Attributes)
{
    UE_LOG(LogThinkingAnalytics, Warning, TEXT("FAnalyticsProviderThinkingAnalytics::RecordError: Ignoring"));
}

void FAnalyticsProviderThinkingAnalytics::RecordProgress(const FString& ProgressType, const FString& ProgressName, const TArray<FAnalyticsEventAttribute>& Attributes)
{
    UE_LOG(LogThinkingAnalytics, Warning, TEXT("FAnalyticsProviderThinkingAnalytics::RecordProgress: Ignoring"));
}

void FAnalyticsProviderThinkingAnalytics::RecordItemPurchase(const FString& ItemId, int ItemQuantity, const TArray<FAnalyticsEventAttribute>& Attributes)
{
    UE_LOG(LogThinkingAnalytics, Warning, TEXT("FAnalyticsProviderThinkingAnalytics::RecordItemPurchage: Ignoring"));
}


void FAnalyticsProviderThinkingAnalytics::RecordCurrencyPurchase(const FString& GameCurrencyType, int GameCurrencyAmount, const TArray<FAnalyticsEventAttribute>& Attributes)
{
    UE_LOG(LogThinkingAnalytics, Warning, TEXT("FAnalyticsProviderThinkingAnalytics::RecordCurrencyPurchage: Ignoring"));
}


void FAnalyticsProviderThinkingAnalytics::RecordCurrencyGiven(const FString& GameCurrencyType, int GameCurrencyAmount, const TArray<FAnalyticsEventAttribute>& Attributes)
{
    UE_LOG(LogThinkingAnalytics, Warning, TEXT("FAnalyticsProviderThinkingAnalytics::RecordCurrencyGiven: Ignoring"));
}
#undef LOCTEXT_NAMESPACE
