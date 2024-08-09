// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataCollectionObject.h"
#include "LogListener.h"
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

	/* 文件夹名字 */
	UPROPERTY(config,EditAnywhere,Category="DataCollection",meta=(EditCondition="EnableDataCollection"))
	FName DirFileName = FName("LogCollection");

	/* log 筛选配置 */
	UPROPERTY(config,EditAnywhere,Category="DataCollection",meta=(EditCondition="EnableDataCollection"))
	TArray<FLogCollectionConfig> LogCollectionConfig;

	
	
	virtual FName GetCategoryName() const override;
};
