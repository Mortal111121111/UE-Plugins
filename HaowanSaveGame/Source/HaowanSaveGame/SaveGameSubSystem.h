// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveGameSubSystem.generated.h"

/**
 * 
 */
UCLASS()
class HAOWANSAVEGAME_API USaveGameSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
private:
	UPROPERTY(EditAnywhere)
	int CurrentUserIndex = 0;

public:
	UFUNCTION(BlueprintCallable)
	int GetCurrentUserIndex() const;

	UFUNCTION(BlueprintCallable)
	void SetCurrentUserIndex(int InUserIndex);
};
