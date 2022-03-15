// Copyright 2021 ThinkingData. All Rights Reserved.
#pragma once

#include "../Common/TALog.h"
#include "../Common/TAConstants.h"

#include "GameFramework/SaveGame.h"
#include "TASaveGame.generated.h"

UCLASS()
class UTASaveGame : public USaveGame
{
    GENERATED_BODY()
public:

    UPROPERTY(VisibleAnywhere, Category = Basic)
    FString m_AppID;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    FString m_DistinctID;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    FString m_AccountID;
    
    UPROPERTY(VisibleAnywhere, Category = Basic)
    FString m_SuperProperties;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    uint32 UserIndex;

	UTASaveGame();

    void SetAppID(const FString& AppID);

    void SetDistinctID(const FString& DistinctID);

    void SetAccountID(const FString& AccountID);

    void SetSuperProperties(const FString& SuperProperties);

    void AddAll(UTASaveGame *SaveGame);
};