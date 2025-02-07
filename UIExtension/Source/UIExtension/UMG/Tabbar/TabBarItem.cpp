// Fill out your copyright notice in the Description page of Project Settings.


#include "TabBarItem.h"

void UTabBarItem::InitTabBarItem(int32 InIndex)
{
	Index = InIndex;
}

void UTabBarItem::UnInitTabBarItem()
{
}

void UTabBarItem::OnTriggerOnClick()
{
	TabBarItemOnClickEvent.Broadcast(Index);
}
