// Copyright 2020 Thinking Data Ltd. All Rights Reserved.

// The Editor test module is not packaged on Android. Keep device tests in the Demo.
#if TDANALYTICS_ANDROID_TESTS && WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "TDAnalytics.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

namespace
{
    const FString UnicodeTestAppId = TEXT("tdanalytics-unicode-regression");

    struct FDistinctIdCase
    {
        const TCHAR* Name;
        const TCHAR* Value;
    };

    const FDistinctIdCase DistinctIdCases[] = {
        {TEXT("ASCII"), TEXT("1A_ascii")},
        {TEXT("Chinese"), TEXT("\u6570\u6570")},
        {TEXT("Emoji"), TEXT("\U0001F60D")},
        {TEXT("TrailingSpace"), TEXT("1A_ascii ")},
        {TEXT("LeadingAndInternalSpaces"), TEXT(" 1A_ \u6570 \u6570 ")},
        {TEXT("ReportedId"), TEXT("1A_\U0001F60D\u3002\u6570\u6570 ")}
    };

    void InitializeUnicodeTestInstance()
    {
        UTDAnalytics::InitializeInstance(UnicodeTestAppId, TEXT("http://127.0.0.1:1"), TAMode::NORMAL, false, TEXT("UTC"));
        // SAVE_ONLY permits local ID/property updates while preventing uploads.
        UTDAnalytics::SetTrackStatus(TEXT("SAVE_ONLY"), UnicodeTestAppId);
    }
}

IMPLEMENT_COMPLEX_AUTOMATION_TEST(
    FTDAnalyticsAndroidDistinctIdTest,
    "TDAnalytics.Android.DistinctIdRoundTrip",
    EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

void FTDAnalyticsAndroidDistinctIdTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
    for (const FDistinctIdCase& TestCase : DistinctIdCases)
    {
        OutBeautifiedNames.Add(TestCase.Name);
        OutTestCommands.Add(TestCase.Name);
    }
}

bool FTDAnalyticsAndroidDistinctIdTest::RunTest(const FString& Parameters)
{
    const TCHAR* ExpectedValue = nullptr;
    for (const FDistinctIdCase& TestCase : DistinctIdCases)
    {
        if (Parameters == TestCase.Name)
        {
            ExpectedValue = TestCase.Value;
            break;
        }
    }
    if (!ExpectedValue)
    {
        AddError(TEXT("Unknown distinct ID test case"));
        return false;
    }
    const FString Expected(ExpectedValue);

    InitializeUnicodeTestInstance();
    const FString OriginalId = UTDAnalytics::GetDistinctId(UnicodeTestAppId);

    UTDAnalytics::SetDistinctId(Expected, UnicodeTestAppId);
    const FString Actual = UTDAnalytics::GetDistinctId(UnicodeTestAppId);
    TestEqual(TEXT("Distinct ID preserves all Unicode characters and spaces"), Actual, Expected);
    TestEqual(TEXT("Distinct ID length is unchanged"), Actual.Len(), Expected.Len());

    UTDAnalytics::SetDistinctId(OriginalId, UnicodeTestAppId);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FTDAnalyticsAndroidSuperPropertiesTest,
    "TDAnalytics.Android.SuperPropertiesUnicode",
    EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FTDAnalyticsAndroidSuperPropertiesTest::RunTest(const FString& Parameters)
{
    InitializeUnicodeTestInstance();
    const FString OriginalProperties = UTDAnalytics::GetSuperProperties(UnicodeTestAppId);

    // Raw Unicode exercises both the setter and getter's JNI conversions.
    UTDAnalytics::SetSuperProperties(TEXT("{\"unicode\":\"1A_\U0001F60D\u3002\u6570\u6570 \"}"), UnicodeTestAppId);
    const double StartTime = FPlatformTime::Seconds();
    // The Android SDK writes super properties on its worker thread.
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([this, OriginalProperties, StartTime]()
    {
        const FString Actual = UTDAnalytics::GetSuperProperties(UnicodeTestAppId);
        TSharedPtr<FJsonObject> Properties;
        const bool bParsed = FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Actual), Properties) && Properties.IsValid();
        FString Value;
        const bool bHasValue = bParsed && Properties->TryGetStringField(TEXT("unicode"), Value);
        if (!bHasValue && FPlatformTime::Seconds() - StartTime < 5.0)
        {
            return false;
        }

        TestTrue(TEXT("Super properties remain valid JSON"), bParsed);
        TestTrue(TEXT("Unicode property is present"), bHasValue);
        TestEqual(TEXT("Super properties preserve Chinese, emoji and trailing space"), Value, FString(TEXT("1A_\U0001F60D\u3002\u6570\u6570 ")));
        UTDAnalytics::ClearSuperProperties(UnicodeTestAppId);
        UTDAnalytics::SetSuperProperties(OriginalProperties, UnicodeTestAppId);
        return true;
    }));
    return true;
}

#endif // TDANALYTICS_ANDROID_TESTS && WITH_DEV_AUTOMATION_TESTS
