#include "CustomNewAssetEditor.h"

#include "AssetToolsModule.h"
#include "AssetTypeActions_NewAsset.h"
#include "IAssetTools.h"

#define LOCTEXT_NAMESPACE "FCustomNewAssetEditorModule"

class FAssetToolsModule;

EAssetTypeCategories::Type FCustomNewAssetEditorModule::NewAssetCategory;

void FCustomNewAssetEditorModule::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	const TSharedRef<IAssetTypeActions> FlowAssetActions = MakeShareable(new FAssetTypeActions_NewAsset());
	RegisteredAssetActions.Add(FlowAssetActions);
	AssetTools.RegisterAssetTypeActions(FlowAssetActions);

	NewAssetCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("NewAsset")), FText::FromString("NewAsset"));
}

void FCustomNewAssetEditorModule::ShutdownModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	for (TSharedRef<IAssetTypeActions> RegisteredAssetAction : RegisteredAssetActions)
	{
		AssetTools.UnregisterAssetTypeActions(RegisteredAssetAction);
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FCustomNewAssetEditorModule, CustomNewAssetEditor)