// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManagerEditorLib.h"

#include "AssetToolsModule.h"
#include "DataTableEditorUtils.h"
#include "WidgetBlueprint.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Framework/Notifications/NotificationManager.h"
#include "UIManager/UIManagerDefine.h"
#include "UIManager/DataTable/UIDataTable.h"
#include "UIManager/Misc/UIManagerSetting.h"
#include "Widgets/Notifications/SNotificationList.h"

#if WITH_EDITOR
class IAssetRegistry;
class UUIManagerSetting;
class FAssetToolsModule;

void UUIManagerEditorLib::CreateUIDataTable()
{
	if(CheckIsExistUIDataTable())
	{
		return;
	}
	
	const TObjectPtr<UUIDataTable> NewDataTable = NewObject<UUIDataTable>(GetTransientPackage(), "DT_UIManager", RF_Public);
	if (NewDataTable != nullptr)
	{
		const FString AssetPath = "/Game/DT_UIManager";
		const FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
		const UObject* Asset = AssetToolsModule.Get().CreateAsset(NewDataTable->GetName(), FPackageName::GetLongPackagePath(AssetPath), UUIDataTable::StaticClass(), nullptr);
		if( Asset != nullptr )
		{
			const UUIManagerSetting* Settings = GetDefault<UUIManagerSetting>();
			const_cast<UUIManagerSetting*>(Settings)->UIDataTablePath = FSoftObjectPath(Asset);

			const FNotificationInfo Info = CreateNotificationInfo("DT_UIManager Create Success");
			if (const TSharedPtr<SNotificationItem> Notification = FSlateNotificationManager::Get().AddNotification(Info))
			{
				Notification->SetCompletionState(SNotificationItem::CS_Success);
			}
			GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(Asset);
		}
	}
}

void UUIManagerEditorLib::GenerateUICreateInfo()
{
	const UUIManagerSetting* Settings = GetDefault<UUIManagerSetting>();
	const TObjectPtr<UUIDataTable> UIDataTable = Cast<UUIDataTable>(Settings->UIDataTablePath.TryLoad());
	if( UIDataTable == nullptr )
	{
		return;
	}

	TMap<FName,FUIAssetInfo> AllUIAssetInfo = FindAllUIAssetInfo();

	// 表里有，但是没有遍历到，删除
	for (FName RowName : UIDataTable->GetRowNames())
	{
		if(!AllUIAssetInfo.Find(RowName))
		{
			UIDataTable->RemoveRow(RowName);
		}
	}

	// 遍历到的 UI , 添加进表
	for (const TTuple<FName, FUIAssetInfo> AssetInfo : AllUIAssetInfo) 
	{
		const FUIAssetInfo Value = AssetInfo.Value;
		if(!UIDataTable->GetRowMap().Find(Value.Name))
		{
			FUICreateInfo Info = FUICreateInfo();
			Info.Cls = Value.Cls;
			UIDataTable->AddRow(Value.Name,Info);
		}
	}
	FDataTableEditorUtils::BroadcastPostChange(UIDataTable, FDataTableEditorUtils::EDataTableChangeInfo::RowList);
}

TMap<FName,FUIAssetInfo> UUIManagerEditorLib::FindAllUIAssetInfo()
{
	const IAssetRegistry& AssetRegistry = IAssetRegistry::GetChecked();
	TArray<FAssetData> AllUIAssetData;
	AssetRegistry.GetAssetsByClass(FTopLevelAssetPath(UWidgetBlueprint::StaticClass()),AllUIAssetData , true);
	
	TMap<FName,FUIAssetInfo> OutInfos;
	for (FAssetData AssetData : AllUIAssetData)
	{
		if(FPaths::IsUnderDirectory(AssetData.PackageName.ToString() , "/Game"))
		{
			FName UIName = FName(AssetData.AssetName);
			UObject* UIObj = LoadObject<UObject>(nullptr,*AssetData.GetObjectPathString());
			UBlueprint* BPObj = Cast<UBlueprint>(UIObj);
			if(BPObj != nullptr)
			{
				OutInfos.Add(UIName,FUIAssetInfo(UIName, *BPObj->GeneratedClass));	
			}
		}
	}
	return OutInfos;
}

FNotificationInfo UUIManagerEditorLib::CreateNotificationInfo(const FString& InInfoText)
{
	const FText InfoText = FText::FromString(InInfoText);

	FMessageLog EditorInfo("UIManagerPlugin");
	EditorInfo.Info(InfoText);

	FNotificationInfo Info(InfoText);
	Info.bFireAndForget = true;
	Info.bUseThrobber = false;
	Info.FadeOutDuration = 0.5f;
	Info.ExpireDuration = 5.0f;
	return Info;
}

bool UUIManagerEditorLib::CheckIsExistUIDataTable()
{
	const IAssetRegistry& AssetRegistry = IAssetRegistry::GetChecked();
	TArray<FAssetData> OutAssetData;
	AssetRegistry.GetAssetsByClass(FTopLevelAssetPath(UUIDataTable::StaticClass()),OutAssetData , false);
	if(OutAssetData.Num() >= 1)
	{
		if (GEditor->GetEditorWorldContext().World())
		{
			const FNotificationInfo Info = CreateNotificationInfo("DT_UIManager Exist");
			if (const TSharedPtr<SNotificationItem> Notification = FSlateNotificationManager::Get().AddNotification(Info))
			{
				Notification->SetCompletionState(SNotificationItem::CS_Fail);
			}
			GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(OutAssetData[0].GetAsset());
		}
		return true;
	}
	return false;
}
#endif