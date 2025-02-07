// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "UObject/Interface.h"
#include "TabBarItemInterface.generated.h"


UINTERFACE()
class UTabBarItemInterface : public UInterface
{
	GENERATED_BODY()
};

class UIEXTENSION_API ITabBarItemInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UButton* GetTabBarTriggerButton();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UCheckBox* GetTabBarTriggerCheckBox();
};
