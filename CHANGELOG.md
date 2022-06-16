#### v1.5.0 2022/06/15
* 更新 iOS(2.8.0.1) ,Android SDK(2.8.0.1) 版本
* Android & IOS 支持数据加密
* PC新增#install_time/#fps/#ram/#disk 预置属性
* Android & IOS 支持三方数据同步
* Android & IOS支持自动采集事件回调
* Android & IOS支持采集Crash事件
* Android & IOS支持setTrackStatus接口
* 全平台支持UserUniqueAppend接口
* 设置动态公共属性接口优化
* 已知bug修复

#### v1.4.1 2022/03/15
* 更新 iOS(2.7.3) ,Android SDK(2.7.3) 版本
* Android & IOS 支持复杂数据类型
* 新增接口支持开启自动采集事件传入需要开启的事件
* 新增接口支持开启自动采集事件时传入自定义属性
* 代码结构调整

#### v1.4.0 2021/12/31
* 新增Windows和MacOS平台支持上报事件、用户属性、进阶功能、公共属性
* 增加Windows和MacOS平台预置属性
* 部分代码逻辑调整

#### v1.3.0 2021/08/27
* 新增预制属性获取接口
* 支持设置动态公共属性
* 支持NTP/时间戳方式校准时间
* 支持静态公共属性的设置和获取

#### v1.2.1 2021/06/29
* 支持设备ID获取
* 支持访客ID获取

#### v1.2.0 2021/06/03
* 优化 install,start 事件上报逻辑
* 优化数据传输格式
* 新增预置属性#bundle_id(应用唯一标识)
* 代码优化

#### v1.1.0 2020/08/25
* 支持首次事件, 允许传入自定义的 ID 校验是否首次上报
* 支持可更新、可重写的事件
* 优化#lib/#lib_version 字段为 UE4 SDK 信息
* 升级原生 SDK 版本为 v2.6.0

#### v1.0.3 2020/06/28
* 更新 Android SDK 到 v2.5.6，避免空指针

#### v1.0.2 2020/06/23
* 新增预置属性 #system_language
* 升级原生 SDK 版本

#### v1.0.1 2020/05/27
* 解决因不合理依赖导致的打包错误

#### v1.0.0 2020/05/18
* 支持事件上报和所有用户属性接口
* 支持 DEBUG 模式
* 支持时区对齐和时间校准
* 支持其他辅助接口：Flush, OptOutTracking, OptInTracking, EnableTracking.
