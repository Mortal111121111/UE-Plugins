// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomToolBarExtend.h"

#include "FCustomToolBarCommands.h"
#include "ToolMenus.h"

#define LOCTEXT_NAMESPACE "FCustomToolBarExtendModule"

static const FName MenuName( "LevelEditor.LevelEditorToolBar.PlayToolBar" );

void FCustomToolBarExtendModule::StartupModule()
{
    RegisterPluginCommands();
}

void FCustomToolBarExtendModule::ShutdownModule()
{
    UnregisterPluginCommands();
    UnregisterMenus();
}

void FCustomToolBarExtendModule::RegisterPluginCommands()
{
    FCustomToolBarCommands::Register();

    PluginCommands = MakeShareable( new FUICommandList );

    PluginCommands->MapAction(
        FCustomToolBarCommands::Get().PluginAction,
        FExecuteAction::CreateRaw( this, &FCustomToolBarExtendModule::PluginActionButtonOnClicked ),
        FCanExecuteAction() );

    UToolMenus::RegisterStartupCallback( FSimpleMulticastDelegate::FDelegate::CreateRaw( this, &FCustomToolBarExtendModule::RegisterMenus ) );
}

void FCustomToolBarExtendModule::UnregisterPluginCommands()
{
    FCustomToolBarCommands::Unregister();
}

void FCustomToolBarExtendModule::RegisterMenus()
{
    // 必要写法
    FToolMenuOwnerScoped OwnerScoped( this );

    TObjectPtr<UToolMenu> Menu = UToolMenus::Get()->ExtendMenu( MenuName );

    FToolMenuSection& Section = Menu->FindOrAddSection( "CustomToolBar" );

    // FToolMenuEntry& Entry = Section.AddEntry( FToolMenuEntry::InitToolBarButton(
    //     "CustomToolBar",
    //     FToolUIActionChoice( FExecuteAction::CreateRaw( this, &FCustomToolBarExtendModule::PluginActionButtonOnClicked ) ),
    //     LOCTEXT( "CustomToolBar", "Test Custom Toolbar" ),
    //     LOCTEXT( "CustomToolBar", "Test Custom Toolbar" )
    //     ) );

    FToolMenuEntry& MenuEntry = Section.AddEntry( FToolMenuEntry::InitToolBarButton(
        "CustomToolBar",
        FToolUIActionChoice( FExecuteAction::CreateRaw( this, &FCustomToolBarExtendModule::PluginActionButtonOnClicked ) )
        ) );

    MenuEntry.Label = LOCTEXT( "CustomToolBar", "Test Custom Toolbar" );
    MenuEntry.StyleNameOverride = "CalloutToolbar";
    MenuEntry.SetCommandList( PluginCommands );

    // FToolMenuEntry& Entry = Section.AddEntry( FToolMenuEntry::InitMenuEntry(
    //         FCustomToolBarCommands::Get().PluginAction,
    //         LOCTEXT( "CustomToolBar", "Test Custom Toolbar" ),
    //         LOCTEXT( "CustomToolBar", "Test Custom Toolbar" )
    //         )
    //     );
}

void FCustomToolBarExtendModule::UnregisterMenus()
{
    UToolMenus::UnRegisterStartupCallback( this );
    UToolMenus::UnregisterOwner( this );
}


/* -------------------------- Plugin Action Func -------------------------- */
void FCustomToolBarExtendModule::PluginActionButtonOnClicked()
{
    UE_LOG( LogTemp, Error, TEXT("PluginAction Button Clicked") );
}

/* -------------------------- Plugin Action Func -------------------------- */

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE( FCustomToolBarExtendModule, CustomToolBarExtend )