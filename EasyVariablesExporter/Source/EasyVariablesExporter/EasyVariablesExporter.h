// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"


typedef TFunction< bool(const UObject*, const FProperty*) > FCustomNeedExportFunc;

class FEasyVariablesExporterModule : public IModuleInterface
{

protected:
	TMap<FString,FCustomNeedExportFunc> CustomNeedExportMap;

public:
	void EASYVARIABLESEXPORTER_API RegisterCustomNeedExportFunc(FString Key, FCustomNeedExportFunc CustomFunc);
	void EASYVARIABLESEXPORTER_API UnregisterCustomNeedExportFunc(FString Key);

public:
	const TMap<FString,FCustomNeedExportFunc>& GetCustomNeedExportMap();
	
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
