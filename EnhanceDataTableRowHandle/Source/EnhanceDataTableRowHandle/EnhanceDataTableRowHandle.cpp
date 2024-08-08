// Copyright Epic Games, Inc. All Rights Reserved.

#include "EnhanceDataTableRowHandle.h"

#include "DTRowHandleCustomization.h"
#include "EnhanceDTRowHandleSetting.h"

#define LOCTEXT_NAMESPACE "FEnhanceDataTableRowHandleModule"

class UEnhanceDTRowHandleSetting;

void FEnhanceDataTableRowHandleModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");//

	const UEnhanceDTRowHandleSetting* Settings = GetDefault<UEnhanceDTRowHandleSetting>();
	for (const FEnhanceDTRowHandleInfo& EnhanceStructPath : Settings->EnhanceStructPaths)
	{
		PropertyModule.RegisterCustomPropertyTypeLayout(
		FName(EnhanceStructPath.StructName.Mid(1)),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&DTRowHandleCustomization::MakeInstance));

		UnregisterStructNames.Add(FName(EnhanceStructPath.StructName.Mid(1)));
	}
	
	PropertyModule.NotifyCustomizationModuleChanged();
}

void FEnhanceDataTableRowHandleModule::ShutdownModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");//

	for (const FName& StructName : UnregisterStructNames)
	{
		PropertyModule.UnregisterCustomClassLayout(StructName);
	}
	
	PropertyModule.NotifyCustomizationModuleChanged();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEnhanceDataTableRowHandleModule, EnhanceDataTableRowHandle)