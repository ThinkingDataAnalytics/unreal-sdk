// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TDAnalyticsSettings.h"
#include "TDAnalyticsWindow.h"

typedef void(*LoadDllCallBack)(void*, const TDWindowsSetting);
/**
 * 
 */
class FDLLLoaderRunnable : public FRunnable
{
private:
    LoadDllCallBack Callback;
    const TDWindowsSetting Setting;
public:
    /**
     * Constructor
     * @param InDllPath Path to the DLL file.
     */
    FDLLLoaderRunnable(const TDWindowsSetting InSetting,LoadDllCallBack InCallback);
    //FDLLLoaderRunnable();
    /**
     * Initializes the runnable object.
     * @return true if initialization was successful, false otherwise.
     */
    virtual bool Init() override;

    /**
     * Main execution function for the runnable object, called once the thread is started.
     * @return The exit code of the thread.
     */
    virtual uint32 Run() override;

    /**
     * Stops the runnable object. This is called if the thread needs to be aborted.
     */
    virtual void Stop() override;

    /**
     * Clean up after the runnable object has stopped running.
     */
    virtual void Exit() override;

    /**
     * Static method to start loading a DLL asynchronously.
     * @param DllPath Path to the DLL file.
     * @return Pointer to the thread running the DLL loading operation.
     */
    /*static FRunnableThread* LoadDLLAsync(const UTDAnalyticsSettings* DefaultSettings, const FString& InVersion, LoadDllCallBack InCallback);*/
    static void LoadDLLAsync(const TDWindowsSetting Setting, LoadDllCallBack InCallback);
};
