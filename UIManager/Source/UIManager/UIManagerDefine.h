// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Blueprint/UserWidget.h"
#include "UIManagerDefine.generated.h"

DECLARE_LOG_CATEGORY_EXTERN( LogUIManagerPlugin, Log, All );

UENUM( BlueprintType )
enum class EUIInputMode : uint8
{
    GameAndUI,
    UIOnly,
    GameOnly
};

USTRUCT( BlueprintType )
struct FUICreateInfo : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY( EditAnywhere, BlueprintReadWrite )
    TSubclassOf<UUserWidget> Cls;

    UPROPERTY( EditAnywhere, BlueprintReadWrite )
    bool bIsUP = false;

    UPROPERTY( EditAnywhere, BlueprintReadWrite )
    int ZOrder = 0;

    UPROPERTY( EditAnywhere, BlueprintReadWrite )
    bool bIsOpenPauseGame = false;

    UPROPERTY( EditAnywhere, BlueprintReadWrite )
    bool bIsShowMouse = true;
};

USTRUCT()
struct FUIAssetInfo
{
    GENERATED_BODY()

    FUIAssetInfo()
    {
    };

    FUIAssetInfo( FName InName, TSubclassOf<UUserWidget> InCls ) : Name( InName ), Cls( InCls )
    {
    };

    FName Name;

    UPROPERTY()
    TSubclassOf<UUserWidget> Cls;
};

USTRUCT()
struct FUIInfo
{
    GENERATED_BODY()

    FUIInfo()
    {
    };

    FUIInfo( UUserWidget* InWidget, bool InIsResetPauseGame, bool InIsResetShowMouse ) :
        Widget( InWidget ), bIsResetPauseGame( InIsResetPauseGame ), bIsResetShowMouse( InIsResetShowMouse )
    {
    };

    UPROPERTY()
    TWeakObjectPtr<UUserWidget> Widget;

    bool bIsResetPauseGame = false;
    bool bIsResetShowMouse = false;
};