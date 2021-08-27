// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TAExampleGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TAEXAMPLE_API ATAExampleGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
    
    virtual void StartPlay() override;
	
};
