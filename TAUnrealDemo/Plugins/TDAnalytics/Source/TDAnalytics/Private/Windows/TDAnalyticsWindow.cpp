// Fill out your copyright notice in the Description page of Project Settings.


#include "Windows/TDAnalyticsWindow.h"
#include <Common/TALog.h>
#include "HAL/Platform.h"
#include "FDLLLoaderRunnable.h"

UTDAnalyticsWindow::UTDAnalyticsWindow(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

UTDAnalyticsWindow::~UTDAnalyticsWindow()
{
}

static void* TDDllHandle;
static FThreadSafeBool isInitSuccess = false;

TQueue<TFunction<void()>> FunctionQueue;

void DLLLoadedCallback(void* DllHandle, const TDWindowsSetting Setting)
{
    TDDllHandle = DllHandle;
    typedef void(*SetCustomLibInfoWrapperFunc)(const char*, const char*);
    SetCustomLibInfoWrapperFunc SetCustomLibInfo = (SetCustomLibInfoWrapperFunc)FPlatformProcess::GetDllExport(TDDllHandle, TEXT("SetCustomLibInfoWrapper"));
    if (SetCustomLibInfo) {
        FString libName = "Unreal";
        SetCustomLibInfo(TCHAR_TO_UTF8(*libName), TCHAR_TO_UTF8(*Setting.Version));
    }
    if (Setting.EnableLog) {
        FTALog::SetEnableLog(true);
        typedef void(*EnableLogTypeWrapperFunc)(int);
        EnableLogTypeWrapperFunc EnableLogType = (EnableLogTypeWrapperFunc)FPlatformProcess::GetDllExport(TDDllHandle, TEXT("EnableLogTypeWrapper"));
        if (EnableLogType) {
            EnableLogType(3);
        }
    }
    UTDAnalyticsWindow::InitWithConfig(Setting);
}

void UTDAnalyticsWindow::Initialize(const UTDAnalyticsSettings* DefaultSettings, FString Version)
{
    if (!isInitSuccess) {
        TDWindowsSetting Setting;
        Setting.AppId = DefaultSettings->AppID;
        Setting.ServerUrl = DefaultSettings->ServerUrl;
        Setting.Mode = DefaultSettings->Mode;
        Setting.EnableLog = DefaultSettings->bEnableLog;
        Setting.Version = Version;
        Setting.EnableEncrypt = DefaultSettings->bEnableEncrypt;
        Setting.EncryptVersion = DefaultSettings->EncryptVersion;
        Setting.EncryptPublicKey = DefaultSettings->EncryptPublicKey;
        FDLLLoaderRunnable::LoadDLLAsync(Setting, DLLLoadedCallback);
    }
    //TSharedPtr<FDLLLoaderRunnable> Runnable = MakeShareable(new FDLLLoaderRunnable());
    //TSharedPtr<FRunnableThread>(FRunnableThread::Create(Runnable.Get(), TEXT("DLLLoaderThread")));
}

void UTDAnalyticsWindow::Initialize(const FString& AppID, const FString& ServerUrl, TAMode Mode, bool EnableLog, const FString& TimeZone, FString Version) {
    if (!isInitSuccess) {
        TDWindowsSetting Setting;
        Setting.AppId = AppID;
        Setting.ServerUrl = ServerUrl;
        Setting.Mode = Mode;
        Setting.EnableLog = EnableLog;
        Setting.Version = Version;
        FDLLLoaderRunnable::LoadDLLAsync(Setting, DLLLoadedCallback);
    }
}

void UTDAnalyticsWindow::InitWithConfig(const TDWindowsSetting Setting) {
    typedef bool(*InitWrapperFunc)(const char*, const char*, bool, int, const char*, int, int, int, const char*);
    InitWrapperFunc InitWrapper = (InitWrapperFunc)FPlatformProcess::GetDllExport(TDDllHandle, TEXT("InitWrapper"));
    if (InitWrapper) {
        FString DataPath = "";
        int mode = 0;
        if (Setting.Mode == TAMode::DEBUG) {
            mode = 1;
        }
        else if (Setting.Mode == TAMode::DEBUG_ONLY) {
            mode = 2;
        }
        bool result = InitWrapper(TCHAR_TO_UTF8(*Setting.AppId), TCHAR_TO_UTF8(*Setting.ServerUrl), Setting.EnableEncrypt, Setting.EncryptVersion, TCHAR_TO_UTF8(*Setting.EncryptPublicKey), mode, 0, 0, TCHAR_TO_UTF8(*DataPath));
        if (result) {
            FTALog::Info(CUR_LOG_POSITION, TEXT("UTDAnalytics Windows Initialize Success !"));
            isInitSuccess = true;
            TFunction<void()> Func;
            while (FunctionQueue.Dequeue(Func))
            {
                Func();
            }
        }
    }
}

void UTDAnalyticsWindow::SetDistinctId(const FString& DistinctID) {
    if (isInitSuccess) {
        typedef void(*IdentifyFunc)(const char*);
        IdentifyFunc IdentifyWrapper = (IdentifyFunc)FPlatformProcess::GetDllExport(TDDllHandle, TEXT("IdentifyWrapper"));
        if (IdentifyWrapper) {
            IdentifyWrapper(TCHAR_TO_UTF8(*DistinctID));
            FTALog::Info(CUR_LOG_POSITION, TEXT("UTDAnalytics Set Distinct Id = ")+ DistinctID);
        }
    }
    else {
        FunctionQueue.Enqueue([DistinctID]() {
            UTDAnalyticsWindow::SetDistinctId(DistinctID);
        });
    }
}

void DeleteChar(char* c) {
    typedef void(*DeleteCharFunc)(const char*);
    DeleteCharFunc DeleteCharWrapper = (DeleteCharFunc)FPlatformProcess::GetDllExport(TDDllHandle, TEXT("DeleteCharWrapper"));
    if (DeleteCharWrapper) {
        DeleteCharWrapper(c);
    }

}

FString UTDAnalyticsWindow::GetDistinctId() {
    if (isInitSuccess) {
        typedef char*(*GetDistinctIdFunc)();
        GetDistinctIdFunc GetDistinctIdWrapper = (GetDistinctIdFunc)FPlatformProcess::GetDllExport(TDDllHandle, TEXT("GetDistinctIdWrapper"));
        if (GetDistinctIdWrapper) {
            char* DistinctId = GetDistinctIdWrapper();
            FString FDistinctID = FString(UTF8_TO_TCHAR(DistinctId));
            DeleteChar(DistinctId);
            return FDistinctID;
        }
    }
    return TEXT("");
}

void UTDAnalyticsWindow::Login(const FString& AccountID) {
    if (isInitSuccess) {
        typedef void(*LoginFunc)(const char*);
        LoginFunc LoginWrapper = (LoginFunc)FPlatformProcess::GetDllExport(TDDllHandle, TEXT("LoginWrapper"));
        if (LoginWrapper) {
            LoginWrapper(TCHAR_TO_UTF8(*AccountID));
            FTALog::Info(CUR_LOG_POSITION, TEXT("UTDAnalytics Set Account Id = ") + AccountID);
        }
    }
    else {
        FunctionQueue.Enqueue([AccountID]() {
            UTDAnalyticsWindow::Login(AccountID);
        });
    }
}

void UTDAnalyticsWindow::Logout() {
    if (isInitSuccess) {
        typedef void(*LogoutFunc)();
        LogoutFunc LogoutWrapper = (LogoutFunc)FPlatformProcess::GetDllExport(TDDllHandle, TEXT("LogoutWrapper"));
        if (LogoutWrapper) {
            LogoutWrapper();
            FTALog::Info(CUR_LOG_POSITION, TEXT("UTDAnalytics Logout Called"));
        }
    }
    else {
        FunctionQueue.Enqueue([]() {
            UTDAnalyticsWindow::Logout();
        });
    }
}

void UTDAnalyticsWindow::Track(const FString& EventName, const FString& Properties) {
    if (isInitSuccess) {
        typedef void(*TrackFunc)(const char*, const char*);
        TrackFunc TrackWrapper = (TrackFunc)FPlatformProcess::GetDllExport(TDDllHandle, TEXT("TrackWrapper"));
        if (TrackWrapper) {
            if (Properties.IsEmpty()) {
                TrackWrapper(TCHAR_TO_UTF8(*EventName), TCHAR_TO_UTF8(TEXT("{}")));
            }
            else {
                TrackWrapper(TCHAR_TO_UTF8(*EventName), TCHAR_TO_UTF8(*Properties));
            }
            FTALog::Info(CUR_LOG_POSITION, TEXT("UTDAnalytics Track Called "));
        }
    }
    else {
        FunctionQueue.Enqueue([EventName, Properties]() {
            UTDAnalyticsWindow::Track(EventName, Properties);
        });
    }
}

void UTDAnalyticsWindow::TimeEvent(const FString& EventName) {
    if (isInitSuccess) {
        typedef void(*TimeEventFunc)(const char*);
        TimeEventFunc TimeEventWrapper = (TimeEventFunc)FPlatformProcess::GetDllExport(TDDllHandle, TEXT("TimeEventWrapper"));
        if (TimeEventWrapper) {
            TimeEventWrapper(TCHAR_TO_UTF8(*EventName));
            FTALog::Info(CUR_LOG_POSITION, TEXT("UTDAnalytics TimeEvent Called "));
        }
    }
    else {
        FunctionQueue.Enqueue([EventName]() {
            UTDAnalyticsWindow::TimeEvent(EventName);
        });
    }
}

void UTDAnalyticsWindow::UserSet(const FString& Properties) {
    if (isInitSuccess) {
        typedef void(*UserSetFunc)(const char*);
        UserSetFunc UserSetWrapper = (UserSetFunc)FPlatformProcess::GetDllExport(TDDllHandle, TEXT("UserSetWrapper"));
        if (UserSetWrapper) {
            UserSetWrapper(TCHAR_TO_UTF8(*Properties));
            FTALog::Info(CUR_LOG_POSITION, TEXT("UTDAnalytics UserSet Called "));
        }
    }
    else {
        FunctionQueue.Enqueue([Properties]() {
            UTDAnalyticsWindow::UserSet(Properties);
        });
    }
}

void UTDAnalyticsWindow::UserSetOnce(const FString& Properties) {
    if (isInitSuccess) {
        typedef void(*UserSetOnceFunc)(const char*);
        UserSetOnceFunc UserSetOnceWrapper = (UserSetOnceFunc)FPlatformProcess::GetDllExport(TDDllHandle, TEXT("UserSetOnceWrapper"));
        if (UserSetOnceWrapper) {
            UserSetOnceWrapper(TCHAR_TO_UTF8(*Properties));
            FTALog::Info(CUR_LOG_POSITION, TEXT("UTDAnalytics UserSetOnce Called "));
        }
    }
    else {
        FunctionQueue.Enqueue([Properties]() {
            UTDAnalyticsWindow::UserSetOnce(Properties);
        });
    }
}

void UTDAnalyticsWindow::UserAdd(const FString& Properties) {
    if (isInitSuccess) {
        typedef void(*UserAddFunc)(const char*);
        UserAddFunc UserAddWrapper = (UserAddFunc)FPlatformProcess::GetDllExport(TDDllHandle, TEXT("UserAddWrapper"));
        if (UserAddWrapper) {
            UserAddWrapper(TCHAR_TO_UTF8(*Properties));
            FTALog::Info(CUR_LOG_POSITION, TEXT("UTDAnalytics UserAdd Called "));
        }
    }
    else {
        FunctionQueue.Enqueue([Properties]() {
            UTDAnalyticsWindow::UserAdd(Properties);
        });
    }
}

void UTDAnalyticsWindow::UserUnset(const FString& Property) {
    if (isInitSuccess) {
        typedef void(*UserUnsetFunc)(const char*);
        UserUnsetFunc UserUnsetWrapper = (UserUnsetFunc)FPlatformProcess::GetDllExport(TDDllHandle, TEXT("UserUnsetWrapper"));
        if (UserUnsetWrapper) {
            UserUnsetWrapper(TCHAR_TO_UTF8(*Property));
            FTALog::Info(CUR_LOG_POSITION, TEXT("UTDAnalytics UserUnset Called "));
        }
    }
    else {
        FunctionQueue.Enqueue([Property]() {
            UTDAnalyticsWindow::UserUnset(Property);
        });
    }
}

void UTDAnalyticsWindow::UserDelete() {
    if (isInitSuccess) {
        typedef void(*UserDeleteFunc)();
        UserDeleteFunc UserDeleteWrapper = (UserDeleteFunc)FPlatformProcess::GetDllExport(TDDllHandle, TEXT("UserDeleteWrapper"));
        if (UserDeleteWrapper) {
            UserDeleteWrapper();
            FTALog::Info(CUR_LOG_POSITION, TEXT("UTDAnalytics UserDelete Called "));
        }
    }
    else {
        FunctionQueue.Enqueue([]() {
            UTDAnalyticsWindow::UserDelete();
        });
    }
}

void UTDAnalyticsWindow::UserAppend(const FString& Properties) {
    if (isInitSuccess) {
        typedef void(*UserAppendFunc)(const char*);
        UserAppendFunc UserAppendWrapper = (UserAppendFunc)FPlatformProcess::GetDllExport(TDDllHandle, TEXT("UserAppendWrapper"));
        if (UserAppendWrapper) {
            UserAppendWrapper(TCHAR_TO_UTF8(*Properties));
            FTALog::Info(CUR_LOG_POSITION, TEXT("UTDAnalytics UserAppend Called "));
        }
    }
    else {
        FunctionQueue.Enqueue([Properties]() {
            UTDAnalyticsWindow::UserAppend(Properties);
        });
    }
}

void UTDAnalyticsWindow::UserUniqueAppend(const FString& Properties) {
    if (isInitSuccess) {
        typedef void(*UserUniqueAppendFunc)(const char*);
        UserUniqueAppendFunc UserUniqAppendWrapper = (UserUniqueAppendFunc)FPlatformProcess::GetDllExport(TDDllHandle, TEXT("UserUniqAppendWrapper"));
        if (UserUniqAppendWrapper) {
            UserUniqAppendWrapper(TCHAR_TO_UTF8(*Properties));
            FTALog::Info(CUR_LOG_POSITION, TEXT("UTDAnalytics UserUniqueAppend Called "));
        }
    }
    else {
        FunctionQueue.Enqueue([Properties]() {
            UTDAnalyticsWindow::UserUniqueAppend(Properties);
        });
    }
}

FString UTDAnalyticsWindow::GetDeviceID() {
    if (isInitSuccess) {
        typedef char*(*GetDeviceIDFunc)();
        GetDeviceIDFunc GetDeviceIdWrapper = (GetDeviceIDFunc)FPlatformProcess::GetDllExport(TDDllHandle, TEXT("GetDeviceIdWrapper"));
        if (GetDeviceIdWrapper) {
            char* DeviceID = GetDeviceIdWrapper();
            FString FDeviceID = FString(UTF8_TO_TCHAR(DeviceID));
            DeleteChar(DeviceID);
            return FDeviceID;
        }
    }
    return TEXT("");
}

void UTDAnalyticsWindow::Flush() {
    if (isInitSuccess) {
        typedef void(*FlushFunc)();
        FlushFunc FlushWrapper = (FlushFunc)FPlatformProcess::GetDllExport(TDDllHandle, TEXT("FlushWrapper"));
        if (FlushWrapper) {
            FlushWrapper();
            FTALog::Info(CUR_LOG_POSITION, TEXT("UTDAnalytics Flush Called "));
        }
    }
    else {
        FunctionQueue.Enqueue([]() {
            UTDAnalyticsWindow::Flush();
        });
    }
}

void UTDAnalyticsWindow::CalibrateTime(int64 timestamp) {
    if (isInitSuccess) {
        typedef void(*CalibrateTimeFunc)(int64);
        CalibrateTimeFunc CalibrateTimeWrapper = (CalibrateTimeFunc)FPlatformProcess::GetDllExport(TDDllHandle, TEXT("CalibrateTimeWrapper"));
        if (CalibrateTimeWrapper) {
            CalibrateTimeWrapper(timestamp);
            FTALog::Info(CUR_LOG_POSITION, TEXT("UTDAnalytics CalibrateTime Called "));
        }
    }
    else {
        FunctionQueue.Enqueue([timestamp]() {
            UTDAnalyticsWindow::CalibrateTime(timestamp);
        });
    }
}

void UTDAnalyticsWindow::SetSuperProperties(const FString& Properties) {
    if (isInitSuccess) {
        typedef void(*SetSuperPropertiesFunc)(const char*);
        SetSuperPropertiesFunc SetSuperPropertyWrapper = (SetSuperPropertiesFunc)FPlatformProcess::GetDllExport(TDDllHandle, TEXT("SetSuperPropertyWrapper"));
        if (SetSuperPropertyWrapper) {
            SetSuperPropertyWrapper(TCHAR_TO_UTF8(*Properties));
            FTALog::Info(CUR_LOG_POSITION, TEXT("UTDAnalytics SetSuperProperties Called "));
        }
    }
    else {
        FunctionQueue.Enqueue([Properties]() {
            UTDAnalyticsWindow::SetSuperProperties(Properties);
        });
    }
}

void UTDAnalyticsWindow::ClearSuperProperties() {
    if (isInitSuccess) {
        typedef void(*ClearSuperPropertiesFunc)();
        ClearSuperPropertiesFunc ClearSuperPropertyWrapper = (ClearSuperPropertiesFunc)FPlatformProcess::GetDllExport(TDDllHandle, TEXT("ClearSuperPropertyWrapper"));
        if (ClearSuperPropertyWrapper) {
            ClearSuperPropertyWrapper();
            FTALog::Info(CUR_LOG_POSITION, TEXT("UTDAnalytics ClearSuperProperties Called "));
        }
    }
    else {
        FunctionQueue.Enqueue([]() {
            UTDAnalyticsWindow::ClearSuperProperties();
            });
    }
}

void UTDAnalyticsWindow::UnsetSuperProperty(const FString& Property) {
    if (isInitSuccess) {
        typedef void(*UnsetSuperPropertyFunc)(const char*);
        UnsetSuperPropertyFunc UnsetSuperPropertyWrapper = (UnsetSuperPropertyFunc)FPlatformProcess::GetDllExport(TDDllHandle, TEXT("UnsetSuperPropertiesWrapper"));
        if (UnsetSuperPropertyWrapper) {
            UnsetSuperPropertyWrapper(TCHAR_TO_UTF8(*Property));
            FTALog::Info(CUR_LOG_POSITION, TEXT("UTDAnalytics UnsetSuperProperty Called "));
        }
    }
    else {
        FunctionQueue.Enqueue([Property]() {
            UTDAnalyticsWindow::SetSuperProperties(Property);
            });
    }
}

FString UTDAnalyticsWindow::GetSuperProperties() {
    if (isInitSuccess) {
        typedef char* (*GetSuperPropertiesFunc)();
        GetSuperPropertiesFunc GetSuperPropertiesWrapper = (GetSuperPropertiesFunc)FPlatformProcess::GetDllExport(TDDllHandle, TEXT("GetSuperPropertiesWrapper"));
        if (GetSuperPropertiesWrapper) {
            char* SuperProperties = GetSuperPropertiesWrapper();
            FString FSuperProperties = FString(UTF8_TO_TCHAR(SuperProperties));
            DeleteChar(SuperProperties);
            return FSuperProperties;
        }
    }
    return TEXT("");
}

void UTDAnalyticsWindow::TrackFirst(const FString& EventName, const FString& Properties, const FString& FirstCheckId) {
    if (isInitSuccess) {
        typedef void(*TrackFirstFunc)(const char*, const char*, const char*);
        TrackFirstFunc TrackFirstEventWrapper = (TrackFirstFunc)FPlatformProcess::GetDllExport(TDDllHandle, TEXT("TrackFirstEventWrapper"));
        if (TrackFirstEventWrapper) {
            if (Properties.IsEmpty()) {
                TrackFirstEventWrapper(TCHAR_TO_UTF8(*EventName), TCHAR_TO_UTF8(TEXT("{}")), TCHAR_TO_UTF8(*FirstCheckId));
            }
            else {
                TrackFirstEventWrapper(TCHAR_TO_UTF8(*EventName), TCHAR_TO_UTF8(*Properties), TCHAR_TO_UTF8(*FirstCheckId));
            }
            FTALog::Info(CUR_LOG_POSITION, TEXT("UTDAnalytics TrackFirst Called "));
        }
    }
    else {
        FunctionQueue.Enqueue([EventName,Properties, FirstCheckId]() {
            UTDAnalyticsWindow::TrackFirst(EventName,Properties, FirstCheckId);
        });
    }
}

void UTDAnalyticsWindow::TrackUpdate(const FString& EventName, const FString& Properties, const FString& EventId) {
    if (isInitSuccess) {
        typedef void(*TrackUpdateFunc)(const char*, const char*, const char*);
        TrackUpdateFunc TrackUpdatableEventWrapper = (TrackUpdateFunc)FPlatformProcess::GetDllExport(TDDllHandle, TEXT("TrackUpdatableEventWrapper"));
        if (TrackUpdatableEventWrapper) {
            if (Properties.IsEmpty()) {
                TrackUpdatableEventWrapper(TCHAR_TO_UTF8(*EventName), TCHAR_TO_UTF8(TEXT("{}")), TCHAR_TO_UTF8(*EventId));
            }
            else {
                TrackUpdatableEventWrapper(TCHAR_TO_UTF8(*EventName), TCHAR_TO_UTF8(*Properties), TCHAR_TO_UTF8(*EventId));
            }
            FTALog::Info(CUR_LOG_POSITION, TEXT("UTDAnalytics TrackUpdate Called "));
        }
    }
    else {
        FunctionQueue.Enqueue([EventName, Properties, EventId]() {
            UTDAnalyticsWindow::TrackUpdate(EventName, Properties, EventId);
        });
    }
}

void UTDAnalyticsWindow::TrackOverwrite(const FString& EventName, const FString& Properties, const FString& EventId) {
    if (isInitSuccess) {
        typedef void(*TrackOverwriteFunc)(const char*, const char*, const char*);
        TrackOverwriteFunc TrackOverWritableEventWrapper = (TrackOverwriteFunc)FPlatformProcess::GetDllExport(TDDllHandle, TEXT("TrackOverWritableEventWrapper"));
        if (TrackOverWritableEventWrapper) {
            if (Properties.IsEmpty()) {
                TrackOverWritableEventWrapper(TCHAR_TO_UTF8(*EventName), TCHAR_TO_UTF8(TEXT("{}")), TCHAR_TO_UTF8(*EventId));
            }
            else {
                TrackOverWritableEventWrapper(TCHAR_TO_UTF8(*EventName), TCHAR_TO_UTF8(*Properties), TCHAR_TO_UTF8(*EventId));
            }
            FTALog::Info(CUR_LOG_POSITION, TEXT("UTDAnalytics TrackOverwrite Called "));
        }
    }
    else {
        FunctionQueue.Enqueue([EventName, Properties, EventId]() {
            UTDAnalyticsWindow::TrackOverwrite(EventName, Properties, EventId);
        });
    }
}

FString UTDAnalyticsWindow::GetPresetProperties() {
    if (isInitSuccess) {
        typedef char* (*GetPresetPropertiesFunc)();
        GetPresetPropertiesFunc GetPresetPropertiesWrapper = (GetPresetPropertiesFunc)FPlatformProcess::GetDllExport(TDDllHandle, TEXT("GetPresetPropertiesWrapper"));
        if (GetPresetPropertiesWrapper) {
            char* PresetProperties = GetPresetPropertiesWrapper();
            FString FPresetProperties = FString(UTF8_TO_TCHAR(PresetProperties));
            DeleteChar(PresetProperties);
            return FPresetProperties;
        }
    }
    return TEXT("");
}

