// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomToolBarExtend.h"

#include "CustomToolBarSettings.h"
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

    PluginCommands->MapAction( FCustomToolBarCommands::Get().PluginAction,
                               FExecuteAction::CreateRaw( this, &FCustomToolBarExtendModule::PluginActionButtonOnClicked ), FCanExecuteAction() );

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

    // FToolMenuEntry& Entry = Section.AddEntry( FToolMenuEntry::InitMenuEntry(
    //         FCustomToolBarCommands::Get().PluginAction,
    //         LOCTEXT( "CustomToolBar", "Test Custom Toolbar" ),
    //         LOCTEXT( "CustomToolBar", "Test Custom Toolbar" )
    //         )
    //     );

    /*FToolMenuEntry& MenuEntry = Section.AddEntry( FToolMenuEntry::InitToolBarButton(
        "CustomToolBar",
        FToolUIActionChoice( FExecuteAction::CreateRaw( this, &FCustomToolBarExtendModule::PluginActionButtonOnClicked ) )
        ) );*/

    FToolMenuEntry& MenuEntry = Section.AddEntry( FToolMenuEntry::InitComboButton(
        "CustomToolBar", FUIAction(), FOnGetContent::CreateRaw( this, &FCustomToolBarExtendModule::MakeComboButtonWidgets ),
        LOCTEXT( "CustomToolBar", "Test Custom Toolbar" ), LOCTEXT( "CustomToolBar", "Test Test Test Test Test" ), FSlateIcon(), false ) );

    MenuEntry.Label = LOCTEXT( "CustomToolBar", "Test Custom Toolbar" );
    MenuEntry.StyleNameOverride = "CalloutToolbar";
    MenuEntry.SetCommandList( PluginCommands );
}

void FCustomToolBarExtendModule::UnregisterMenus()
{
    UToolMenus::UnRegisterStartupCallback( this );
    UToolMenus::UnregisterOwner( this );
}

TSharedRef<SWidget> FCustomToolBarExtendModule::MakeComboButtonWidgets()
{
    FMenuBuilder MenuBuilder( true, PluginCommands );

    const UCustomToolBarSettings* Settings = GetDefault<UCustomToolBarSettings>();
    if(Settings->FuncCls == nullptr)
    {
        return MenuBuilder.MakeWidget();
    }

    TObjectPtr<UObject> FuncObjCDO = Settings->FuncCls->GetDefaultObject();
    MenuBuilder.BeginSection( "CustomToolBar", LOCTEXT( "CustomToolBar", "CustomToolBar" ) );
    {
        for(const FCustomToolBarInfo& ToolBarInfo : Settings->ToolBarInfos)
        {
            UFunction* Function = FuncObjCDO->FindFunction( *ToolBarInfo.FunctionName );
            if(Function == nullptr)
            {
                continue;
            }

            FText Label = FText::FromString( ToolBarInfo.ButtonName.IsEmpty() ? Function->GetName() : ToolBarInfo.ButtonName );
            FText ToolTips = FText::FromString( ToolBarInfo.ButtonToolTips.IsEmpty() ? Function->GetDesc() : ToolBarInfo.ButtonName );

            MenuBuilder.AddMenuEntry( Label, ToolTips, FSlateIcon(),
                                      FUIAction( FExecuteAction::CreateLambda(
                                          [FuncObjCDO,Function]()
                                          {
                                              FuncObjCDO->ProcessEvent( Function, nullptr );
                                          } ) ) );
        }
    }
    MenuBuilder.EndSection();

    return MenuBuilder.MakeWidget();
}

/* -------------------------- Plugin Action Func -------------------------- */
void FCustomToolBarExtendModule::PluginActionButtonOnClicked()
{
    UE_LOG( LogTemp, Error, TEXT( "PluginAction Button Clicked" ) );
}

void FCustomToolBarExtendModule::PluginActionButtonOnClicked_1()
{
    UE_LOG( LogTemp, Error, TEXT( "PluginAction Button 111111111 Clicked" ) );
}

void FCustomToolBarExtendModule::PluginActionButtonOnClicked_2()
{
    UE_LOG( LogTemp, Error, TEXT( "PluginAction Button 2222222222 Clicked" ) );
}

/* -------------------------- Plugin Action Func -------------------------- */

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE( FCustomToolBarExtendModule, CustomToolBarExtend )