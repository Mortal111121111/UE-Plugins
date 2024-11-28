#include "UIExtensionEditor.h"

#include "EnhanceTextCustomization.h"

#define LOCTEXT_NAMESPACE "FUIExtensionEditorModule"

void FUIExtensionEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	PropertyModule.RegisterCustomPropertyTypeLayout(FName("EnhanceText"),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&EnhanceTextCustomization::MakeInstance));
	PropertyModule.NotifyCustomizationModuleChanged();
	
}

void FUIExtensionEditorModule::ShutdownModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	
	PropertyModule.UnregisterCustomClassLayout(FName("EnhanceText"));
	PropertyModule.NotifyCustomizationModuleChanged();
	
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FUIExtensionEditorModule, UIExtensionEditor)