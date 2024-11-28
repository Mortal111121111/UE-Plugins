// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "UserIndexSaveGame.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class HAOWANSAVEGAME_API UUserIndexSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TArray<int> UserIndexList;

	UPROPERTY(BlueprintReadWrite)
	int CurrentIndex = 0;

	UPROPERTY(BlueprintReadWrite)
	int CurrentUserIndex = -1;
};
