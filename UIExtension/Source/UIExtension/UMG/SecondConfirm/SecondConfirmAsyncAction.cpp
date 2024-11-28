// Fill out your copyright notice in the Description page of Project Settings.


#include "SecondConfirmAsyncAction.h"

USecondConfirmAsyncAction* USecondConfirmAsyncAction::SecondConfirm(const UObject* WorldContextObject,USecondConfirmWnd* Widget)
{
	USecondConfirmAsyncAction* Node = NewObject<USecondConfirmAsyncAction>();
	Widget->SecondConfirmEvent.AddDynamic(Node,&USecondConfirmAsyncAction::OnSecondConfirmEvent);
	Node->RegisterWithGameInstance(WorldContextObject);
	return Node;
}

void USecondConfirmAsyncAction::OnSecondConfirmEvent(bool bIsConfirm)
{
	if( bIsConfirm )
	{
		Confirm.Broadcast();
	}
	else
	{
		Cancel.Broadcast();
	}

	SetReadyToDestroy();
}
