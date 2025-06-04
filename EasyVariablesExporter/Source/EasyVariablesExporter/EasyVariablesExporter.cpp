// Copyright Epic Games, Inc. All Rights Reserved.

#include "EasyVariablesExporter.h"

#define LOCTEXT_NAMESPACE "FEasyVariablesExporterModule"

void FEasyVariablesExporterModule::RegisterCustomNeedExportFunc(FString Key, FCustomNeedExportFunc CustomFunc)
{
	CustomNeedExportMap.Add(Key,CustomFunc);
}

void FEasyVariablesExporterModule::UnregisterCustomNeedExportFunc(FString Key)
{
	CustomNeedExportMap.Remove(Key);
}

const TMap<FString, FCustomNeedExportFunc>& FEasyVariablesExporterModule::GetCustomNeedExportMap()
{
	return CustomNeedExportMap;
}

void FEasyVariablesExporterModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FEasyVariablesExporterModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	CustomNeedExportMap.Empty();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEasyVariablesExporterModule, EasyVariablesExporter)