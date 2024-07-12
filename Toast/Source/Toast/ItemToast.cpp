// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemToast.h"



void UItemToast::ShowFinish()
{
	ToastShowFinishEvent.Execute();
}

void UItemToast::ShowItemToast_Implementation()
{
	// Empty
}

void UItemToast::InitItemToast_Implementation(UObject* InitData)
{
	// Empty
}
