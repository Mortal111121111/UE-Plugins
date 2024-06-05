// Fill out your copyright notice in the Description page of Project Settings.


#include "DataCollectionObject.h"


UWorld* UDataCollectionObject::GetWorld() const
{
	if (!HasAnyFlags(RF_ClassDefaultObject) && GetOuter() != nullptr && !GetOuter()->HasAnyFlags(RF_BeginDestroyed) && !GetOuter()->IsUnreachable())
	{
		if (const TObjectPtr<UDataCollectionSubSystem> Outer = GetTypedOuter<UDataCollectionSubSystem>())
		{
			return Outer->GetWorld();
		}
	}
	return nullptr;
}

void UDataCollectionObject::StartDataCollection(FString DefaultName)
{
	// if(Name.IsEmpty())
	// {
	// 	Name = DefaultName;
	// }
	
	FileHandle = CreateFile();
	if(FileHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("DataCollection Start Error: %s"), *Name);
		return;
	}
	
	BP_StartDataCollection();
}

void UDataCollectionObject::StopDataCollection()
{
	BP_StopDataCollection();
	
	if(FileHandle != nullptr)
	{
		delete FileHandle;
	}
}

IFileHandle* UDataCollectionObject::CreateFile() const
{
	const FString DirectoryPath = FPaths::ProjectSavedDir() / TEXT("DataCollection");
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (!PlatformFile.DirectoryExists(*DirectoryPath))
	{
		PlatformFile.CreateDirectory(*DirectoryPath);
		if (!PlatformFile.DirectoryExists(*DirectoryPath))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create directory: %s"), *DirectoryPath);
			return nullptr;
		}
	}

	const FString NowTimeStr = FDateTime::Now().ToString();
	const FString FilePath = DirectoryPath / Name + "-" + NowTimeStr + ".txt";
	FFileHelper::SaveStringToFile(FString::Printf(TEXT("%s \r\n"),*NowTimeStr), *FilePath);
	if(!PlatformFile.FileExists(*FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save text to file: %s"), *FilePath);
		return nullptr;
	}
	
	UE_LOG(LogTemp, Log, TEXT("Successfully saved text to file: %s"), *FilePath);
	return PlatformFile.OpenWrite(*FilePath,true,true);
}

void UDataCollectionObject::WriteFile(const FString& WriteValue) const
{
	const auto Data = StringCast<ANSICHAR>(*(WriteValue + "\r\n"));
	const uint8* ByteData = (const uint8*)(Data.Get());
	const int32 ByteCount = Data.Length();
	// 写入字节数组到文件
	FileHandle->Write(ByteData, ByteCount);
	
}

FString UDataCollectionObject::GetNowToString() const
{
	return FDateTime::Now().ToString();
}
