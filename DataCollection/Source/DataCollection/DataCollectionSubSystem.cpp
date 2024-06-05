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
	StopDataCollection();
}

void UDataCollectionSubSystem::StartDataCollection()
{
	const UDataCollectionSetting* Settings = GetDefault<UDataCollectionSetting>();
	for (TSubclassOf<UDataCollectionObject> Cls : Settings->DataCollectionCls)
	{
		const TObjectPtr<UDataCollectionObject> DCObject = NewObject<UDataCollectionObject>(this, Cls, NAME_None, RF_NoFlags);
		DCObject->StartDataCollection(Cls->GetName());
		DataCollectionList.Add(DCObject);
	}
}

void UDataCollectionSubSystem::StopDataCollection()
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