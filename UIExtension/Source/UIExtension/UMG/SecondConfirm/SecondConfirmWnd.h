// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SecondConfirmWnd.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSecondConfirmEvent,bool,bIsConfirm);

/**
 * 
 */
UCLASS()
class UIEXTENSION_API USecondConfirmWnd : public UUserWidget
{
	GENERATED_BODY()

private:
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true))
	void Confirm();
	
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true))
	void Cancel();
	
public:
	UPROPERTY(BlueprintAssignable)
	FSecondConfirmEvent SecondConfirmEvent;
	
};
