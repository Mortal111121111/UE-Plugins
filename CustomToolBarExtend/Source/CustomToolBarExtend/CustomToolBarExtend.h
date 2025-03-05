// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FCustomToolBarExtendModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    void RegisterPluginCommands();
    void UnregisterPluginCommands();

    void RegisterMenus();
    void UnregisterMenus();

    TSharedRef<SWidget> MakeComboButtonWidgets();
    
    // Action Func
    void PluginActionButtonOnClicked();
    void PluginActionButtonOnClicked_1();
    void PluginActionButtonOnClicked_2();
private:
    TSharedPtr<class FUICommandList> PluginCommands;
};