// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "UIExtensionSetting.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig)
class UIEXTENSION_API UUIExtensionSetting : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	virtual FName GetCategoryName() const override { return TEXT("Game"); };

public:
	UPROPERTY(config,EditAnywhere,meta=(ConfigRestartRequired=true))
	float EnhanceTextHeight = 100.0;

	UPROPERTY(config,EditAnywhere,meta=(ConfigRestartRequired=true))
	float EnhanceFontSize = 20.0;
};
