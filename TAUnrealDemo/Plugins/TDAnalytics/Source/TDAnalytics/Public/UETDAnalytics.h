// Copyright 2020 Thinking Data Ltd. All Rights Reserved.

#pragma once

#include "Interfaces/IAnalyticsProviderModule.h"
#include "Core.h"

class IAnalyticsProvider;

class FAnalyticsTDAnalytics : public IAnalyticsProviderModule
{
    TSharedPtr<IAnalyticsProvider> Provider;
public:

    static inline FAnalyticsTDAnalytics& Get()
    {
          return FModuleManager::LoadModuleChecked< FAnalyticsTDAnalytics >( "TDAnalytics" );
    }
    
    virtual TSharedPtr<IAnalyticsProvider> CreateAnalyticsProvider(const FAnalyticsProviderConfigurationDelegate& GetConfigValue) const override;
    
private:

    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
