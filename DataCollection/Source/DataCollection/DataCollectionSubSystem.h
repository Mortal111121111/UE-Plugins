// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataCollectionSubSystem.generated.h"

/**
 * 
 */
UCLASS()
class DATACOLLECTION_API UDataCollectionSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	UFUNCTION(BlueprintCallable)
	void StartDataCollection();

	UFUNCTION(BlueprintCallable)
	void StopDataCollection();

private:
	UPROPERTY()
	TArray<TObjectPtr<UDataCollectionObject>> DataCollectionList;
};
