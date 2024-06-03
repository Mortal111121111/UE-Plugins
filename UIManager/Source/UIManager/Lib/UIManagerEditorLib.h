// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UIManager/UIManagerDefine.h"
#include "UIManagerEditorLib.generated.h"

/**
 * 
 */
UCLASS()
class UIMANAGER_API UUIManagerEditorLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

#if WITH_EDITOR
	UFUNCTION(BlueprintCallable)
	static void CreateUIDataTable();

	UFUNCTION(BlueprintCallable)
	static void GenerateUICreateInfo();
	
	static TMap<FName,FUIAssetInfo> FindAllUIAssetInfo();
	static FNotificationInfo CreateNotificationInfo(const FString& InInfoText);
	static bool CheckIsExistUIDataTable();
#endif
};
