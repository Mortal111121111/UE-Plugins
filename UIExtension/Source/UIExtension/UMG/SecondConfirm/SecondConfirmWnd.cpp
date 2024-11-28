// Fill out your copyright notice in the Description page of Project Settings.


#include "SecondConfirmWnd.h"

void USecondConfirmWnd::Confirm()
{
	SecondConfirmEvent.Broadcast(true);
}

void USecondConfirmWnd::Cancel()
{
	SecondConfirmEvent.Broadcast(false);
}
