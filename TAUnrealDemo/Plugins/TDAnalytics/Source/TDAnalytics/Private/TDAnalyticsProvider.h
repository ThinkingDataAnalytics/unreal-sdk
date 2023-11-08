// Copyright 2020 Thinking Data Ltd. All Rights Reserved.

#pragma once
#include "Interfaces/IAnalyticsProvider.h"

class FAnalyticsProviderTDAnalytics :
  public IAnalyticsProvider
{
    /** Tracks whether we need to start the session or restart it */
    bool bHasSessionStarted;

    static TSharedPtr<IAnalyticsProvider> Provider;
    FAnalyticsProviderTDAnalytics();

public:

    static TSharedPtr<IAnalyticsProvider> Create()

    {
        if (!Provider.IsValid())
        {
            Provider = TSharedPtr<IAnalyticsProvider>(new FAnalyticsProviderTDAnalytics());
        }
        return Provider;
    }

	static void Destroy()
	{
          Provider.Reset();
	}

	static TSharedPtr<FAnalyticsProviderTDAnalytics> GetProvider() 
    {
		return StaticCastSharedPtr<FAnalyticsProviderTDAnalytics>(Provider);
	}

    virtual ~FAnalyticsProviderTDAnalytics();

    virtual bool StartSession(const TArray<FAnalyticsEventAttribute>& Attributes) override;

    virtual void EndSession() override;

    virtual void FlushEvents() override;

    virtual void SetUserID(const FString& InUserID) override;

    virtual FString GetUserID() const override;

    virtual FString GetSessionID() const override;

    virtual bool SetSessionID(const FString& InSessionID) override;

    virtual void RecordEvent(const FString& EventName, const TArray<FAnalyticsEventAttribute>& Attributes) override;

    virtual void RecordItemPurchase(const FString& ItemId, const FString& Currency, int PerItemCost, int ItemQuantity) override;

    virtual void RecordCurrencyPurchase(const FString& GameCurrencyType, int GameCurrencyAmount, const FString& RealCurrencyType, float RealMoneyCost, const FString& PaymentProvider) override;

    virtual void RecordCurrencyGiven(const FString& GameCurrencyType, int GameCurrencyAmount) override;

    virtual void SetBuildInfo(const FString& InBuildInfo) override;

    virtual void SetGender(const FString& InGender) override;

    virtual void SetLocation(const FString& InLocation) override;

    virtual void SetAge(const int32 InAge) override;

    virtual void RecordItemPurchase(const FString& ItemId, int ItemQuantity, const TArray<FAnalyticsEventAttribute>& EventAttrs) override;

    virtual void RecordCurrencyPurchase(const FString& GameCurrencyType, int GameCurrencyAmount, const TArray<FAnalyticsEventAttribute>& EventAttrs) override;

    virtual void RecordCurrencyGiven(const FString& GameCurrencyType, int GameCurrencyAmount, const TArray<FAnalyticsEventAttribute>& EventAttrs) override;

    virtual void RecordError(const FString& Error, const TArray<FAnalyticsEventAttribute>& EventAttrs) override;
    
    virtual void RecordProgress(const FString& ProgressType, const FString& ProgressHierarchy, const TArray<FAnalyticsEventAttribute>& EventAttrs) override;
};
