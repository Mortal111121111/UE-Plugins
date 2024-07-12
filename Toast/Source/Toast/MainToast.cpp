// Fill out your copyright notice in the Description page of Project Settings.


#include "MainToast.h"

#include "ToastSystem.h"
#include "Blueprint/WidgetTree.h"

void UMainToast::Construct_Implementation()
{
	GetGameInstance()->GetSubsystem<UToastSystem>();
}

void UMainToast::AddItem(UObject* ItemData)
{
	if(ToastList.Num() >= ToastShowList)
	{
		return;
	}
	
	const TObjectPtr<UItemToast> NewItem = CreateItem(ItemData);
	if( !IsShowing )
	{
		StartShowItem(NewItem);
	}
}

UItemToast* UMainToast::CreateItem(UObject* ItemData)
{
	TObjectPtr<UItemToast> NewItem = WidgetTree->ConstructWidget<UItemToast>(ItemToastCls);
	
	NewItem->ToastShowFinishEvent.BindLambda([NewItem,this]
	{
		this->OnToastShowFinish(NewItem);
	});
	NewItem->InitItemToast(ItemData);
	ToastList.Add(NewItem);
	return NewItem;
}

void UMainToast::StartShowItem(UItemToast* ItemToast)
{
	VboxShowToast->AddChildToVerticalBox(ItemToast);
	ItemToast->ShowItemToast();
	IsShowing = true;
}

void UMainToast::StopShowItem()
{
	// 手动停后，把第一个播完
	const TObjectPtr<UItemToast> FirstItemToast = ToastList.begin()->Get();
	ToastList.Empty();
	ToastList.Add(FirstItemToast);
	IsShowing = false;
}

void UMainToast::OnToastShowFinish(UItemToast* ItemToast)
{
	VboxShowToast->RemoveChild(ItemToast);
	ToastList.Remove(ItemToast);

	if( ToastList.IsEmpty() )
	{
		IsShowing = false;
		return;
	}

	const TObjectPtr<UItemToast> NewItemToast = ToastList.begin()->Get();
	StartShowItem(NewItemToast);
}