// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EasyVariablesExporter/EasyVariablesExporterDefine.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EasyVariablesExporterFuncLib.generated.h"

/**
 * 
 */

UCLASS()
class EASYVARIABLESEXPORTER_API UEasyVariablesExporterFuncLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static bool ExportParamFromObject(UObject* Object, FEasyExporterParamsMap& OutParamsMap);

	UFUNCTION(BlueprintCallable)
	static bool ImportParamToObject(const FEasyExporterParamsMap& ParamsMap);

	UFUNCTION(BlueprintPure,meta=(DeterminesOutputType = "InClass"))
	static UObject* GetClassDefaultObject(TSubclassOf<UObject> InClass);

protected:
	static bool ShouldPropertyExport(const UObject* Owner, const FProperty* Property);

	static bool ExportEnumerateProperty(UObject* Object, const FProperty* Property, FEasyExporterParam& OutParam);
	static bool ExportBooleanProperty(UObject* Object, const FProperty* Property, FEasyExporterParam& OutParam);
	static bool ExportNumericProperty(UObject* Object, const FProperty* Property, FEasyExporterParam& OutParam);
	static bool ExportStructureProperty(UObject* Object, const FProperty* Property, FEasyExporterParam& OutParam);
	static bool ExportVectorProperty(UObject* Object, const FProperty* Property, FEasyExporterParam& OutParam);
	static bool ExportStringProperty(UObject* Object, const FProperty* Property, FEasyExporterParam& OutParam);
	static bool ExportNameProperty(UObject* Object, const FProperty* Property, FEasyExporterParam& OutParam);
	static bool ExportTextProperty(UObject* Object, const FProperty* Property, FEasyExporterParam& OutParam);

	static bool ImportEnumerateProperty(UObject* Object, const FProperty* Property, const FEasyExporterParam& Param);
	static bool ImportBooleanProperty(UObject* Object, const FProperty* Property, const FEasyExporterParam& Param);
	static bool ImportNumericProperty(UObject* Object, const FProperty* Property, const FEasyExporterParam& Param);
	static bool ImportStructureProperty(UObject* Object, const FProperty* Property, const FEasyExporterParam& Param);
	static bool ImportVectorProperty(UObject* Object, const FProperty* Property, const FEasyExporterParam& Param);
	static bool ImportStringProperty(UObject* Object, const FProperty* Property, const FEasyExporterParam& Param);
	static bool ImportNameProperty(UObject* Object, const FProperty* Property, const FEasyExporterParam& Param);
	static bool ImportTextProperty(UObject* Object, const FProperty* Property, const FEasyExporterParam& Param);
};
