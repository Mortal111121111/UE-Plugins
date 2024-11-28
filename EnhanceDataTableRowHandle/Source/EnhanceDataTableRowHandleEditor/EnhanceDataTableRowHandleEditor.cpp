// Copyright Epic Games, Inc. All Rights Reserved.

#include "EnhanceDataTableRowHandleEditor.h"

#include "DTRowHandleCustomization.h"
#include "EnhanceDataTableRowHandle/EnhanceDTRowHandleSetting.h"

#define LOCTEXT_NAMESPACE "FEnhanceDataTableRowHandleEditorModule"

class UEnhanceDTRowHandleSetting;

void FEnhanceDataTableRowHandleEditorModule::StartupModule()
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

void FEnhanceDataTableRowHandleEditorModule::ShutdownModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");//

	for (const FName& StructName : UnregisterStructNames)
	{
		PropertyModule.UnregisterCustomClassLayout(StructName);
	}
	
	PropertyModule.NotifyCustomizationModuleChanged();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEnhanceDataTableRowHandleEditorModule, EnhanceDataTableRowHandleEditorModule)