# UE-Plugins

UE 插件

# CustomNewAsset

自定义右键新建资产

# DataCollection

数据收集，并将内容打印到 Save/xxxlog

# DataTableEnhance

多表合表插件

# MyCustomDetailPanels

自定义细节面板

# MyCustomThunk

泛型蓝图

# UIManager

自动收集 UI 到表里，提供统一 Open 和 Close 接口统一管理层级、是否暂停游戏、输入方式等

# UIExtension

一些常用的小功能 UI

- TabBar ： 目前使用侵入式解决，可以利用 Object 外置逻辑做到完全无感，也不需要使用自己提供的类，但是可能比较费
- SecondConfirm ： 通用二次确认框逻辑
- AutoWriterRichText ： 打字机并支持富文本
- KTimerText : 计时文本插件
- SkipNumber : 数值动态变化累加控件，并使用 C++ 动态实现动画

# MyAsyncBlueprint

异步蓝图函数的实现

# MyCommands

学习 UI Commands

# Toast

通用 Toast 插件

# EnhanceDataTableRowHandle

将 DataTableRowHandle 改为不用选择表，而是从设置中进行绑定

# HaowanSaveGame

多存档

# CustomToolBarExtend

自定义工具栏扩展插件
功能 ：通过配置方式动态增加按钮

# MyCMDLet

熟悉 UE Commandlet

- 新增使用 Commandlet 通过动画 Asset 路径获取对应动画的骨骼信息，并且写入本地 Json

# CustomCsvToDT

手动读取 Csv ,根据自定义的规则构造 Struct 并将其写入 DT

- 重写了 AddDataTableRow 因为原生不能将蓝图声明的结构体写入 DT
- 提供解决修改 Csv 后导致编码不一致的脚本 bat

# EasyVariablesExporter

UObject 属性导入导出插件

- 为了方便策划统一配置 GAS GA 的具体数值而封装为插件
- 目前支持 int、float、bool、string、enum、vector、struct、name、text 数据
