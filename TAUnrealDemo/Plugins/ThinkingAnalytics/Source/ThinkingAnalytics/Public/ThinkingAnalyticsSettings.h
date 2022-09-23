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

    //Enable Encrypt
    UPROPERTY(Config, EditAnywhere, Category = "ThinkingAnalytics", meta = (DisplayName = "Enable Encrypt"))
    bool bEnableEncrypt;

    //EncryptPublicKey
    UPROPERTY(Config, EditAnywhere, Category = "ThinkingAnalytics", meta = (DisplayName = "EncryptPublicKey"))
    FString EncryptPublicKey;

    //Encrypt Version
    UPROPERTY(Config, EditAnywhere, Category = "ThinkingAnalytics", meta = (DisplayName = "EncryptVersion"))
    int EncryptVersion;

    //SymmetricEncryption
    UPROPERTY(Config, EditAnywhere, Category = "ThinkingAnalytics", meta = (DisplayName = "SymmetricEncryption"))
    FString SymmetricEncryption;

    //AsymmetricEncryption
    UPROPERTY(Config, EditAnywhere, Category = "ThinkingAnalytics", meta = (DisplayName = "AsymmetricEncryption"))
    FString AsymmetricEncryption;

    // runs SDK in the given timezone
    UPROPERTY(Config, EditAnywhere, Category = "ThinkingAnalytics", meta = (DisplayName = "TimeZone"))
    FString TimeZone;
};

