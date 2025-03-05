#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

class FCustomToolBarCommands : public TCommands<FCustomToolBarCommands>
{
public:
    FCustomToolBarCommands(): TCommands<FCustomToolBarCommands>( TEXT( "CustomToolBar" ),
                                                                 NSLOCTEXT( "Contexts", "CustomToolBar", "CustomToolBar" ),
                                                                 NAME_None,
                                                                 FAppStyle::GetAppStyleSetName() )
    {
    };

    // 必须重载
    virtual void RegisterCommands() override;

public:
    TSharedPtr<FUICommandInfo> PluginAction;
};