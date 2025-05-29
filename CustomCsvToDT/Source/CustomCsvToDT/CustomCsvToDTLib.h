// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CustomCsvToDTLib.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FParseCsvInfo
{
	GENERATED_BODY()

public:
	// 字段名
	UPROPERTY(BlueprintReadOnly)
	FString FieldName;

	// 字段类型
	UPROPERTY(BlueprintReadOnly)
	FString FieldType;

	// 该字段下的所有值
	UPROPERTY(BlueprintReadOnly)
	TArray<FString> Content;
};

/**
 * 
 */
UCLASS()
class CUSTOMCSVTODT_API UCustomCsvToDTLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

#if WITH_EDITOR
	UFUNCTION(BlueprintCallable)
	static TMap<FString,FParseCsvInfo> ReadCsvFile(const FString& CsvFilePath);

	/* 支持蓝图声明的结构体 */
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "Editor Scripting | DataTable", meta=(AutoCreateRefTerm="RowName", CustomStructureParam="RowData"))
	static void AddDataTableRow(UDataTable* const DataTable, const FName& RowName, const FTableRowBase& RowData);
	DECLARE_FUNCTION(execAddDataTableRow);

	UFUNCTION(BlueprintCallable)
	static void RunCsvToUTF8Bat();

	UFUNCTION( BlueprintCallable )
	static void NotifyDataTableChanged( UDataTable* DataTable );

	UFUNCTION( BlueprintCallable )
	static void ClearDataTable( UDataTable* DataTable );

	UFUNCTION( BlueprintPure )
	static FText FStringToFText(const FString& InStr );

	UFUNCTION( BlueprintPure )
	static TMap<FString,FString> GetSettingCsvPaths();

#endif
};
