// Fill out your copyright notice in the Description page of Project Settings.


#include "TAUserWidget.h"



void UTAUserWidget::Call_TA_Init(){
    UTDAnalytics::Initialize();
    AppID = TEXT("1b1c1fef65e3482bad5c9d0e6a823356");
}


void UTAUserWidget::Call_TA_Set_DistinctID(){
    UTDAnalytics::SetDistinctId("distinct_id_1", AppID);
}


void UTAUserWidget::Call_TA_Get_DistinctID(){
    FString DistinctId = UTDAnalytics::GetDistinctId(AppID);
    UE_LOG(LogTemp, Log, TEXT("get disticid = %s"), *DistinctId);
}


void UTAUserWidget::Call_TA_Login(){
    UTDAnalytics::Login("account_id_1", AppID);
}


void UTAUserWidget::Call_TA_Logout(){
    UTDAnalytics::Logout(AppID);
}


void UTAUserWidget::Call_TA_TrackEvent_Normal(){
    UTDAnalytics::Track("TEST_NORMAL_EVENT", "", AppID);
}


void UTAUserWidget::Call_TA_TrackEvent_With_Prop(){
    UTDAnalytics::Track("TEST_EVENT", TEXT("{\"key1\":\"one\"}"), AppID);
}


void UTAUserWidget::Call_TA_TrackEvent_10000(){
    for (int i = 0; i < 10000; ++i)
    {
        UTDAnalytics::Track(*FString::Printf(TEXT("TEST_EVENT%d"),i), TEXT("{\"key1\":\"one\"}"), AppID);
    }
}


void UTAUserWidget::Call_TA_TrackEvent_Complex(){
    TSharedPtr<FJsonObject> m_DataJsonObject = MakeShareable(new FJsonObject);
    m_DataJsonObject->SetStringField(TEXT("stringKey"), TEXT("string value"));
    m_DataJsonObject->SetBoolField(TEXT("boolKey"), true);
    m_DataJsonObject->SetNumberField(TEXT("intKey"), 12);
    m_DataJsonObject->SetNumberField(TEXT("doubleKey"), 12.34);
    m_DataJsonObject->SetStringField(TEXT("dateKey"), *FString::Printf(TEXT("%s.%03d"), *FDateTime::Now().ToString(), FDateTime::UtcNow().GetMillisecond()));

    TSharedPtr<FJsonObject> m_DataJsonObject2 = MakeShareable(new FJsonObject);
    m_DataJsonObject2->SetStringField(TEXT("stringKey"), TEXT("string value"));
    m_DataJsonObject2->SetBoolField(TEXT("boolKey"), true);
    m_DataJsonObject2->SetNumberField(TEXT("intKey"), 12);
    m_DataJsonObject2->SetNumberField(TEXT("doubleKey"), 12.34);
    m_DataJsonObject2->SetStringField(TEXT("dateKey"), *FString::Printf(TEXT("%s.%03d"), *FDateTime::Now().ToString(), FDateTime::UtcNow().GetMillisecond()));

    TArray< TSharedPtr<FJsonValue> > DataArray;
    TSharedPtr<FJsonValueObject> JsonDataValue = MakeShareable(new FJsonValueObject(m_DataJsonObject2));
    TSharedPtr<FJsonValue> NumberValue = MakeShareable(new FJsonValueNumber(222));
    DataArray.Add(JsonDataValue);
    DataArray.Add(NumberValue);
    TSharedPtr<FJsonValue> StringValue = MakeShareable(new FJsonValueString(TEXT("string value")));
    DataArray.Add(StringValue);
    TSharedPtr<FJsonValue> Date = MakeShareable(new FJsonValueString(*FString::Printf(TEXT("%s.%03d"), *FDateTime::Now().ToString(), FDateTime::UtcNow().GetMillisecond())));
    DataArray.Add(Date);
    m_DataJsonObject->SetArrayField(TEXT("arrayKey"), DataArray);

    m_DataJsonObject->SetObjectField(TEXT("jsonObjKey"), m_DataJsonObject2);

    FString JsonStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonStr);
    FJsonSerializer::Serialize(m_DataJsonObject.ToSharedRef(), Writer);

    UTDAnalytics::Track("TEST_EVENT", JsonStr, AppID);
}


void UTAUserWidget::Call_TA_EnableTimeEvent(){
    UTDAnalytics::TimeEvent("TIME_EVENT1", AppID);
}


void UTAUserWidget::Call_TA_Track_TimeEvent(){
    UTDAnalytics::Track("TIME_EVENT1", "", AppID);
}


void UTAUserWidget::Call_TA_UserSet(){
    UTDAnalytics::UserSet("{\"user_key1\":\"user_value1\"}", AppID);
}


void UTAUserWidget::Call_TA_UserSetOnce(){
    UTDAnalytics::UserSetOnce(TEXT("{\"user_once_key1\":\"user_once_value1\"}"), AppID);
}


void UTAUserWidget::Call_TA_UserAdd(){
    UTDAnalytics::UserAdd("USER_PROP_NUM", 10.9, AppID);
}


void UTAUserWidget::Call_TA_UserDelete(){
    UTDAnalytics::UserDelete(AppID);
}


void UTAUserWidget::Call_TA_UserUnset(){
    UTDAnalytics::UserUnset("user_once_key1", AppID);
}


void UTAUserWidget::Call_TA_UserAppend(){
    UTDAnalytics::UserAppend("{\"appendKey\":[\"apple\", \"ball\"]}", AppID);
}


void UTAUserWidget::Call_TA_UserUniqueAppend(){
    UTDAnalytics::UserUniqueAppend("{\"appendKey\":[\"appleunique\", \"ball\"]}", AppID);
}


void UTAUserWidget::Call_TA_EnableAutoTrack(){
    UTDAnalytics::EnableAutoTrack(AppID);
}


void UTAUserWidget::Call_TA_EnableAutoTrackWithType(){
    TArray<FString> EventTypeList;
    EventTypeList.Emplace(TEXT("ta_app_install"));
    EventTypeList.Emplace(TEXT("ta_app_start"));
    EventTypeList.Emplace(TEXT("ta_app_end"));
    EventTypeList.Emplace(TEXT("ta_app_crash"));
    UTDAnalytics::EnableAutoTrackWithType(EventTypeList, AppID);
}


void UTAUserWidget::Call_TA_EnableAutoTrackWithTypeAndProperties(){
    TArray<FString> EventTypeList;
    EventTypeList.Emplace(TEXT("ta_app_install"));
    EventTypeList.Emplace(TEXT("ta_app_start"));
    EventTypeList.Emplace(TEXT("ta_app_end"));
    EventTypeList.Emplace(TEXT("ta_app_crash"));
    UTDAnalytics::EnableAutoTrackWithTypeAndProperties(EventTypeList, TEXT("{\"autoTrackKey1\":\"autoTrackvalue1\",\"autoTrackKey2\":\"autoTrackvalue2\"}"), AppID);
}


void UTAUserWidget::Call_TA_Get_DeviceID(){
    FString DeviceId = UTDAnalytics::GetDeviceId(AppID);
    UE_LOG(LogTemp, Log, TEXT("get device id = %s"), *DeviceId);
}

// Flush
void UTAUserWidget::Call_TA_Flush(){
    UTDAnalytics::Flush();
}


void UTAUserWidget::Call_TA_CalibrateTime(){
    FDateTime Time = FDateTime::Now();

    int64 Timestamp = Time.ToUnixTimestamp() * 1000 + Time.GetMillisecond();

    UTDAnalytics::CalibrateTime(Timestamp);
}


void UTAUserWidget::Call_TA_CalibrateTimeWithNtp(){

    UTDAnalytics::CalibrateTimeWithNtp("cn.ntp.org.cn");
}


void UTAUserWidget::Call_TA_EnableTracking(){
    UTDAnalytics::EnableTracking(true, AppID);
}


void UTAUserWidget::Call_TA_DisableTracking(){
    UTDAnalytics::EnableTracking(false, AppID);
}


void UTAUserWidget::Call_TA_OptOutTracking(){
    UTDAnalytics::OptOutTracking(AppID);
}


void UTAUserWidget::Call_TA_OptInTracking(){
    UTDAnalytics::OptInTracking(AppID);
}


void UTAUserWidget::Call_TA_Get_PreProp(){
    FString PresetProperties = UTDAnalytics::GetPresetProperties(AppID);
    UE_LOG(LogTemp, Log, TEXT("get PresetProperties = %s"), *PresetProperties);
}


void UTAUserWidget::Call_TA_SetSuperProp(){
    UTDAnalytics::SetSuperProperties("{\"static_super_property1\":\"value1\",\"static_super_property2\":\"value2\"}", AppID);
}


void UTAUserWidget::Call_TA_SetTrackStatus(FString Status){
    UTDAnalytics::SetTrackStatus(Status, AppID);
}


void UTAUserWidget::Call_TA_GetSuperProp(){
    UE_LOG(LogTemp, Log, TEXT("GetSuperProperties = %s"), *UTDAnalytics::GetSuperProperties(AppID));
}


void UTAUserWidget::Call_TA_TrackFirst(){
    UTDAnalytics::TrackFirst(TEXT("TrackFirst"), TEXT("{\"TrackFirst_key1\":\"TrackFirst_value1\"}"));
}


void UTAUserWidget::Call_TA_TrackFirstWithId(){
    UTDAnalytics::TrackFirstWithId(TEXT("TrackFirstWithId"), TEXT("{\"TrackFirst_key2\":\"TrackFirst_value2\"}"), TEXT("FirstCheckId_Test"));
}


void UTAUserWidget::Call_TA_Updateable(){
    UTDAnalytics::TrackUpdate(TEXT("TrackUpdate"), TEXT("{\"TrackUpdate_Key1\":\"TrackUpdate_value1\"}"), TEXT("track_update_event_id"));
}


void UTAUserWidget::Call_TA_TrackOverwrite(){
    UTDAnalytics::TrackOverwrite(TEXT("TrackOverwrite"), TEXT("{\"TrackOverwrite_Key1\":\"TrackOverwrite_value1\"}"), TEXT("track_overwrite_event_id"));
}



FString UTAUserWidget::TDReturnDyldParams() {
    FDateTime TDateTime = FDateTime::Now();
    int64 SecondTimestamp = TDateTime.ToUnixTimestamp();
    int32 MillisecondPart = TDateTime.GetMillisecond();
    FString TimeStr = *FString::Printf(TEXT("%llu"), SecondTimestamp);
    TimeStr += *FString::Printf(TEXT("%lld"), MillisecondPart);
    return "{\"dyld_property1\":\"value1\",\"dyld_property2\":\"" + TimeStr + "\"}";
}


void UTAUserWidget::Call_TA_SetDynamicProp(){
   // UTDAnalytics::dynamicPropertiesMap.insert(std::pair<FString,FString(*)(void)>(AppID ,&TDReturnDyldParams));
    UTDAnalytics::SetDynamicSuperProperties(this, &UTAUserWidget::TDReturnDyldParams, AppID);
}

FString UTAUserWidget::TAAutoTrackProperties(FString AutoTrackEventType, FString Properties)
{
    FDateTime TDateTime = FDateTime::Now();
    int64 SecondTimestamp = TDateTime.ToUnixTimestamp();
    int32 MillisecondPart = TDateTime.GetMillisecond();
    FString TimeStr = *FString::Printf(TEXT("%llu"), SecondTimestamp);
    TimeStr += *FString::Printf(TEXT("%lld"), MillisecondPart);
    return "{\"auto_property1_name\":\"" + AutoTrackEventType + "\",\"auto_property2_time\":\"" + TimeStr + "\"}";
}

void UTAUserWidget::Call_TA_SetAutoTrackEventListener()
{   
    TArray<FString> EventTypeList;
    EventTypeList.Emplace(TEXT("ta_app_install"));
    EventTypeList.Emplace(TEXT("ta_app_start"));
    EventTypeList.Emplace(TEXT("ta_app_end"));
    EventTypeList.Emplace(TEXT("ta_app_crash"));
    UTDAnalytics::SetAutoTrackEventListener(this, &UTAUserWidget::TAAutoTrackProperties, EventTypeList, AppID);
}

void UTAUserWidget::Call_TA_EnableThirdPartySharing()
{
    TArray<FString> EventTypeList;
    EventTypeList.Emplace(TEXT("TAThirdPartyShareTypeNONE"));
    EventTypeList.Emplace(TEXT("TAThirdPartyShareTypeAPPSFLYER"));
    EventTypeList.Emplace(TEXT("TAThirdPartyShareTypeIRONSOURCE"));
    EventTypeList.Emplace(TEXT("TAThirdPartyShareTypeADJUST"));
    EventTypeList.Emplace(TEXT("TAThirdPartyShareTypeBRANCH"));
    EventTypeList.Emplace(TEXT("TAThirdPartyShareTypeTOPON"));
    EventTypeList.Emplace(TEXT("TAThirdPartyShareTypeTRACKING"));
    EventTypeList.Emplace(TEXT("TAThirdPartyShareTypeTRADPLUS"));

    TSharedPtr<FJsonObject> m_DataJsonObject = MakeShareable(new FJsonObject);
    m_DataJsonObject->SetStringField(TEXT("thirdkey1"), TEXT("thirdvalue1"));
    m_DataJsonObject->SetStringField(TEXT("thirdkey2"), TEXT("thirdvalue2"));
    UTDAnalytics::EnableThirdPartySharingWithCustomProperties(EventTypeList, m_DataJsonObject, AppID);
}
