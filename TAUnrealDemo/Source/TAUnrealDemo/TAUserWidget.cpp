// Fill out your copyright notice in the Description page of Project Settings.


#include "TAUserWidget.h"


// 初始化 SDK
void UTAUserWidget::Call_TA_Init(){
    UThinkingAnalytics::Initialize();
    AppID = TEXT("1b1c1fef65e3482bad5c9d0e6a823356");
}

// 设置访客id
void UTAUserWidget::Call_TA_Set_DistinctID(){
    UThinkingAnalytics::Identify("distinct_id_1", AppID);
}

// 获得访客id
void UTAUserWidget::Call_TA_Get_DistinctID(){
    FString DistinctId = UThinkingAnalytics::GetDistinctId(AppID);
    UE_LOG(LogTemp, Log, TEXT("获得访客id = %s"), *DistinctId);
}

// 设置账号id
void UTAUserWidget::Call_TA_Login(){
    UThinkingAnalytics::Login("account_id_1", AppID);
}

// 清除账号id
void UTAUserWidget::Call_TA_Logout(){
    UThinkingAnalytics::Logout(AppID);
}

// 上报一个不带属性事件
void UTAUserWidget::Call_TA_TrackEvent_Normal(){
    UThinkingAnalytics::Track("TEST_NORMAL_EVENT", "", AppID);
}

// 上报一个带属性事件
void UTAUserWidget::Call_TA_TrackEvent_With_Prop(){
    UThinkingAnalytics::Track("TEST_EVENT", TEXT("{\"key1\":\"打劫\"}"), AppID);
}

// 循环上报10000条
void UTAUserWidget::Call_TA_TrackEvent_10000(){
    for (int i = 0; i < 10000; ++i)
    {
        UThinkingAnalytics::Track(*FString::Printf(TEXT("TEST_EVENT%d"),i), TEXT("{\"key1\":\"打劫\"}"), AppID);
    }
}

// 上报复杂类型数据
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

    UThinkingAnalytics::Track("TEST_EVENT", JsonStr, AppID);
}

// 调用 TimeEvent 开启对 TIME_EVENT1 事件的计时
void UTAUserWidget::Call_TA_EnableTimeEvent(){
    UThinkingAnalytics::TimeEvent("TIME_EVENT1", AppID);
}

// 通过 Track 上传 TIME_EVENT 事件时，会在属性中添加 #duration 属性
void UTAUserWidget::Call_TA_Track_TimeEvent(){
    UThinkingAnalytics::Track("TIME_EVENT1", "", AppID);
}

// 设置用户属性
void UTAUserWidget::Call_TA_UserSet(){
    UThinkingAnalytics::UserSet("{\"user_key1\":\"user_value1\"}", AppID);
}

// 只设置一次的用户属性
void UTAUserWidget::Call_TA_UserSetOnce(){
    UThinkingAnalytics::UserSetOnce(TEXT("{\"user_once_key1\":\"user_once_value1\"}"), AppID);
}

// 数值属性累加
void UTAUserWidget::Call_TA_UserAdd(){
    UThinkingAnalytics::UserAdd("USER_PROP_NUM", 10.9, AppID);
}

// 删除某个用户
void UTAUserWidget::Call_TA_UserDelete(){
    UThinkingAnalytics::UserDelete(AppID);
}

// 删除已设置的属性
void UTAUserWidget::Call_TA_UserUnset(){
    UThinkingAnalytics::UserUnset("user_once_key1", AppID);
}

// 为List类型用户属性追加item
void UTAUserWidget::Call_TA_UserAppend(){
    UThinkingAnalytics::UserAppend("{\"appendKey\":[\"apple\", \"ball\"]}", AppID);
}

// 为List类型用户属性追加item
void UTAUserWidget::Call_TA_UserUniqueAppend(){
    UThinkingAnalytics::UserUniqueAppend("{\"appendKey\":[\"appleunique\", \"ball\"]}", AppID);
}

// 启用自动采集事件
void UTAUserWidget::Call_TA_EnableAutoTrack(){
    UThinkingAnalytics::EnableAutoTrack(AppID);
}

// 启用自动采集事件 + 事件列表
void UTAUserWidget::Call_TA_EnableAutoTrackWithType(){
    TArray<FString> EventTypeList;
    EventTypeList.Emplace(TEXT("ta_app_install"));
    EventTypeList.Emplace(TEXT("ta_app_start"));
    EventTypeList.Emplace(TEXT("ta_app_end"));
    EventTypeList.Emplace(TEXT("ta_app_crash"));
    UThinkingAnalytics::EnableAutoTrackWithType(EventTypeList, AppID);
}

// 启用自动采集事件 + 事件列表 + 事件属性
void UTAUserWidget::Call_TA_EnableAutoTrackWithTypeAndProperties(){
    TArray<FString> EventTypeList;
    EventTypeList.Emplace(TEXT("ta_app_install"));
    EventTypeList.Emplace(TEXT("ta_app_start"));
    EventTypeList.Emplace(TEXT("ta_app_end"));
    EventTypeList.Emplace(TEXT("ta_app_crash"));
    UThinkingAnalytics::EnableAutoTrackWithTypeAndProperties(EventTypeList, TEXT("{\"autoTrackKey1\":\"autoTrackvalue1\",\"autoTrackKey2\":\"autoTrackvalue2\"}"), AppID);
}

// 获取设备id
void UTAUserWidget::Call_TA_Get_DeviceID(){
    FString DeviceId = UThinkingAnalytics::GetDeviceId(AppID);
    UE_LOG(LogTemp, Log, TEXT("获取设备id = %s"), *DeviceId);
}

// Flush
void UTAUserWidget::Call_TA_Flush(){
    UThinkingAnalytics::Flush();
}

// 时间校准
void UTAUserWidget::Call_TA_CalibrateTime(){
    FDateTime Time = FDateTime::Now();
    //获取时间戳
    int64 Timestamp = Time.ToUnixTimestamp() * 1000 + Time.GetMillisecond();
    // 入参为当前 unix 时间戳，单位为毫秒
    UThinkingAnalytics::CalibrateTime(Timestamp);
}

// Ntp时间校准
void UTAUserWidget::Call_TA_CalibrateTimeWithNtp(){
    //入参为Ntp服务器地址
    UThinkingAnalytics::CalibrateTimeWithNtp("");
}

// 从暂停恢复上报
void UTAUserWidget::Call_TA_EnableTracking(){
    UThinkingAnalytics::EnableTracking(true, AppID);
}

// 暂停上报
void UTAUserWidget::Call_TA_DisableTracking(){
    UThinkingAnalytics::EnableTracking(false, AppID);
}

// 停止上报
void UTAUserWidget::Call_TA_OptOutTracking(){
    UThinkingAnalytics::OptOutTracking(AppID);
}

// 从停止恢复上报
void UTAUserWidget::Call_TA_OptInTracking(){
    UThinkingAnalytics::OptInTracking(AppID);
}

// 获取预置属性
void UTAUserWidget::Call_TA_Get_PreProp(){
    FString PresetProperties = UThinkingAnalytics::GetPresetProperties(AppID);
    UE_LOG(LogTemp, Log, TEXT("获取预置属性 = %s"), *PresetProperties);
}

// 设置公共属性
void UTAUserWidget::Call_TA_SetSuperProp(){
    UThinkingAnalytics::SetSuperProperties("{\"static_super_property1\":\"value1\",\"static_super_property2\":\"value2\"}", AppID);
}

// 设置采集模式SAVE_ONLY
void UTAUserWidget::Call_TA_SetTrackStatus(FString Status){
    UThinkingAnalytics::SetTrackStatus(Status, AppID);
}

// 获取公共属性
void UTAUserWidget::Call_TA_GetSuperProp(){
    UE_LOG(LogTemp, Log, TEXT("获取预置属性 = %s"), *UThinkingAnalytics::GetSuperProperties(AppID));
}

// 首次事件
void UTAUserWidget::Call_TA_TrackFirst(){
    UThinkingAnalytics::TrackFirst(TEXT("TrackFirst"), TEXT("{\"TrackFirst_key1\":\"TrackFirst_value1\"}"));
}

// 首次事件withId
void UTAUserWidget::Call_TA_TrackFirstWithId(){
    UThinkingAnalytics::TrackFirstWithId(TEXT("TrackFirstWithId"), TEXT("{\"TrackFirst_key2\":\"TrackFirst_value2\"}"), TEXT("FirstCheckId_Test"));
}

// 可更新
void UTAUserWidget::Call_TA_Updateable(){
    UThinkingAnalytics::TrackUpdate(TEXT("TrackUpdate"), TEXT("{\"TrackUpdate_Key1\":\"TrackUpdate_value1\"}"), TEXT("track_update_event_id"));
}

// 可重写
void UTAUserWidget::Call_TA_TrackOverwrite(){
    UThinkingAnalytics::TrackOverwrite(TEXT("TrackOverwrite"), TEXT("{\"TrackOverwrite_Key1\":\"TrackOverwrite_value1\"}"), TEXT("track_overwrite_event_id"));
}


// 定义动态公共属性函数
FString UTAUserWidget::TDReturnDyldParams() {
    FDateTime TDateTime = FDateTime::Now();
    int64 SecondTimestamp = TDateTime.ToUnixTimestamp();
    int32 MillisecondPart = TDateTime.GetMillisecond();
    FString TimeStr = *FString::Printf(TEXT("%llu"), SecondTimestamp);
    TimeStr += *FString::Printf(TEXT("%lld"), MillisecondPart);
    return "{\"dyld_property1\":\"value1\",\"dyld_property2\":\"" + TimeStr + "\"}";
}

// 设置动态公共属性
void UTAUserWidget::Call_TA_SetDynamicProp(){
   // UThinkingAnalytics::dynamicPropertiesMap.insert(std::pair<FString,FString(*)(void)>(AppID ,&TDReturnDyldParams));
    UThinkingAnalytics::SetDynamicSuperProperties(this, &UTAUserWidget::TDReturnDyldParams, AppID);
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
    UThinkingAnalytics::SetAutoTrackEventListener(this, &UTAUserWidget::TAAutoTrackProperties, EventTypeList, AppID);
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
    UThinkingAnalytics::EnableThirdPartySharingWithCustomProperties(EventTypeList, m_DataJsonObject, AppID);
}