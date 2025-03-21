
#include "FDLLLoaderRunnable.h"
#include <Common/TALog.h>

FDLLLoaderRunnable::FDLLLoaderRunnable(TDWindowsSetting InSetting, LoadDllCallBack InCallback)
    :Setting(InSetting),Callback(InCallback)
{
}

bool FDLLLoaderRunnable::Init()
{
    return true;
}

uint32 FDLLLoaderRunnable::Run()
{
#if WITH_EDITOR
    FString DllPath = FPaths::Combine(FPaths::ProjectDir(), TEXT("Binaries/Win64/CppWrapper.dll"));
#else
    FString DllPath = FPaths::Combine(FPaths::LaunchDir(), TEXT("CppWrapper.dll"));
#endif
    void* DllHandle = FPlatformProcess::GetDllHandle(*DllPath);
    if (DllHandle != nullptr && Callback != nullptr) {
        Callback(DllHandle,Setting);
    }
    return 0;
}

void FDLLLoaderRunnable::Stop()
{
}

void FDLLLoaderRunnable::Exit()
{
    delete this;
}

void FDLLLoaderRunnable::LoadDLLAsync(const TDWindowsSetting Setting, LoadDllCallBack InCallback)
{
    FDLLLoaderRunnable* Runnable = new FDLLLoaderRunnable(Setting, InCallback);
    FRunnableThread::Create(Runnable, TEXT("DLLLoaderThread"));
}