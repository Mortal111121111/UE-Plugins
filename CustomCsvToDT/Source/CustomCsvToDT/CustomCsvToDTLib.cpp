// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomCsvToDTLib.h"

#include "DataTableEditorUtils.h"
#include "CustomCsvToDTModule.h"
#include "CustomCsvToDTSetting.h"
#include "Serialization/Csv/CsvParser.h"

#if WITH_EDITOR

TMap<FString,FParseCsvInfo> UCustomCsvToDTLib::ReadCsvFile(const FString& CsvFilePath)
{
	TMap<FString,FParseCsvInfo> Out;
	TArray<FParseCsvInfo> CsvValue;
	
	FString CSVContent;
	if(!FFileHelper::LoadFileToString(CSVContent,*CsvFilePath))
	{
		return Out;
	}

	const FCsvParser Parser(CSVContent);
	const FCsvParser::FRows& Rows = Parser.GetRows();

	if (!Rows.IsValidIndex(0) || !Rows.IsValidIndex(1))
	{
		UE_LOG(LogExportCsv, Error, TEXT("Csv %s Export Error! ,FieldName or FieldType is Valid!"), *CsvFilePath);
		return Out;
	}
	
	for (int i = 0; i < Rows[0].Num(); ++i)
	{
		FString FieldName = WCHAR_TO_TCHAR(Rows[0][i]);
		FString FieldType = WCHAR_TO_TCHAR(Rows[1][i]);
		if (FieldName.IsEmpty() || FieldType.IsEmpty())
		{
			UE_LOG(LogExportCsv, Error, TEXT("Csv %s Export Error! ,Column : %d FieldName or FieldType Error!"), *CsvFilePath , i);
			break;
		}
		
		FParseCsvInfo Info;
		Info.FieldName = FieldName;
		Info.FieldType = FieldType;
	
		TArray<FString> NewContent;
		for (int j = 2; j < Rows.Num(); ++j)
		{
			if (Rows.IsValidIndex(j) && Rows[j].IsValidIndex(i))
			{
				NewContent.Add(WCHAR_TO_TCHAR(Rows[j][i]));
			}
			else
			{
				NewContent.Add("");
				UE_LOG(LogExportCsv, Error, TEXT("Csv Data Error! , Rows : %d , Column : %d"), j,i);
			}
		}
		Info.Content = NewContent;
		CsvValue.Add(Info);
	}
	
	for (const FParseCsvInfo& Value : CsvValue)
	{
		Out.Add(Value.FieldName, Value);
	}
	
	return Out;
}

// UDataTableFunctionLibrary 提供的 AddDataTableRow 对于蓝图声明的 Struct 不生效
void UCustomCsvToDTLib::AddDataTableRow(UDataTable* const DataTable, const FName& RowName,const FTableRowBase& RowData)
{
	if (!DataTable)
	{
		UE_LOG(LogDataTable, Error, TEXT("AddDataTableRow - The DataTable is invalid."));
		return;
	}

	DataTable->AddRow(RowName, RowData);
}

DEFINE_FUNCTION(UCustomCsvToDTLib::execAddDataTableRow)
{
	P_GET_OBJECT(UDataTable, DataTable);
	P_GET_PROPERTY(FNameProperty, RowName);

	Stack.MostRecentPropertyAddress = nullptr;
	Stack.MostRecentPropertyContainer = nullptr;
	Stack.StepCompiledIn<FStructProperty>(nullptr);

	const FTableRowBase* const RowData = reinterpret_cast<FTableRowBase*>(Stack.MostRecentPropertyAddress);

	P_FINISH;

	if (!DataTable)
	{
		FBlueprintExceptionInfo ExceptionInfo(
			EBlueprintExceptionType::AccessViolation,
			NSLOCTEXT("AddDataTableRow", "MissingTableInput", "Failed to resolve the DataTable parameter.")
		);
		FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
	}
	else if (RowData)
	{
		P_NATIVE_BEGIN;
		DataTable->AddRow(RowName, *RowData);
		P_NATIVE_END;
	}
}

void UCustomCsvToDTLib::RunCsvToUTF8Bat()
{
	const UCustomCsvToDTSetting* Setting = GetDefault<UCustomCsvToDTSetting>();

	if (Setting->BatToolPath.FilePath.IsEmpty())
	{
		UE_LOG(LogExportCsv, Error, TEXT("BatToolPath is Empty"));
		return;
	}

	// Bat 文件的全路径
	FString FullPath = FPaths::ConvertRelativePathToFull(Setting->BatToolPath.FilePath);

	// 找到 Bat 工作路径
	FString DirectoryPath = FPaths::GetPath(FullPath);
	FString CommandLine = FString::Printf(TEXT("cd \"\"%s\"\" && cmd /C \"\"%s\"\""), *DirectoryPath,*FullPath);
	system(TCHAR_TO_ANSI(*CommandLine));
}

// 修改表格后需要通知 UE Asset 发生更改提醒保存
void UCustomCsvToDTLib::NotifyDataTableChanged(UDataTable* DataTable)
{
	FDataTableEditorUtils::BroadcastPostChange( DataTable, FDataTableEditorUtils::EDataTableChangeInfo::RowList );
}

void UCustomCsvToDTLib::ClearDataTable(UDataTable* DataTable)
{
	if(DataTable == nullptr)
	{
		return;
	}
	DataTable->Modify();
	DataTable->EmptyTable();
	FDataTableEditorUtils::BroadcastPostChange( DataTable, FDataTableEditorUtils::EDataTableChangeInfo::RowList );
}

// 使用蓝图的 String 转 FText ，FText 的本地化相关配置会提示补全
FText UCustomCsvToDTLib::FStringToFText(const FString& InStr)
{
	return FText::FromString( InStr );
}

TMap<FString, FString> UCustomCsvToDTLib::GetSettingCsvPaths()
{
	TMap<FString, FString>Out;

	const UCustomCsvToDTSetting* Setting = GetDefault<UCustomCsvToDTSetting>();
	TArray<FFilePath> CsvPaths = Setting->CsvPaths;
	if ( CsvPaths.IsEmpty() )
	{
		return Out;
	}

	for (const FFilePath& CsvPath : CsvPaths)
	{
		FString CsvFullPath = FPaths::ConvertRelativePathToFull(CsvPath.FilePath);
		FString CsvFileName = FPaths::GetCleanFilename(CsvFullPath);
		Out.Add(CsvFileName, CsvFullPath);
	}

	return Out;
}

#endif
