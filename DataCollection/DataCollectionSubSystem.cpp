// Fill out your copyright notice in the Description page of Project Settings.


#include "DataCollectionSubSystem.h"

#include "DataCollectionSetting.h"

bool UDataCollectionSubSystem::ShouldCreateSubsystem(UObject* Outer) const
{
	const UDataCollectionSetting* Settings = GetDefault<UDataCollectionSetting>();
	return Settings->EnableDataCollection;
}

void UDataCollectionSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
}

void UDataCollectionSubSystem::Deinitialize()
{
	StopAllDataCollection();
}

void UDataCollectionSubSystem::StartDataCollection(TSubclassOf<UDataCollectionObject> DataCollectionCls)
{
	const TObjectPtr<UDataCollectionObject> DCObject = NewObject<UDataCollectionObject>(this, DataCollectionCls, NAME_None, RF_NoFlags);
	DCObject->StartDataCollection(DataCollectionCls->GetName());
	DataCollectionList.Add(DCObject);
}

void UDataCollectionSubSystem::StopAllDataCollection()
{
	for (const TObjectPtr<UDataCollectionObject> Collection : DataCollectionList)
	{
		if(IsValid(Collection))
		{
			Collection->StopDataCollection();
		}
	}

	DataCollectionList.Empty();
}
