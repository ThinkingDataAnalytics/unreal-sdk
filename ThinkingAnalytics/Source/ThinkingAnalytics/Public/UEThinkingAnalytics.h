// Copyright 2020 Thinking Data Ltd. All Rights Reserved.

#pragma once

#include "Interfaces/IAnalyticsProviderModule.h"
#include "Core.h"

class IAnalyticsProvider;

class FAnalyticsThinkingAnalytics : public IAnalyticsProviderModule
{
    TSharedPtr<IAnalyticsProvider> Provider;
public:
    static inline FAnalyticsThinkingAnalytics& Get()
    {
          return FModuleManager::LoadModuleChecked< FAnalyticsThinkingAnalytics >( "ThinkingAnalytics" );
    }
    
    virtual TSharedPtr<IAnalyticsProvider> CreateAnalyticsProvider(const FAnalyticsProviderConfigurationDelegate& GetConfigValue) const override;
private:

    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
