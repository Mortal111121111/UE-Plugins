// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetTypeActions_NewAsset.h"

#include "CustomNewAssetEditor.h"
#include "CustomNewAsset/MyNewAsset.h"

FText FAssetTypeActions_NewAsset::GetName() const
{
	return  FText::FromString("MyNewAsset");
}

uint32 FAssetTypeActions_NewAsset::GetCategories()
{
	return FCustomNewAssetEditorModule::NewAssetCategory;
}

FColor FAssetTypeActions_NewAsset::GetTypeColor() const
{
	return FColor(255, 196, 128);
}

UClass* FAssetTypeActions_NewAsset::GetSupportedClass() const
{
	return UMyNewAsset::StaticClass();
}

// void FAssetTypeActions_NewAsset::OpenAssetEditor(const TArray<UObject*>& InObjects,TSharedPtr<IToolkitHost> EditWithinLevelEditor)
// {
// 	FAssetTypeActions_Base::OpenAssetEditor(InObjects, EditWithinLevelEditor);
// }
