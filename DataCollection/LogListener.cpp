// Fill out your copyright notice in the Description page of Project Settings.


#include "LogListener.h"

void FLogListener::Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const FName& Category)
{
	if(FileCategory.Contains(Category))
	{
		TArray<FString> FileNameList = FileCategory.FindChecked(Category);
		for (const FString& NameList : FileNameList)
		{
			WriteFile(NameList,V);
		}
	}
}

void FLogListener::InitLogListener(const FString& InDirName,TArray<FLogCollectionConfig> ConfigList)
{
	DirName = InDirName;
	for (const FLogCollectionConfig& Config : ConfigList)
	{
		for (const FName& Category : Config.LogCategory)
		{
			TArray<FString> FileNameList;
			if(FileCategory.Contains(Category))
			{
				FileNameList = FileCategory.FindChecked(Category);
			}
			FileNameList.Add(Config.LogsFile);
			FileCategory.FindOrAdd(Category,FileNameList);
		}
		
		CreateFile(Config.LogsFile);
	}
}

void FLogListener::UnInitLogListener()
{
	for (const TTuple<FString, IFileHandle*> HandleList : FileHandleList)
	{
		if(HandleList.Value != nullptr)
		{
			delete HandleList.Value;
		}
	}

	FileHandleList.Empty();
}

void FLogListener::CreateFile(const FString& FileName)
{
	const FString DirectoryPath = FPaths::ProjectSavedDir() / DirName;
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (!PlatformFile.DirectoryExists(*DirectoryPath))
	{
		PlatformFile.CreateDirectory(*DirectoryPath);
		if (!PlatformFile.DirectoryExists(*DirectoryPath))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create directory: %s"), *DirectoryPath);
			return;
		}
	}

	const FString NowTimeStr = FDateTime::Now().ToString();
	const FString FilePath = DirectoryPath / FileName + "-" + NowTimeStr + ".txt";
	FFileHelper::SaveStringToFile(FString::Printf(TEXT("%s \r\n"),*NowTimeStr), *FilePath);
	if(!PlatformFile.FileExists(*FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save text to file: %s"), *FilePath);
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("Successfully saved text to file: %s"), *FilePath);

	IFileHandle* FileHandle = PlatformFile.OpenWrite(*FilePath,true,true);
	if(FileHandle == nullptr)
	{
		return;
	}
	FileHandleList.FindOrAdd(FileName,FileHandle);
}

void FLogListener::WriteFile(const FString& FileName, const FString& WriteValue) const
{
	const auto Data = StringCast<ANSICHAR>(*(WriteValue + "\r\n"));
	const uint8* ByteData = (const uint8*)(Data.Get());
	const int32 ByteCount = Data.Length();

	if( FileHandleList.Contains(FileName) )
	{
		IFileHandle* FileHandle = FileHandleList.FindChecked(FileName);
		FileHandle->Write(ByteData, ByteCount);
	}
	
}




