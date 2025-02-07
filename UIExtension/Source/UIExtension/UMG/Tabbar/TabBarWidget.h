// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TabBarItem.h"
#include "Blueprint/UserWidget.h"
#include "Components/CheckBox.h"
#include "TabBarWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTabBarSelectedIndexChangedEvent,int32,SelectedIndex,int32,LastSelectedIndex);

/**
 * 
 */
UCLASS()
class UIEXTENSION_API UTabBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void RegisterContainerWidget(UPanelWidget* ContainerWidget);

	UFUNCTION(BlueprintCallable)
	void UnRegisterContainerWidget(UPanelWidget* ContainerWidget);
	
	UFUNCTION(BlueprintCallable)
	void SetDefaultSelectedTabBar(int32 Index = 0);

	UFUNCTION(BlueprintCallable)
	void ChangedSelectedItem(int32 NewIndex);

private:
	void BindTabBarDelegate(UTabBarItem* Widget , int32 Index);
	void OnTabBarItemClick(int32 Index);
	
public:
	UPROPERTY(BlueprintAssignable)
	FTabBarSelectedIndexChangedEvent TabBarSelectedIndexChangedEvent; 
	
private:
	UPROPERTY()
	TObjectPtr<UPanelWidget> TabBarContainer;

	UPROPERTY()
	TArray<TObjectPtr<UTabBarItem>> TabBarItems;

	int32 SelectedIndex = -1;
};
