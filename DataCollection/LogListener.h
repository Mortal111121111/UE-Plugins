// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

USTRUCT()
struct FLogCollectionConfig
{
	GENERATED_BODY()

	/* 筛选出的 log 存放的文件名字 */
	UPROPERTY(EditAnywhere)
	FString LogsFile;

	/* 只筛选以下 LogCategory */
	UPROPERTY(EditAnywhere)
	TArray<FName> LogCategory;
};


class DATACOLLECTION_API FLogListener : public FOutputDevice
{
public:
	virtual void Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const FName& Category) override;

public:
	void InitLogListener(const FString& InDirName,TArray<FLogCollectionConfig> ConfigList);
	void UnInitLogListener();
	
	void CreateFile(const FString& FileName);
	void WriteFile(const FString& FileName, const FString& WriteValue) const;
	
private:
	TMap<FString,IFileHandle*> FileHandleList;
	TMap<FName,TArray<FString>> FileCategory;
	FString DirName;
};
