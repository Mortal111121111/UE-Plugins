// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "HaowanSaveGameSetting.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig)
class HAOWANSAVEGAME_API UHaowanSaveGameSetting : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	virtual FName GetCategoryName() const override { return TEXT("Game"); };

	/* 若重名可以改这里 */
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Config)
	FString UserIndexSaveGameSlotName = "UserIndex";

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Config)
	TArray<FString> SaveNameSlotList;
};
