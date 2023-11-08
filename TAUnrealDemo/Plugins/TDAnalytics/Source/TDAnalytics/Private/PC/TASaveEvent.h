// Copyright 2022 ThinkingData. All Rights Reserved.
#pragma once

#include "../Common/TALog.h"
#include "../Common/TAConstants.h"

#include "GameFramework/SaveGame.h"
#include "TASaveEvent.generated.h"

UCLASS()
class UTASaveEvent : public USaveGame
{
    GENERATED_BODY()
public:

    UPROPERTY(VisibleAnywhere, Category = Basic)
    FString EventJsonContent;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    uint32 UserIndex;

	UTASaveEvent();

    void AddEvent(TSharedPtr<FJsonObject> EventJson);

    TArray<TSharedPtr<FJsonObject>> GetEvents(uint32 Count);

    void RemoveEvents(uint32 Count);

    uint32 Num();
};