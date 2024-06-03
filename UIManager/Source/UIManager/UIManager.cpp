// Copyright Epic Games, Inc. All Rights Reserved.

#include "UIManager.h"

#include "K2Node/CreateUIGraphPanelPinFactory.h"

#define LOCTEXT_NAMESPACE "FUIManagerModule"

void FUIManagerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	const TSharedPtr<FCreateUIGraphPanelPinFactory> BlueprintGraphPanelPinFactory = MakeShareable(new FCreateUIGraphPanelPinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(BlueprintGraphPanelPinFactory);
}

void FUIManagerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUIManagerModule, UIManager)