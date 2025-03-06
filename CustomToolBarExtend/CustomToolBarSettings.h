// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CustomToolBarSettings.generated.h"

USTRUCT( Blueprintable, BlueprintType )
struct FCustomToolBarInfo
{
    GENERATED_BODY()

public:
    UPROPERTY( BlueprintReadOnly, EditAnywhere )
    FString ButtonName;

    UPROPERTY( BlueprintReadOnly, EditAnywhere )
    FString ButtonToolTips;

    UPROPERTY( BlueprintReadOnly, EditAnywhere )
    FString FunctionName;
};

/**
 * 
 */
UCLASS( Config = Game, DefaultConfig )
class CUSTOMTOOLBAREXTEND_API UCustomToolBarSettings : public UDeveloperSettings
{
    GENERATED_BODY()

protected:
    virtual FName GetCategoryName() const override
    {
        return FName( "Game" );
    };

public:
    /* 自定义按钮点击后将会调用其中绑定的函数 */
    UPROPERTY( Config, EditAnywhere, meta=(ConfigRestartRequired=true) )
    TSubclassOf<UObject> FuncCls;

    /* 自定义按钮信息列表 */
    UPROPERTY( config, EditAnywhere, meta=(ConfigRestartRequired=true) )
    TArray<FCustomToolBarInfo> ToolBarInfos;
};