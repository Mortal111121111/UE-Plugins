// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SecondConfirmWnd.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "SecondConfirmAsyncAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSecondConfirmAsyncActionEvent);

/**
 * 
 */
UCLASS()
class UIEXTENSION_API USecondConfirmAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,meta = (BlueprintInternalUseOnly = "true",WorldContext = "WorldContextObject"))
	static USecondConfirmAsyncAction* SecondConfirm(const UObject* WorldContextObject,USecondConfirmWnd* Widget);

private:
	UFUNCTION()
	void OnSecondConfirmEvent(bool bIsConfirm);

	void BindSecondConfirmEvent();
public:
	UPROPERTY(BlueprintAssignable)
	FSecondConfirmAsyncActionEvent Confirm;
	
	UPROPERTY(BlueprintAssignable)
	FSecondConfirmAsyncActionEvent Cancel;
};
