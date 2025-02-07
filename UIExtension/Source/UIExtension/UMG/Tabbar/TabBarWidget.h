// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "TabBarWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTabBarSelectedIndexChangedEvent,int32,SelectedIndex,int32,LastSelectedIndex);

UENUM(BlueprintType)
enum class ETabBarDelegateType : uint8
{
	None,
	Button,
	CheckBox,
	Custom,
};

/**
 * 
 */
UCLASS()
class UIEXTENSION_API UTabBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void RegisterContainerWidget(UPanelWidget* ContainerWidget,ETabBarDelegateType DelegateType);

	UFUNCTION(BlueprintCallable)
	void UnRegisterContainerWidget(UPanelWidget* ContainerWidget);
	
	UFUNCTION(BlueprintCallable)
	void SetDefaultSelectedTabBar(int32 Index = 0);

	UFUNCTION(BlueprintCallable)
	void ChangedSelectedTabBar(int32 NewIndex);

private:
	void BindTabBarDelegate(UWidget* Widget , int32 Index);
	void BindTabBarDelegate(UButton* ButtonWidget, int32 Index);
	void BindTabBarDelegate(UCheckBox* CheckBoxWidget, int32 Index);
	void BindTabBarDelegate(UUserWidget* UserWidget, int32 Index);

	UFUNCTION()
	void OnTabBarItemTrigger();
	
public:
	UPROPERTY(BlueprintAssignable)
	FTabBarSelectedIndexChangedEvent TabBarSelectedIndexChangedEvent; 
	
private:
	UPROPERTY()
	TObjectPtr<UPanelWidget> TabbarContainer;

	int32 SelectedIndex = 0;
};
