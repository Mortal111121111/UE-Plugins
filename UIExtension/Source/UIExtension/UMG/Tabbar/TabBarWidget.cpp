// Fill out your copyright notice in the Description page of Project Settings.


#include "TabBarWidget.h"

void UTabBarWidget::RegisterContainerWidget(UPanelWidget* ContainerWidget)
{
	int32 ItemCounts = ContainerWidget->GetChildrenCount();
	if (ItemCounts <= 0)
	{
		UE_LOG(LogTemp,Warning,TEXT("TabBarWidget: ContainerWidget is empty"));
		return;
	}
	
	TabBarContainer = ContainerWidget;
	for (int32 i = 0; i < ItemCounts; ++i)
	{
		TObjectPtr<UTabBarItem> Item = Cast<UTabBarItem>(ContainerWidget->GetChildAt(i));
		if ( Item != nullptr )
		{
			BindTabBarDelegate(Item,i);
			TabBarItems.Add(Item);
		}
	}
}

void UTabBarWidget::UnRegisterContainerWidget(UPanelWidget* ContainerWidget)
{
	for (TObjectPtr<UTabBarItem> Item : TabBarItems)
	{
		Item->UnInitTabBarItem();
		Item->TabBarItemOnClickEvent.RemoveAll(this);
	}

	TabBarItems.Empty();
	TabBarContainer = nullptr;
}

void UTabBarWidget::SetDefaultSelectedTabBar(const int32 Index)
{
	SelectedIndex = Index;
	TabBarItems[Index]->BP_TabBarItemActivate();
}

void UTabBarWidget::BindTabBarDelegate(UTabBarItem* Widget, const int32 Index)
{
	Widget->InitTabBarItem(Index);
	Widget->TabBarItemOnClickEvent.AddUObject(this,&UTabBarWidget::OnTabBarItemClick);
}

void UTabBarWidget::OnTabBarItemClick(const int32 Index)
{
	ChangedSelectedItem(Index);
}

void UTabBarWidget::ChangedSelectedItem(const int32 NewIndex)
{
	if (!TabBarItems.Contains(NewIndex) || SelectedIndex == NewIndex )
	{
		return;
	}

	TabBarItems[NewIndex]->BP_TabBarItemActivate();
	TabBarItems[SelectedIndex]->BP_TabBarItemActivate();
	
	TabBarSelectedIndexChangedEvent.Broadcast(NewIndex,SelectedIndex);
	SelectedIndex = NewIndex;
}


