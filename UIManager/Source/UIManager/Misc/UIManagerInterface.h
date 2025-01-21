// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InstancedStruct.h"
#include "UObject/Interface.h"
#include "UIManagerInterface.generated.h"

/**
 * 
 */
UINTERFACE(BlueprintType)
class UIMANAGER_API UUIManagerInterface : public UInterface
{
	GENERATED_BODY()
};

class UIMANAGER_API IUIManagerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void OnOpenUI(const FInstancedStruct& Params);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void OnCloseUI();
};