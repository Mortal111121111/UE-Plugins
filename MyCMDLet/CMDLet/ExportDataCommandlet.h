// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Commandlets/Commandlet.h"
#include "ExportDataCommandlet.generated.h"

/**
 * 
 */
UCLASS()
class MYCMDLET_API UExportDataCommandlet : public UCommandlet
{
	GENERATED_BODY()
	virtual int32 Main(const FString& params) override;

private:
	void WriteAnimDataToJson(const UAnimSequence* Sequence,const FString& BoneName);
	FString ParseAssetPath(const FString& AssetPath);
};
