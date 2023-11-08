// Copyright 2020 Thinking Data Ltd. All Rights Reserved.

#pragma once

#include "TDAnalyticsSettings.generated.h"

UENUM()
enum class TAMode : uint8
{
    NORMAL = 0,
    DEBUG = 1,
    DEBUG_ONLY = 2
};

UCLASS(config = Engine, defaultconfig)
class UTDAnalyticsSettings : public UObject
{
    GENERATED_BODY()

public:
    
    UTDAnalyticsSettings(const FObjectInitializer& ObjectInitializer);

    // Url of TA receiver
    UPROPERTY(Config, EditAnywhere, Category = "TDAnalytics", meta = (DisplayName = "Server Url"))
    FString ServerUrl;
    
    // App ID
    UPROPERTY(Config, EditAnywhere, Category = "TDAnalytics", meta = (DisplayName = "App ID"))
    FString AppID;
    
    UPROPERTY(Config, EditAnywhere, Category = "TDAnalytics", meta = (DisplayName = "SDK MODE"))
    TAMode Mode;
    
    UPROPERTY(Config, EditAnywhere, Category = "TDAnalytics", meta = (DisplayName = "Enable Log"))
    bool bEnableLog;

    //Enable Encrypt
    UPROPERTY(Config, EditAnywhere, Category = "TDAnalytics", meta = (DisplayName = "Enable Encrypt"))
    bool bEnableEncrypt;

    //EncryptPublicKey
    UPROPERTY(Config, EditAnywhere, Category = "TDAnalytics", meta = (DisplayName = "EncryptPublicKey"))
    FString EncryptPublicKey;

    //Encrypt Version
    UPROPERTY(Config, EditAnywhere, Category = "TDAnalytics", meta = (DisplayName = "EncryptVersion"))
    int EncryptVersion;

    //SymmetricEncryption
    UPROPERTY(Config, EditAnywhere, Category = "TDAnalytics", meta = (DisplayName = "SymmetricEncryption"))
    FString SymmetricEncryption;

    //AsymmetricEncryption
    UPROPERTY(Config, EditAnywhere, Category = "TDAnalytics", meta = (DisplayName = "AsymmetricEncryption"))
    FString AsymmetricEncryption;

    // runs SDK in the given timezone
    UPROPERTY(Config, EditAnywhere, Category = "TDAnalytics", meta = (DisplayName = "TimeZone"))
    FString TimeZone;
};

