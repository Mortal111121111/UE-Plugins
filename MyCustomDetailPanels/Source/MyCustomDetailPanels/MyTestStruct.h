// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyTestStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FMyStruct
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int TestInt = 0;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FName TestName;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool TestBool = false;
 
};
