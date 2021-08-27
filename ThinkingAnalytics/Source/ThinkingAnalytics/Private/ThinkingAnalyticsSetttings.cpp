// Copyright 2020 Thinking Data Ltd. All Rights Reserved.

#include "ThinkingAnalyticsSettings.h"

UThinkingAnalyticsSettings::UThinkingAnalyticsSettings(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , ServerUrl("")
    , AppID("")
    , Mode(TAMode::NORMAL)
    , bEnableLog(false)
    , TimeZone("")
{
}
