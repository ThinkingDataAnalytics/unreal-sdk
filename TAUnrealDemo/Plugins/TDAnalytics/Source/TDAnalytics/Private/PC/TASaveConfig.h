// Copyright 2021 ThinkingData. All Rights Reserved.
#pragma once

#include "../Common/TALog.h"
#include "../Common/TAConstants.h"

#include "GameFramework/SaveGame.h"
#include "TASaveConfig.generated.h"

UCLASS()
class UTASaveConfig : public USaveGame
{
    GENERATED_BODY()
public:

    UPROPERTY(VisibleAnywhere, Category = Basic)
    FString m_TrackState;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    FString m_DistinctID;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    FString m_AccountID;
    
    UPROPERTY(VisibleAnywhere, Category = Basic)
    FString m_SuperProperties;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    uint32 UserIndex;

	UTASaveConfig();

    void SetTrackState(const FString& State);

    void SetDistinctID(const FString& DistinctID);

    void SetAccountID(const FString& AccountID);

    void SetSuperProperties(const FString& SuperProperties);

    void AddAll(UTASaveConfig *SaveConfig);
};