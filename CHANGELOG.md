# 更新日志

按版本号倒序记录，优先保留[历史更新日志]的内容与日期，其余根据 Git 标签和版本号变更提交整理，日期使用对应提交日期，无标签的版本已注明。提交日期不代表正式发布日期；2.x 与 3.x 曾并行维护，因此日期可能交错。

## 未发布

- 修复 Android `GetDistinctId` 等字符串 getter 返回中文、emoji 时的乱码，保留 ID 中的末尾空格。
- 统一 Android JNI 字符串输入、返回值、数组及回调的 UTF-16 转换，补强异常处理和局部引用释放。
- 增加 Unicode 回读及 JNI 异常回归测试；Android 测试代码与相关依赖仅在启用自动化测试的 Debug、DebugGame、Development 构建中引入，Test / Shipping 构建不引入。

## [3.1.0] - 2026-09-29

此版本号节点尚无对应 Git 标签。

- 修复 Android 时间校准和日志开关的桥接调用，改由 GameActivity 调用 `TDAnalytics` 接口。
- 修复编辑器 PIE 退出时定时上报引发的崩溃，完善定时器和对象生命周期清理。
- 修复 UE 5.4 及以上版本的数组删除接口适配，并调整平台文件头名称和 iOS 布尔参数声明，改善编译兼容性。

## [3.0.4] - 2026-07-24

此版本号节点尚无对应 Git 标签。

- Android 原生 SDK 升级至 `3.4.4`，iOS 原生 SDK 升级至 `3.4.6`。
- Android 初始化时通过 `TDAnalytics.setCustomerLibInfo` 设置 Unreal SDK 信息。
- Demo 工程升级至 Unreal Engine 5.6。
- 增加 Editor Automation 测试及运行脚本、Android / iOS 打包验证脚本。

## [3.0.3] - 2025-10-11

- Windows 原生 C++ SDK 升级至 `1.5.3`，支持断网时非阻塞地将数据写入本地数据库。

## [3.0.2] - 2025-09-08

- 修复 Windows 动态公共属性获取时 App ID 为空的问题，回退使用当前默认 App ID。
- 补齐 Windows `UnsetSuperProperty` 调用，修复初始化前排队执行时误调用设置公共属性接口的问题。
- 更新 Windows 原生库并修复平台条件编译问题。

## [3.0.1] - 2025-08-01

此版本号节点尚无对应 Git 标签。

- Windows 原生库升级至 `1.5.0`。
- Windows 事件上报合并动态公共属性。

## [3.0.0] - 2025-03-20

- Windows 接入原生 C++ SDK，替换原有 PC 实现，并配置所需 DLL 的复制和打包依赖。
- Windows 支持异步初始化，缓存初始化完成前的接口调用。
- 增加 Windows 预置属性获取及清空公共属性支持。
- 更新 Android / iOS 原生 SDK，并补充 iOS `ThinkingDataCore` framework 依赖。

## [2.1.1]

此版本没有对应 Git 标签，历史中存在两处独立的同版本号维护节点：

- 2025-07-07（[47da479]）：更新 Android / iOS 原生 SDK 及相关依赖。
- 2025-09-25（[7830077]）：修复 Android 时间校准调用遗漏时间戳参数的问题。

## [2.1.0] - 2025-03-25

- 更新 Android / iOS 原生 SDK。
- iOS 增加 `ThinkingDataCore` framework 依赖，并调整 SDK 头文件引用。
- 更新 Android SDK 包以修复已知问题。

## [2.0.7] - 2024-12-06

此版本号节点尚无对应 Git 标签。

- 调整时区偏移的时间格式化计算。
- 降低 PC 上报任务线程优先级。

## [2.0.6] - 2024-09-11

- 适配 Unreal Engine 5.4 的 Analytics Provider 接口。

## [2.0.5] - 2024-09-03

- Android、iOS 和 PC 增加 `ClearSuperProperties`、`UnsetSuperProperty`，支持清空全部公共属性及删除单项公共属性。
- 修复 PC `DEBUG_ONLY` 模式无法持续上报数据的问题。

## [2.0.4] - 2024-08-05

- 修复 PC Debug 模式上报时访问空任务队列导致的崩溃。

## [2.0.3] - 2024-08-05

- 修复编辑器 PIE 模式下实例及定时器处理问题。
- 修复 PC 多线程上报导致的崩溃。

## [2.0.2] - 2024-02-07

- 修复 PC 网络请求返回无效响应时的空指针崩溃。

## [2.0.1] - 2023-11-23

- Android 原生 SDK 升级至 `3.0.0`，调整为 `TDAnalytics` 与 `TDCore` 依赖。
- 更新 Android 初始化及 SDK 信息设置逻辑。

## [2.0.0] - 2023-11-08

- 发布 2.0.0 正式版本，延续 beta.1 的模块命名及 Unreal Engine 5.3 适配。
- 补充 SDK API 文档入口和初始化接口说明。

## [2.0.0-beta.1] - 2023-09-22

- 插件及模块由 `ThinkingAnalytics` 更名为 `TDAnalytics`，同步调整 API 类名、头文件和配置引用；从 1.x 升级时需同步迁移项目引用。
- 适配 Unreal Engine 5.3，调整部分引擎 API 的版本兼容逻辑。
- 补充新版 Android GameActivity 的自动采集回调入口。
- 修复 iOS 首次事件、可更新事件和可重写事件遗漏动态公共属性的问题。
- 修复 PC 请求失败后任务工作状态未重置的问题。

## [1.6.0] - 2022-09-23

- 迁移至 Unreal Engine 5。
- 修复 PC 预置属性 `#lib` 缺失。
- 修复 PC 公共属性设置逻辑，改为追加属性。
- 禁用 Android `#fps` 采集，避免与引擎 MediaPlayer 冲突导致视频无法播放。
- 增加 PC 预置属性 `#zone_offset` 获取及时间格式化逻辑。
- 增加 PC 事件缓存，按 15 秒或 20 条事件触发上报。
- 增加 PC 设置上报状态接口。

## [1.5.0.1] - 2022-09-02

此版本号节点尚无对应 Git 标签。

- 更新 Android 原生库，修复与 Unreal MediaPlayer 的兼容问题。

## [1.5.0] - 2022-06-15

- iOS / Android 原生 SDK 升级至 `2.8.0.1`。
- Android / iOS 支持数据加密、第三方数据同步、自动采集事件回调及 Crash 事件采集。
- Android / iOS 支持 `SetTrackStatus` 接口。
- PC 新增 `#install_time`、`#fps`、`#ram`、`#disk` 预置属性。
- 全平台支持 `UserUniqueAppend` 接口。
- 优化动态公共属性接口并修复已知问题。

## [1.4.1] - 2022-03-15

- iOS / Android 原生 SDK 升级至 `2.7.3`。
- Android / iOS 支持复杂数据类型。
- 自动采集接口支持指定事件类型及自定义属性。
- 调整代码结构。

## [1.4.0] - 2021-12-31

- 新增 Windows / macOS 支持，覆盖事件上报、用户属性、进阶功能及公共属性。
- 增加 Windows / macOS 预置属性。
- 调整部分代码逻辑。

## [1.3.0] - 2021-08-27

- 新增预置属性获取接口。
- 支持设置动态公共属性。
- 支持通过 NTP 或时间戳校准时间。
- 支持静态公共属性的设置和获取。

## [1.2.1] - 2021-06-29

- 支持获取设备 ID。
- 支持获取访客 ID。

## [1.2.0] - 2021-06-03

- 优化 `install`、`start` 事件上报逻辑。
- 优化数据传输格式。
- 新增应用唯一标识预置属性 `#bundle_id`。
- 优化代码。

## [1.1.0] - 2020-08-25

- 支持首次事件，并允许通过自定义 ID 判断是否首次上报。
- 支持可更新事件、可重写事件。
- `#lib`、`#lib_version` 使用 Unreal SDK 信息。
- 原生 SDK 升级至 `2.6.0`。

## [1.0.3] - 2020-06-28

- Android 原生 SDK 升级至 `2.5.6`，避免空指针问题。

## [1.0.2] - 2020-06-23

- 新增系统语言预置属性 `#system_language`。
- 升级原生 SDK。

## [1.0.1] - 2020-05-27

- 修复不合理依赖导致的打包错误。

## [1.0.0] - 2020-05-18

- 支持事件上报和全部用户属性接口。
- 支持 Debug 模式。
- 支持时区对齐和时间校准。
- 支持 `Flush`、`OptOutTracking`、`OptInTracking`、`EnableTracking` 等辅助接口。

[历史更新日志]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/blob/v1.6.0/CHANGELOG.md
[3.1.0]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/commit/82e7491
[3.0.4]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/commit/8c7fdab
[3.0.3]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/tags/v3.0.3
[3.0.2]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/tags/v3.0.2
[3.0.1]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/commit/4b5f88a
[3.0.0]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/tags/v3.0.0
[2.1.1]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/commit/7830077
[47da479]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/commit/47da479
[7830077]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/commit/7830077
[2.1.0]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/tags/v2.1.0
[2.0.7]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/commit/bf25dde
[2.0.6]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/tags/v2.0.6
[2.0.5]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/tags/v2.0.5
[2.0.4]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/tags/v2.0.4
[2.0.3]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/tags/v2.0.3
[2.0.2]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/tags/v2.0.2
[2.0.1]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/tags/v2.0.1
[2.0.0]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/tags/v2.0.0
[2.0.0-beta.1]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/tags/v2.0.0-beta.1
[1.6.0]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/tags/v1.6.0
[1.5.0.1]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/commit/d17ea01
[1.5.0]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/tags/v1.5.0
[1.4.1]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/tags/v1.4.1
[1.4.0]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/tags/v1.4.0
[1.3.0]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/tags/v1.3.0
[1.2.1]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/tags/v1.2.1
[1.2.0]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/tags/v1.2.0
[1.1.0]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/tags/v1.1.0
[1.0.3]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/tags/v1.0.3
[1.0.2]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/tags/v1.0.2
[1.0.1]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/tags/v1.0.1
[1.0.0]: https://gitlab.thinkingdata.cn/thinking-analytics/data-collector/client-sdk/unreal-sdk/-/tags/v1.0.0
