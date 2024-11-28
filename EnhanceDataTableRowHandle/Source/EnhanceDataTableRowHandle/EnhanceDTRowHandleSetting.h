// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "EnhanceDTRowHandleSetting.generated.h"


USTRUCT(BlueprintType)
struct FEnhanceDTRowHandleBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName Row;

	bool operator==(const FEnhanceDTRowHandleBase& Other) const
	{
		return Row == Other.Row;
	}
};

USTRUCT()
struct FEnhanceDTRowHandleInfo
{
	GENERATED_BODY()

	UPROPERTY(config,EditAnywhere)
	FString StructName;

	UPROPERTY(config,EditAnywhere,meta=(AllowedClasses="/Script/Engine.DataTable"))
	FSoftObjectPath DataTable;
};

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig)
class ENHANCEDATATABLEROWHANDLE_API UEnhanceDTRowHandleSetting : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	virtual FName GetCategoryName() const override { return TEXT("Game"); };

public:
	UPROPERTY(config,EditAnywhere,meta=(ConfigRestartRequired=true))
	TArray<FEnhanceDTRowHandleInfo> EnhanceStructPaths;

};
