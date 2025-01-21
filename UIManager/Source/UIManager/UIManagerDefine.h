// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UIManagerDefine.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogUIManagerPlugin, Log, All);

UENUM(BlueprintType)
enum class EUIInputMode : uint8
{
	GameAndUI,
	UIOnly,
	GameOnly
};

USTRUCT(BlueprintType)
struct FUICreateInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UUserWidget> Cls;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsUP = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int ZOrder = 0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsOpenPauseGame = false;
};

struct FUIAssetInfo
{
	FUIAssetInfo(){};
	FUIAssetInfo(FName InName,TSubclassOf<UUserWidget> InCls) : Name(InName) , Cls(InCls) {};

	FName Name;
	TSubclassOf<UUserWidget> Cls;
};

struct FUIInfo
{
	FUIInfo(){};
	FUIInfo(UUserWidget* InWidget, bool InIsResetPauseGame,bool InIsResetShowMouse) :
		Widget(InWidget),bIsResetPauseGame(InIsResetPauseGame),bIsResetShowMouse(InIsResetShowMouse){};

	TObjectPtr<UUserWidget> Widget;
	bool bIsResetPauseGame = false;
	bool bIsResetShowMouse = false;
};
