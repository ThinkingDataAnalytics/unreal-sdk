// Copyright 2021 ThinkingData. All Rights Reserved.
#include "TASaveConfig.h"


UTASaveConfig::UTASaveConfig()
{
    UserIndex = FTAConstants::USER_INDEX_CONFIG;
    m_TrackState = FTAConstants::TRACK_STATUS_NORMAL;
}

void UTASaveConfig::SetTrackState(const FString& State)
{
    this->m_TrackState = State;
}

void UTASaveConfig::SetDistinctID(const FString& DistinctID)
{
    this->m_DistinctID = DistinctID;
}

void UTASaveConfig::SetAccountID(const FString& AccountID)
{
    this->m_AccountID = AccountID;
}

void UTASaveConfig::SetSuperProperties(const FString& SuperProperties)
{
    this->m_SuperProperties = SuperProperties;
}

void UTASaveConfig::AddAll(UTASaveConfig *SaveConfig)
{
    if ( SaveConfig != nullptr )
    {
        SetTrackState(SaveConfig->m_TrackState);
        SetDistinctID(SaveConfig->m_DistinctID);
        SetAccountID(SaveConfig->m_AccountID);
        SetSuperProperties(SaveConfig->m_SuperProperties);
    }
    else
    {
        FTALog::Warning(CUR_LOG_POSITION, TEXT("Passing a nullptr UTASaveConfig !"));
    }
}