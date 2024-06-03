// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "UIManagerSetting.generated.h"

/**
 * 
 */
UCLASS(Config="Game")
class UIMANAGER_API UUIManagerSetting : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(config,EditAnywhere,Category="DataTable",meta=(AllowedClasses="UIDataTable"))
	FSoftObjectPath UIDataTablePath;

public:
	virtual FName GetCategoryName() const override { return TEXT("UIManager"); }
};
