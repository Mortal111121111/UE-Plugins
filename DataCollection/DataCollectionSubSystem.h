// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataCollectionSubSystem.generated.h"

class UDataCollectionObject;
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
	void StartDataCollection(TSubclassOf<UDataCollectionObject> DataCollectionCls);
	void StopAllDataCollection();
	
private:
	UPROPERTY()
	TArray<TObjectPtr<UDataCollectionObject>> DataCollectionList;
};
