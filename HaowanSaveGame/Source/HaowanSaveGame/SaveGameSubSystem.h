// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveGameSubSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUserIndexChangedEvent,int,CurrentUserIndex,int,OldUserIndex);
/**
 * 
 */
UCLASS()
class HAOWANSAVEGAME_API USaveGameSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
public:
	UPROPERTY(BlueprintAssignable)
	FUserIndexChangedEvent UserIndexChangedEvent;
	
private:
	UPROPERTY(EditAnywhere)
	int CurrentUserIndex = -1;

public:
	UFUNCTION(BlueprintCallable)
	int GetCurrentUserIndex() const;

	UFUNCTION(BlueprintCallable)
	void SetCurrentUserIndex(int InUserIndex);

	
};
