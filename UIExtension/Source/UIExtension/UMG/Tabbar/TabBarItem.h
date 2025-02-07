// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TabBarItem.h"
#include "Blueprint/UserWidget.h"
#include "TabBarItem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FTabBarItemOnClickEvent,int32);

/**
 * 
 */
UCLASS()
class UIEXTENSION_API UTabBarItem : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitTabBarItem(int32 InIndex);
	void UnInitTabBarItem();

	UFUNCTION(BlueprintCallable)
	void OnTriggerOnClick();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_TabBarItemActivate();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_TabBarItemDeActivate();
	
public:
	FTabBarItemOnClickEvent TabBarItemOnClickEvent;

private:
	int32 Index;
};
