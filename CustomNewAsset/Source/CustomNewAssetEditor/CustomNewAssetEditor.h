#pragma once

#include "CoreMinimal.h"
#include "AssetTypeCategories.h"
#include "Modules/ModuleManager.h"

class IAssetTypeActions;

class FCustomNewAssetEditorModule : public IModuleInterface
{
public:
    static EAssetTypeCategories::Type NewAssetCategory;

private:
    TArray<TSharedRef<IAssetTypeActions>> RegisteredAssetActions;
    
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
