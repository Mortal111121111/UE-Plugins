// Fill out your copyright notice in the Description page of Project Settings.


#include "TestAsyncAction.h"

UTestAsyncAction* UTestAsyncAction::TestAsync(const UObject* WorldContextObject, float Time)
{
	UTestAsyncAction* Node = NewObject<UTestAsyncAction>();
	Node->TempTime = Time;
	Node->WorldContextObject = WorldContextObject;
	
	FTimerDynamicDelegate Delegate;
	Delegate.BindUFunction(Node, FName("TriggerTest"));
	WorldContextObject->GetWorld()->GetTimerManager().SetTimer(Node->TimerHandle ,Delegate,Time,false);

	Node->RegisterWithGameInstance(WorldContextObject);
	UE_LOG(LogTemp,Error,TEXT("Start"));
	return Node;
}

void UTestAsyncAction::Activate()
{
	BP_Activate.Broadcast();
	UE_LOG(LogTemp,Error,TEXT("Activate"));
}

void UTestAsyncAction::TriggerTest()
{
	if( TempTime <=  5.0 )
	{
		Cancel.Broadcast();
	}
	else
	{
		Completed.Broadcast();
	}

	WorldContextObject->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	SetReadyToDestroy();
}
