// Copyright Epic Games, Inc. All Rights Reserved.

#include "DataCollection.h"

#include "LogListener.h"

#define LOCTEXT_NAMESPACE "FDataCollectionModule"

void FDataCollectionModule::StartupModule()
{
	CustomDevice = MakeShareable(new FLogListener());
	GLog->AddOutputDevice(CustomDevice.Get());
}

void FDataCollectionModule::ShutdownModule()
{
	if (CustomDevice.IsValid())
	{
		GLog->RemoveOutputDevice(CustomDevice.Get());
		CustomDevice.Reset();
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDataCollectionModule, DataCollection)