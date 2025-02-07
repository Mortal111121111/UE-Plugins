// Fill out your copyright notice in the Description page of Project Settings.


#include "TabBarWidget.h"

void UTabBarWidget::RegisterContainerWidget(UPanelWidget* ContainerWidget, ETabBarDelegateType DelegateType)
{
	int32 ItemCounts = ContainerWidget->GetChildrenCount();
	if (ItemCounts <= 0)
	{
		UE_LOG(LogTemp,Warning,TEXT("TabBarWidget: ContainerWidget is empty"));
		return;
	}
	
	TabbarContainer = ContainerWidget;
	for (int32 i = 0; i < ItemCounts; ++i)
	{
		BindTabBarDelegate(ContainerWidget->GetChildAt(i),i);
	}
	
}

void UTabBarWidget::BindTabBarDelegate(UWidget* Widget, int32 Index)
{
	UE_LOG(LogTemp,Warning,TEXT("BindTabBarDelegate Error, WidgetType : %s ,Error"),*Widget->GetClass()->GetName());
}

void UTabBarWidget::BindTabBarDelegate(UButton* ButtonWidget, int32 Index)
{
	ButtonWidget->OnClicked.Add(this,&UTabBarWidget::OnTabBarItemTrigger);
}

void UTabBarWidget::BindTabBarDelegate(UCheckBox* CheckBoxWidget, int32 Index)
{
	CheckBoxWidget->OnCheckStateChanged.AddDynamic(this,&UTabBarWidget::OnTabBarItemTrigger);
}

void UTabBarWidget::BindTabBarDelegate(UUserWidget* UserWidget, int32 Index)
{
}

void UTabBarWidget::OnTabBarItemTrigger()
{
}
