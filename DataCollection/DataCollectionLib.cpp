// Fill out your copyright notice in the Description page of Project Settings.


#include "DataCollectionLib.h"

#include "DataCollectionSubSystem.h"

class UDataCollectionSubSystem;

void UDataCollectionLib::StartDataCollection(UObject* WorldContext,TSubclassOf<UDataCollectionObject> Cls)
{
	const TObjectPtr<UWorld> World = WorldContext->GetWorld();
	if( World == nullptr )
	{
		return;
	}
	
	const TObjectPtr<UDataCollectionSubSystem> Manager = World->GetGameInstance()->GetSubsystem<UDataCollectionSubSystem>();
	if( Manager == nullptr )
	{
		return;
	}
	
	Manager->StartDataCollection(Cls);
}
