// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MyNewAsset.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class CUSTOMNEWASSET_API UMyNewAsset : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int TestInt = 0;
};
