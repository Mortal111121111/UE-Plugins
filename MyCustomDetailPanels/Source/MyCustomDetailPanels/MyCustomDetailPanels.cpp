// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyCustomDetailPanels.h"

#include "MyStructCustomization.h"
#include "MyTestStruct.h"

#define LOCTEXT_NAMESPACE "FMyCustomDetailPanelsModule"

void FMyCustomDetailPanelsModule::StartupModule()
{
	// 加载 PropertyEditor 模块
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");//
	
	// 注册自定义属性，绑定结构名与自定义类
	PropertyModule.RegisterCustomPropertyTypeLayout(
	// 结构名
	FMyStruct::StaticStruct()->GetFName(),
	// 自定义类实例
	FOnGetPropertyTypeCustomizationInstance::CreateStatic(&MyStructCustomization::MakeInstance));
	
	PropertyModule.NotifyCustomizationModuleChanged();
}

void FMyCustomDetailPanelsModule::ShutdownModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");//
	
	// 注册自定义属性，绑定结构名与自定义类
	PropertyModule.UnregisterCustomPropertyTypeLayout(
	// 结构名
	FMyStruct::StaticStruct()->GetFName()
	);
	
	PropertyModule.NotifyCustomizationModuleChanged();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMyCustomDetailPanelsModule, MyCustomDetailPanels)