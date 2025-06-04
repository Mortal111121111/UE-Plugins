#include "EasyVariablesExporterEditor.h"

#include "BlueprintEditorModule.h"
#include "Customization/EasyExporterEnumParam/EasyExporterEnumParamCustomization.h"
#include "Customization/NeedExportVariableDetailCustomization/NeedExportVariableDetailCustomization.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FEasyVariablesExporterEditorModule"

void FEasyVariablesExporterEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");//
	
	PropertyModule.RegisterCustomPropertyTypeLayout(
		"EasyExporterEnumParam",
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&EasyExporterEnumParamCustomization::MakeInstance));
	
	PropertyModule.NotifyCustomizationModuleChanged();

	// Register Blueprint editor variable customization
	FBlueprintEditorModule& BlueprintEditorModule = FModuleManager::LoadModuleChecked<FBlueprintEditorModule>("Kismet");
	BlueprintVariableCustomizationHandle = BlueprintEditorModule.RegisterVariableCustomization(FProperty::StaticClass(), FOnGetVariableCustomizationInstance::CreateStatic(&NeedExportVariableDetailCustomization::MakeInstance));
}

void FEasyVariablesExporterEditorModule::ShutdownModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");//

	PropertyModule.UnregisterCustomClassLayout("EasyExporterEnumParam");
	PropertyModule.NotifyCustomizationModuleChanged();

	// Unregister Blueprint editor variable customization
	FBlueprintEditorModule& BlueprintEditorModule = FModuleManager::LoadModuleChecked<FBlueprintEditorModule>("Kismet");
	BlueprintEditorModule.UnregisterVariableCustomization(FProperty::StaticClass(), BlueprintVariableCustomizationHandle);
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FEasyVariablesExporterEditorModule, EasyVariablesExporterEditor)