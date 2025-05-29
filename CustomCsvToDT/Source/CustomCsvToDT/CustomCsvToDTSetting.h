// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CustomCsvToDTSetting.generated.h"

/**
 * 
 */
UCLASS(Config = Engine, DefaultConfig)
class CUSTOMCSVTODT_API UCustomCsvToDTSetting : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	virtual FName GetCategoryName() const override { return TEXT( "Plugins" ); };

	UPROPERTY(config, EditAnywhere, meta = (FilePathFilter = "(*.bat)|*.bat"))
	FFilePath BatToolPath;

	UPROPERTY(config, EditAnywhere , meta = (FilePathFilter = "(*.csv)|*.csv"))
	TArray<FFilePath> CsvPaths;
};
