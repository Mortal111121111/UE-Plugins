// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTableEnhanceLib.generated.h"

/**
 * 
 */
UCLASS()
class DATATABLEENHANCE_API UDataTableEnhanceLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static bool CheckRowNames(TArray<UDataTable*> SubDTList,FText& CheckInfo);

	UFUNCTION(BlueprintCallable)
	static bool CheckDataTableStruct(const UDataTable* TargetDT , const UDataTable* OtherDT);
	
	UFUNCTION(BlueprintCallable)
	static bool AddRowByOtherDT(UDataTable* TargetDT, UDataTable* OtherDT , FName OtherRowName);

private:
	static bool CheckRepetitionRowName(const UDataTable* TargetDT , UDataTable* OtherDT , FString& CheckInfo);
	static void RemoveDataTableRow(UDataTable* TargetDT , FName RowName);
};
