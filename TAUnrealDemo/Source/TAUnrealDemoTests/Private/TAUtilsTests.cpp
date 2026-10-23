// Copyright 2021 ThinkingData. All Rights Reserved.

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Common/TAUtils.h"
#include "Misc/Base64.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FTAUtilsIsInvalidNameTest,
	"TDAnalytics.Utils.IsInvalidName",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FTAUtilsIsInvalidNameTest::RunTest(const FString& Parameters)
{
	TestFalse(TEXT("Event should be valid"), FTAUtils::IsInvalidName(TEXT("Event")));
	TestFalse(TEXT("single letter should be valid"), FTAUtils::IsInvalidName(TEXT("a")));
	TestFalse(TEXT("Login_01 should be valid"), FTAUtils::IsInvalidName(TEXT("Login_01")));

	TestTrue(TEXT("empty should be invalid"), FTAUtils::IsInvalidName(TEXT("")));
	TestTrue(TEXT("leading digit should be invalid"), FTAUtils::IsInvalidName(TEXT("1bad")));
	TestTrue(TEXT("dash should be invalid"), FTAUtils::IsInvalidName(TEXT("has-dash")));

	FString TooLong = TEXT("A");
	for (int32 Index = 0; Index < 50; ++Index)
	{
		TooLong += TEXT("x");
	}
	TestTrue(TEXT("name longer than 50 should be invalid"), FTAUtils::IsInvalidName(TooLong));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FTAUtilsGetZoneOffsetWithTimeZoneTest,
	"TDAnalytics.Utils.GetZoneOffsetWithTimeZone",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FTAUtilsGetZoneOffsetWithTimeZoneTest::RunTest(const FString& Parameters)
{
	TestTrue(TEXT("UTC08:00"), FMath::IsNearlyEqual(FTAUtils::GetZoneOffsetWithTimeZone(TEXT("UTC08:00")), 8.0f));
	TestTrue(TEXT("GMT-5:30"), FMath::IsNearlyEqual(FTAUtils::GetZoneOffsetWithTimeZone(TEXT("GMT-5:30")), -5.5f));

	const float EmptyFallback = FTAUtils::GetZoneOffsetWithTimeZone(TEXT(""));
	TestTrue(TEXT("empty timezone fallback should be finite"), FMath::IsFinite(EmptyFallback));

	const float SignedFallback = FTAUtils::GetZoneOffsetWithTimeZone(TEXT("UTC+08:00"));
	TestTrue(TEXT("UTC+08:00 falls back to finite offset"), FMath::IsFinite(SignedFallback));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FTAUtilsFormatTimeTest,
	"TDAnalytics.Utils.FormatTime",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FTAUtilsFormatTimeTest::RunTest(const FString& Parameters)
{
	const FDateTime DateTime(2021, 1, 1, 12, 12, 21, 2);
	TestEqual(TEXT("FormatTime"), FTAUtils::FormatTime(DateTime), FString(TEXT("2021-01-01 12:12:21.002")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FTAUtilsEncodeDataTest,
	"TDAnalytics.Utils.EncodeData",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FTAUtilsEncodeDataTest::RunTest(const FString& Parameters)
{
	const FString Encoded = FTAUtils::EncodeData(TEXT("{\"a\":1}"));
	TestFalse(TEXT("EncodeData should return non-empty"), Encoded.IsEmpty());

	TArray<uint8> DecodedBytes;
	const bool bDecoded = FBase64::Decode(Encoded, DecodedBytes);
	TestTrue(TEXT("EncodeData result should be valid Base64"), bDecoded);
	TestTrue(TEXT("decoded gzip payload should be non-empty"), DecodedBytes.Num() > 0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FTAUtilsMergePropertiesWithOffsetTest,
	"TDAnalytics.Utils.MergePropertiesWithOffset",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FTAUtilsMergePropertiesWithOffsetTest::RunTest(const FString& Parameters)
{
	const FString First = TEXT("{\"k\":0,\"extra\":1}");
	const FString Second = TEXT("{\"k\":1}");
	const FString Merged = FTAUtils::MergePropertiesWithOffset(First, Second, 8.0f);

	TestFalse(TEXT("merge result should be non-empty"), Merged.IsEmpty());

	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Merged);
	const bool bParsed = FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid();
	TestTrue(TEXT("merge result should be valid JSON object"), bParsed);

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
