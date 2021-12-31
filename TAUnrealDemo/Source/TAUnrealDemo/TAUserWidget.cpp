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

// 启用自动采集事件
void UTAUserWidget::Call_TA_EnableAutoTrack(){
    UThinkingAnalytics::EnableAutoTrack(AppID);
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
    int64 Timestamp = Time.ToUnixTimestamp();
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
static FString TDReturnDyldParams() {
   return "{\"dyld_property1\":\"value1\",\"dyld_property2\":\"value2\"}";
}

// 设置动态公共属性
void UTAUserWidget::Call_TA_SetDynamicProp(){
   UThinkingAnalytics::dynamicPropertiesMap.insert(std::pair<FString,FString(*)(void)>(AppID ,&TDReturnDyldParams));
}

