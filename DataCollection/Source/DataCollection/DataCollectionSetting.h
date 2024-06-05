// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataCollectionObject.h"
#include "Engine/DeveloperSettings.h"
#include "DataCollectionSetting.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig)
class DATACOLLECTION_API UDataCollectionSetting : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	/* 是否启动收集系统 */
	UPROPERTY(config,EditAnywhere,Category="DataCollection")
	bool EnableDataCollection = false;
	
	/* 需要启动的收集子类 */
	UPROPERTY(config,EditAnywhere,Category="DataCollection",meta=(EditCondition="EnableDataCollection", EditConditionHides))
	TArray<TSubclassOf<UDataCollectionObject>> DataCollectionCls;

	virtual FName GetCategoryName() const override;
};
