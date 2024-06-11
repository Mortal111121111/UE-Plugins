// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "UObject/Object.h"
#include "TestAsyncAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTestHandleDelegate);

/**
 * 
 */
UCLASS()
class MYASYCNBLUEPRINT_API UTestAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,meta = (BlueprintInternalUseOnly = "true",WorldContext = "WorldContextObject"))
	static UTestAsyncAction* TestAsync(const UObject* WorldContextObject,float Time);

	virtual void Activate() override;
	
public:
	UPROPERTY(BlueprintAssignable)
	FTestHandleDelegate BP_Activate;
	
	UPROPERTY(BlueprintAssignable)
	FTestHandleDelegate Completed;

	UPROPERTY(BlueprintAssignable)
	FTestHandleDelegate Cancel;
	
private:
	float TempTime = 0.f;
	FTimerHandle TimerHandle;
	const UObject* WorldContextObject = nullptr;
	
private:
	UFUNCTION()
	void TriggerTest();
};
