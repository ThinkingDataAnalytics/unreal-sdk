// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TDAnalytics.h"

#include "TAUserWidget.generated.h"


static FString AppID;

/**
 * 
 */
UCLASS()
class TAUNREALDEMO_API UTAUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
    UFUNCTION(BlueprintCallable)
    void Call_TA_Init();
    
    UFUNCTION(BlueprintCallable)
    void Call_TA_Set_DistinctID();
    
    UFUNCTION(BlueprintCallable)
    void Call_TA_Get_DistinctID();
    
    UFUNCTION(BlueprintCallable)
    void Call_TA_Login();
    
    UFUNCTION(BlueprintCallable)
    void Call_TA_Logout();
    
    UFUNCTION(BlueprintCallable)
    void Call_TA_TrackEvent_Normal();
    
    UFUNCTION(BlueprintCallable)
    void Call_TA_TrackEvent_With_Prop();
    
    UFUNCTION(BlueprintCallable)
    void Call_TA_EnableTimeEvent();
    
    UFUNCTION(BlueprintCallable)
    void Call_TA_Track_TimeEvent();
    
    UFUNCTION(BlueprintCallable)
    void Call_TA_UserSet();
    
    UFUNCTION(BlueprintCallable)
    void Call_TA_UserSetOnce();
    
    UFUNCTION(BlueprintCallable)
    void Call_TA_UserAdd();
    
    UFUNCTION(BlueprintCallable)
    void Call_TA_UserDelete();
    
    UFUNCTION(BlueprintCallable)
    void Call_TA_UserUnset();
    
    UFUNCTION(BlueprintCallable)
    void Call_TA_UserAppend();

    UFUNCTION(BlueprintCallable)
    void Call_TA_UserUniqueAppend();
    
    UFUNCTION(BlueprintCallable)
    void Call_TA_EnableAutoTrack();
    
    UFUNCTION(BlueprintCallable)
    void Call_TA_EnableAutoTrackWithType();

    UFUNCTION(BlueprintCallable)
    void Call_TA_EnableAutoTrackWithTypeAndProperties();

    UFUNCTION(BlueprintCallable)
    void Call_TA_Get_DeviceID();
    
    UFUNCTION(BlueprintCallable)
    void Call_TA_Flush();
    
    UFUNCTION(BlueprintCallable)
    void Call_TA_CalibrateTime();
    
    UFUNCTION(BlueprintCallable)
    void Call_TA_CalibrateTimeWithNtp();
    
    UFUNCTION(BlueprintCallable)
    void Call_TA_EnableTracking();
    
    UFUNCTION(BlueprintCallable)
    void Call_TA_DisableTracking();
    
    UFUNCTION(BlueprintCallable)
    void Call_TA_OptOutTracking();
    
    UFUNCTION(BlueprintCallable)
    void Call_TA_OptInTracking();
    
    UFUNCTION(BlueprintCallable)
    void Call_TA_Get_PreProp();
    
    UFUNCTION(BlueprintCallable)
    void Call_TA_SetSuperProp();
    
    UFUNCTION(BlueprintCallable)
    void Call_TA_SetDynamicProp();

    UFUNCTION(BlueprintCallable)
    void Call_TA_GetSuperProp();

    UFUNCTION(BlueprintCallable)
    void Call_TA_TrackFirst();

    UFUNCTION(BlueprintCallable)
    void Call_TA_TrackFirstWithId();

    UFUNCTION(BlueprintCallable)
    void Call_TA_TrackEvent_10000();

    UFUNCTION(BlueprintCallable)
    void Call_TA_TrackEvent_Complex();

    UFUNCTION(BlueprintCallable)
    void Call_TA_Updateable();

    UFUNCTION(BlueprintCallable)
    void Call_TA_TrackOverwrite();

    UFUNCTION(BlueprintCallable)
    void Call_TA_SetTrackStatus(FString Status);

    UFUNCTION(BlueprintCallable)
    FString TDReturnDyldParams();

    UFUNCTION(BlueprintCallable)
    FString TAAutoTrackProperties(FString AutoTrackEventType, FString Properties);

    UFUNCTION(BlueprintCallable)
    void Call_TA_SetAutoTrackEventListener();

    UFUNCTION(BlueprintCallable)
    void Call_TA_EnableThirdPartySharing();
};