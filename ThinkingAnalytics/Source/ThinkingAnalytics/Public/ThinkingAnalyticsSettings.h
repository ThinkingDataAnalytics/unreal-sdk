// Copyright 2020 Thinking Data Ltd. All Rights Reserved.

#pragma once

#include "ThinkingAnalyticsSettings.generated.h"

UENUM()
enum class TAMode : uint8
{
    NORMAL = 0,
    DEBUG = 1,
    DEBUG_ONLY = 2
};

UCLASS(config = Engine, defaultconfig)
class UThinkingAnalyticsSettings : public UObject
{
    GENERATED_BODY()

public:
    UThinkingAnalyticsSettings(const FObjectInitializer& ObjectInitializer);

    // Url of TA receiver
    UPROPERTY(Config, EditAnywhere, Category = "ThinkingAnalytics", meta = (DisplayName = "Server Url"))
    FString ServerUrl;
    
    // App ID
    UPROPERTY(Config, EditAnywhere, Category = "ThinkingAnalytics", meta = (DisplayName = "App ID"))
    FString AppID;
    
    UPROPERTY(Config, EditAnywhere, Category = "ThinkingAnalytics", meta = (DisplayName = "SDK MODE"))
    TAMode Mode;
    
    UPROPERTY(Config, EditAnywhere, Category = "ThinkingAnalytics", meta = (DisplayName = "Enable Log"))
    bool bEnableLog;

    // runs SDK in the given timezone
    UPROPERTY(Config, EditAnywhere, Category = "ThinkingAnalytics", meta = (DisplayName = "TimeZone"))
    FString TimeZone;
};

