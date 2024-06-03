// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CustomThunkLib.generated.h"

USTRUCT(BlueprintType,Blueprintable)
struct  FSaveGamePropertyInfo 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString PropertyName;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int IntValue;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool BoolValue;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float FloatValue;
};

/**
 * 
 */
UCLASS()
class MYCUSTOMTHUNK_API UCustomThunkLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable,CustomThunk,meta=(CustomStructureParam="OutValue"))
	static void GetObjectPropertyByName(UObject* Target , FString PropertyName , const int& OutValue);
	static void Generic_GetObjectPropertyByName(UObject* Target , FString PropertyName , void* OutValue);
	DECLARE_FUNCTION(execGetObjectPropertyByName)
	{
		P_GET_OBJECT(UObject,Target);
		P_GET_PROPERTY(FStrProperty,PropertyName);
		// 关键
		Stack.StepCompiledIn<FProperty>(NULL);
		void* SrcPropertyAddr = Stack.MostRecentPropertyAddress;
		
		P_FINISH;
	
		P_NATIVE_BEGIN;
		Generic_GetObjectPropertyByName(Target, PropertyName, SrcPropertyAddr);
		P_NATIVE_END;
	}

	UFUNCTION(BlueprintCallable)
	static TArray<FSaveGamePropertyInfo> GetObjectSaveGamePropertyInfo(UObject* Object);

	UFUNCTION(BlueprintCallable)
	static bool SetObjectPropertyByName(UObject* Object,const FSaveGamePropertyInfo& Info);
};
