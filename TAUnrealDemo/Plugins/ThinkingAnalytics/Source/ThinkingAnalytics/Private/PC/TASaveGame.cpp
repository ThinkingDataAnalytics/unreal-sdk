// Copyright 2021 ThinkingData. All Rights Reserved.
#include "TASaveGame.h"


UTASaveGame::UTASaveGame()
{
    UserIndex = FTAConstants::USER_INDEX;
}

void UTASaveGame::SetAppID(const FString& AppID)
{
    this->m_AppID = AppID;
}

void UTASaveGame::SetDistinctID(const FString& DistinctID)
{
    this->m_DistinctID = DistinctID;
}

void UTASaveGame::SetAccountID(const FString& AccountID)
{
    this->m_AccountID = AccountID;
}

void UTASaveGame::SetSuperProperties(const FString& SuperProperties)
{
    this->m_SuperProperties = SuperProperties;
}

void UTASaveGame::AddAll(UTASaveGame *SaveGame)
{
    if ( SaveGame != nullptr )
    {
        SetAppID(SaveGame->m_AppID);
        SetDistinctID(SaveGame->m_DistinctID);
        SetAccountID(SaveGame->m_AccountID);
        SetSuperProperties(SaveGame->m_SuperProperties);
    }
    else
    {
        FTALog::Warning(CUR_LOG_POSITION, TEXT("Passing a nullptr UTASaveGame !"));
    }
}