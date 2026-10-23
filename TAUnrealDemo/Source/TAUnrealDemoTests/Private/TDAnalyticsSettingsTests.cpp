// Copyright 2020 Thinking Data Ltd. All Rights Reserved.

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "TDAnalyticsSettings.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FTDAnalyticsSettingsSmokeTest,
	"TDAnalytics.Settings.Smoke",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FTDAnalyticsSettingsSmokeTest::RunTest(const FString& Parameters)
{
	// Config UObject loads project DefaultEngine.ini on construct, so do not assert empty defaults.
	UTDAnalyticsSettings* Settings = NewObject<UTDAnalyticsSettings>();
	TestNotNull(TEXT("Settings object"), Settings);

	Settings->ServerUrl = TEXT("https://example.com");
	Settings->AppID = TEXT("test-app-id");
	Settings->Mode = TAMode::DEBUG;
	Settings->bEnableLog = true;
	Settings->TimeZone = TEXT("UTC08:00");

	TestEqual(TEXT("ServerUrl writable"), Settings->ServerUrl, FString(TEXT("https://example.com")));
	TestEqual(TEXT("AppID writable"), Settings->AppID, FString(TEXT("test-app-id")));
	TestEqual(TEXT("Mode writable"), Settings->Mode, TAMode::DEBUG);
	TestTrue(TEXT("bEnableLog writable"), Settings->bEnableLog);
	TestEqual(TEXT("TimeZone writable"), Settings->TimeZone, FString(TEXT("UTC08:00")));

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
