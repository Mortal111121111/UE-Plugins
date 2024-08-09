// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataCollectionObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DataCollectionLib.generated.h"

/**
 * 
 */
UCLASS()
class DATACOLLECTION_API UDataCollectionLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,meta=(WorldContext="WorldContext",DeterminesOutputType = "Cls"))
	static void StartDataCollection(UObject* WorldContext,TSubclassOf<UDataCollectionObject> Cls);
};
