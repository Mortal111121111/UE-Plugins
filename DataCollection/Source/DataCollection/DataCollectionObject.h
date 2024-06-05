// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DataCollectionObject.generated.h"

/**
 * 
 */
UCLASS()
class DATACOLLECTION_API UDataCollectionObject : public UObject
{
	GENERATED_BODY()
	
	virtual UWorld* GetWorld() const override;
	
public:
	void StartDataCollection(FString DefaultName);
	void StopDataCollection();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BP_StartDataCollection();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_StopDataCollection();
	
	UFUNCTION(BlueprintCallable)
	void WriteFile(const FString& WriteValue) const;

	UFUNCTION(BlueprintCallable)
	FString GetNowToString() const;
	
private:
	IFileHandle* CreateFile() const;
	
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FString Name;

private:
	IFileHandle* FileHandle = nullptr;
};
