// Fill out your copyright notice in the Description page of Project Settings.


#include "DataTableEnhanceLib.h"

#include "Kismet/DataTableFunctionLibrary.h"

bool UDataTableEnhanceLib::CheckRowNames(TArray<UDataTable*> SubDTList , FText& CheckInfo)
{
	FString RepetitionRowNameInfo = "";
	for (const TObjectPtr<UDataTable> TargetDT : SubDTList)
	{
		for (const TObjectPtr<UDataTable> OtherDT : SubDTList)
		{
			if(TargetDT != OtherDT && CheckRepetitionRowName(TargetDT,OtherDT,RepetitionRowNameInfo))
			{
				CheckInfo = FText::FromString(RepetitionRowNameInfo);
				return false;
			}
		}
	}
	CheckInfo = FText::FromString(RepetitionRowNameInfo);
	return true;
}

bool UDataTableEnhanceLib::AddRowByOtherDT(UDataTable* TargetDT, UDataTable* OtherDT , FName OtherRowName)
{
	if(!CheckDataTableStruct(TargetDT,OtherDT))
	{
		return false;
	}
	
	const UScriptStruct* StructType = TargetDT->GetRowStruct();
	RemoveDataTableRow(TargetDT,OtherRowName);

	const uint8* RowPtr = OtherDT->FindRowUnchecked(OtherRowName);
	uint8* NewRawRowData = (uint8*)FMemory::Malloc(StructType->GetStructureSize());
	StructType->InitializeStruct(NewRawRowData);
	StructType->CopyScriptStruct(NewRawRowData, RowPtr);
	const_cast<TMap<FName, uint8*>&>(TargetDT->GetRowMap()).Add(OtherRowName,NewRawRowData);
	
	return true;
}

bool UDataTableEnhanceLib::CheckRepetitionRowName(const UDataTable* TargetDT, UDataTable* OtherDT, FString& CheckInfo)
{
	TArray<FName> TargetRowNames = TargetDT->GetRowNames();

	bool IsExist = false;
	for (const FName TargetRowName : TargetRowNames)
	{
		if(UDataTableFunctionLibrary::DoesDataTableRowExist(OtherDT,TargetRowName))
		{
			IsExist = true;
			CheckInfo += FString::Printf(TEXT("%s and %s ExistRepetition , RepetitionRowName : %s. \r\n"), *TargetDT->GetName(), *OtherDT->GetName(),*TargetRowName.ToString());
		}
	}

	return IsExist;
}

void UDataTableEnhanceLib::RemoveDataTableRow(UDataTable* TargetDT, FName RowName)
{
	const UScriptStruct* EmptyUsingStruct = TargetDT->GetRowStruct();

	uint8* RowData = nullptr;
	const_cast<TMap<FName, uint8*>&>(TargetDT->GetRowMap()).RemoveAndCopyValue(RowName, RowData);
		
	if (RowData)
	{
		EmptyUsingStruct->DestroyStruct(RowData);
		FMemory::Free(RowData);
	}
}

bool UDataTableEnhanceLib::CheckDataTableStruct(const UDataTable* TargetDT, const UDataTable* OtherDT)
{
	return TargetDT->RowStruct == OtherDT->RowStruct;
}



