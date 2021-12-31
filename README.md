## ThinkingAnalytics Unreal 4 SDK 使用指南

本指南将会介绍如何使用 ThinkingAnalytics Unreal Engine 4 SDK 接入您的项目。

ThinkingAnalytics 实现了游戏引擎内置的 Analytics 服务，同时也支持直接通过 C++ 代码调用接口上报数据。

目前支持 Android、iOS、Windows和MacOS平台。

### 1 集成 SDK
1.1 下载 [Unreal SDK ](http://download.thinkingdata.cn/client/release/ta_unreal_sdk.zip)，解压后将 `ThinkingAnalytics` 放入您项目的 `Plugins` 目录； 如果 `Plugins` 目录不存在，首先在项目根目录下创建 `Plugins` 目录，然后将 `ThinkingAnalytics` 目录放入其中。

1.2 激活 ThinkingAnalytics 插件

* 重启 Unreal Editor
* 打开 编辑>插件，在项目 `Analytics` 类别下，启用 `ThinkingAnalytics`
* 如果您使用 Blueprint，请启用内置 `Analytics` 类别下的 `Analytics Blueprint Library`
* 再次重启 Unreal Editor
* 打开 编辑>项目设置，在插件类别下，设置 SDK 参数：
    * Server Url: 接收端地址
    * App ID: 您项目的 APP ID，在 TA 后台项目管理页可以查看
    * SDK MODE: SDK 运行模式
    * Enable Log: 是否开启客户端日志
    * TimeZone: 可选，如果需要对齐时区，请填写规范的 TimeZone ID，例如 "Asia/Shanghai", "UTC".
    * 在 `Config` 目录下的 `DefaultEngine.ini` 文件中添加以下内容:
    ```
    [Analytics]
    ProviderModuleName=ThinkingAnalytics
    ```
* 如果您希望直接在 C++ 代码中使用 `ThinkingAnalytics` 接口，需要在您项目的 `*.Build.cs` 文件中添加以下内容：
    ```csharp
    PrivateDependencyModuleNames.AddRange(new string[] { "ThinkingAnalytics" });
        
    PrivateIncludePathModuleNames.AddRange(new string[] { "ThinkingAnalytics" });
    ```
    并且，在您希望使用 SDK 的文件中引用 `ThinkingAnalytics.h` 头文件:
    ```cpp
    #include "ThinkingAnalytics.h"
    ```

1.3 初始化 SDK

在调用 `ThinkingAnalytics` 接口之前，您应该首先调用 SDK 的初始化接口。

如果您使用蓝图，需要首先调用 `Analytics Blueprint Library` 的 `StartSession` 接口。您也可调用 `Thinking Analytics` 类目下的 `Initialize` 接口。

如果在 C++ 代码中调用，可如下调用:
```cpp
// 初始化 SDK
UThinkingAnalytics::Initialize();
```

之后，就可以使用 SDK 上报事件了。可以使用 `Analytics Blueprint Library` 中的 `RecordEvent` 接口，可参考 UE4 官方的[蓝图分析插件](https://docs.unrealengine.com/zh-CN/Gameplay/Analytics/Blueprints/index.html)指南，也可以使用下文描述的 `UThinkingAnalytics` 类提供的接口。


### 2 设置用户 ID
默认情况下，SDK 会使用一个随机 UUID 作为访客 ID 来标识一个用户，在用户重新安装游戏，或者更换设备时，访客 ID 将会变更。

2.1 设置访客 ID（可选）

如果您的游戏有自己的访客 ID 管理体系，则您可以调用 `Identify` 来设置访客 ID:

```cpp
UThinkingAnalytics::Identify("your_distinct_id");
```

2.2 设置账号 ID

在用户进行登录时，可调用 `Login` 来设置用户的账号 ID，在设置完账号 ID 后，将会以账号 ID 作为身份识别 ID，并且设置的账号 ID 将会在调用 `Logout` 之前一直保留：
```cpp
// 设置账号 ID
UThinkingAnalytics::Login("your_account_id");

// 清除账号 ID
UThinkingAnalytics::Logout();
```
> 注意：该方法不会上传用户登录、用户登出等事件。

在 `Analytics Blueprint Library` 中的 `SetUserId` 接口等同于上述 `Login` 接口。

### 3 上传事件

您可以通过 `Analytics Blueprint Library` 中的 `RecordEvent` 接口去上报时间，也可以在 C++ 代码中使用 Json 去更灵活的上报携带事件属性的事件。一般情况下，您可能需要上传十几到上百个不同的事件，如果您是第一次使用 TA 后台，我们推荐您先上传几个关键事件。

3.1 上传事件

建议您根据先前梳理的文档来设置事件的属性以及发送信息的条件。事件名称是`FString`类型，只能以字母开头，可包含数字，字母和下划线“_”，长度最大为50个字符，对字母大小写不敏感。
- 事件属性是 JSON 格式的 `FString` 类型
- 事件属性`Key`为属性名称，规定只能以字母开头，包含数字，字母和下划线“_”，长度最大为50个字符，对字母大小写不敏感；
- 属性值支持四种类型：字符串、数值类、bool、JSONArray

```cpp
// 上报一个不带属性事件
UThinkingAnalytics::Track("TEST_EVENT", "");
```

3.2 记录事件时长

您可以调用 `TimeEvent` 来开始计时，配置您想要计时的事件名称，当您上传该事件时，将会自动在您的事件属性中加入`#duration`这一属性来表示记录的时长，单位为秒。
```cpp
// 调用 TimeEvent 开启对 TIME_EVENT 事件的计时
UThinkingAnalytics::TimeEvent("TIME_EVENT");

// do some thing...

// 通过 Track 上传 TIME_EVENT 事件时，会在属性中添加 #duration 属性
UThinkingAnalytics::Track("TIME_EVENT", "");
```

### 4 用户属性
TA 平台目前支持的用户属性设置接口为 `UserSet`、`UserSetOnce`、`UserAdd`、`UserDelete`、`UserUnset`、`UserAppend`.

4.1 UserSet

对于一般的用户属性，您可以调用 `UserSet` 来进行设置，使用该接口上传的属性将会覆盖原有的属性值，如果之前不存在该用户属性，则会新建该用户属性。
```cpp
// UserSet 接口定义
UThinkingAnalytics::UserSet(const FString& Properties);
```

用户属性的格式要求与事件属性类似。

4.2 UserSetOnce

如果您要上传的用户属性只要设置一次，则可以调用 `UserSetOnce` 来进行设置，当该属性之前已经有值的时候，将会忽略这条信息：
```cpp
// UserSetOnce 接口定义
UThinkingAnalytics::UserSetOnce(const FString& Properties);
```
> 注意：`UserSetOnce` 设置的用户属性类型及限制条件与 `UserSet` 一致。

4.3 UserAdd

当您要上传数值型的属性时，您可以调用 `UserAdd` 来对该属性进行累加操作，如果该属性还未被设置，则会赋值 `0` 后再进行计算，可传入负值，等同于相减操作。
```cpp
// 数值类用户属性减去 100.9
UThinkingAnalytics::UserAdd("USER_PROP_NUM", -100.9);
```
> 注意： `UserAdd` 置的属性类型以及 `Key` 值的限制与 `UserSet` 一致，但 `Value` 只允许数值类型。

4.4 UserDelete

如果您要删除某个用户，可以调用 `UserDelete` 将这名用户删除，您将无法再查询该名用户的用户属性，但该用户产生的事件仍然可以被查询到。
```cpp
// 删除用户
UThinkingAnalytics::UserDelete();
```

4.5 UserUnset

如果您需要重置用户的某个属性，可以调用 `UserUnset` 将已经设置的属性删除。
```cpp
// 删除某个用户属性
UThinkingAnalytics::UserUnset("userPropertyName");
```

4.6 UserAppend

您可以调用 UserAppend 为 List 类型的用户属性追加元素。
```cpp
// UserSetOnce 接口定义
UThinkingAnalytics::UserAppend(const FString& Properties);
```

### 5 自动采集事件

本 SDK 提供了三种事件的自动采集：
* ta_app_install: 游戏安装，当安装后首次打开游戏会采集该事件
* ta_app_start: 游戏进入前台的时候采集该事件
* ta_app_end: 游戏退到后台的时候采集该事件

通过调用 `EnableAutoTrack` 接口可以开启自动采集：
```cpp
// 开启自动采集
UThinkingAnalytics::EnableAutoTrack();
```

> 注意： 如果您需要自定义访客 ID，请务必在开启自动采集功能之前调用  `Identify` 接口设置访客 ID.

### 6 其他接口

6.1 Flush

默认情况下，数据会先保存在本地缓存，然后依据一定的上报策略上传到服务端。如果您希望立即将缓存中的数据上报可以调用 `Flush` 接口。 `Analytics Blueprint Library` 中的 `FlushEvents` 接口最终也会调用 `Flush`:
```cpp
UThinkingAnalytics::Flush();
```

6.2 校准时间

6.2.1 时间戳校准
SDK 默认会使用本机时间作为事件发生时间上报，如果用户手动修改设备时间会影响到您的业务分析，您可以使用从服务端获取的当前时间戳对 SDK 的时间进行校准。此后，所有未指定时间的调用，包括事件数据和用户属性设置操作，都会使用校准后的时间作为发生时间。

```cpp
// 1585633785954 为当前 unix 时间戳，单位为毫秒，对应北京时间 2020-03-31 13:49:45
UThinkingAnalytics::CalibrateTime(1585633785954);
```

6.2.2 NTP校准
在 v1.3.0 版本新增 NTP 时间校准接口, 您需要传入您的用户可以访问的 NTP 服务器地址。之后 SDK 会尝试从传入的 NTP 服务地址中获取当前时间，并对 SDK 时间进行校准。如果在默认的超时时间（3 秒）之内，未获取正确的返回结果，后续将使用本地时间上报数据。
```cpp
UThinkingAnalytics::CalibrateTimeWithNtp(const FString& urlString);
```

> 注意：校准时间应该在初始化完成后，开启自动采集或者上报任何其他数据之前进行；您也可以先校准时间，然后再调用初始化接口.

6.3 暂停/停止数据上报

有两类停止 SDK 上报的接口：

#### 暂停 SDK 上报（EnableTracking）

您可能希望在一些场景下，暂时停止 SDK 的数据采集以及上报，比如用户处于测试环境中、或者用户登录了一个测试账号，此时您可以调用 `EnableTracking`，传入 `false` 来暂停 SDK 的上报，此前已经设置的访客 ID、账户 ID 等将保留；已经采集但还未上报成功的数据将继续尝试上报；后续不能采集以及上报任何新数据、不能设置访客 ID、账户 ID 等。

SDK 的停止状态将会被保存在本地缓存，直到调用 `EnableTracking` 并传入 `true`，SDK 将会重新恢复数据采集以及上报。

```cpp
// 暂停上报，已缓存数据和已经设置的信息不被清除
UThinkingAnalytics::EnableTracking(false);

// 恢复上报
UThinkingAnalytics::EnableTracking(true);
```

#### 停止 SDK 上报（OptOutTracking）

在一些特殊场景下，您可能需要完全停止 SDK 的功能，比如在适用 GDPR 的地区，用户选择不提供数据采集权限，则您可以调用 `OptOutTracking`. 该接口与 `EnableTracking` 的最大区别在于，其将会清空本地缓存，包括访客 ID，账号 ID，以及未上报的数据队列。之后再关闭采集和上报功能。

```cpp
// 停止默认实例的上报, 并清空本地缓存
UThinkingAnalytics::OptOutTracking();
```

SDK 的停止状态也将保存在本地缓存，直到调用 OptInTracking，后续可以继续上报，但此时相当于一个全新的实例。

```cpp
// 重新开启上报
ThinkingAnalyticsAPI.OptInTracking();
```

6.6 设置公共事件属性

在 v1.3.0 版本新增设置公共事件属性, 对于一些重要的属性，譬如用户的会员等级、来源渠道等，这些属性需要设置在每个事件中，此时您可以将这些属性设置为公共事件属性。公共事件属性指的就是每个事件都会带有的属性。我们推荐您在发送事件前，先设置公共事件属性。
```cpp
UThinkingAnalytics::SetSuperProperties("{\"static_property1\":\"value1\",\"static_property2\":\"value2\"}");
```

您也可以获取设置的公共事件属性。
```cpp
FString SuperProperties = UThinkingAnalytics::GetSuperProperties();
```

6.7 设置动态公共属性

在 v1.3.0 版本新增设置动态公共属性, 在设置动态公共属性类之后，SDK 将在上报事件属性时自动执行并获取返回值中的属性，并添加到每个事件中。
```cpp
// 定义动态公共属性函数
static FString TDReturnDyldParams() {
    return "{\"dyld_property1\":\"value1\",\"dyld_property2\":\"value2\"}";
}

// 设置动态公共属性
void UMyDemoWidget::callSetDynamicSuperPropertiesFunction(){
    UThinkingAnalytics::dynamicPropertiesMap.insert(pair<FString,FString(*)(void)>("inset your appid" ,&TDReturnDyldParams));
}
```

6.8 获取预制属性

在 v1.3.0 版本新增预获取制属性接口, SDK 会在事件属性上报时候自动加上一些内置属性, 您可以通过此接口获取预置属性。
```cpp
FString PresetProperties = UThinkingAnalytics::GetPresetProperties();
```