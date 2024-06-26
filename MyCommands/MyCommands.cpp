// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyCommands.h"

#include "LevelEditor.h"
#include "TestCommand.h"
#include "Misc/MessageDialog.h"

#define LOCTEXT_NAMESPACE "FMyCommandsModule"

void FMyCommandsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// 命令的注册与绑定
	FTestCommands::Register();
	PluginCommandList = MakeShared<FUICommandList>();
	PluginCommandList->MapAction(
	  FTestCommands::Get().CommandA,   // Command
	  FExecuteAction::CreateRaw(this, &FMyCommandsModule::CommandAAction)
	);

	// 将其命令放入对应编辑器菜单
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	//扩展关卡编辑器的菜单
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout"
			, EExtensionHook::After
			, PluginCommandList
			, FMenuExtensionDelegate::CreateLambda([](FMenuBuilder& Builder)
				{
					Builder.AddMenuEntry(FTestCommands::Get().CommandA);
				}));
		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
}

void FMyCommandsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FTestCommands::Unregister();
	
	
}

void FMyCommandsModule::CommandAAction()
{
	FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("Execute CommandA"));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMyCommandsModule, MyCommands)