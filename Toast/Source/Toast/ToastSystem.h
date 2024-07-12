// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainToast.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ToastSystem.generated.h"

/**
 * 
 */
UCLASS()
class TOAST_API UToastSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Deinitialize() override;
	
	UFUNCTION(BlueprintCallable,meta=(WorldContext="WorldContextObject"))
	void CreateToast(UObject* WorldContextObject , TSubclassOf<UMainToast> Cls);

	void DestroyAllToast();

	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "Cls"))
	void ShowToast(TSubclassOf<UMainToast> Cls, UObject* ToastData);

	UFUNCTION(BlueprintCallable,meta = (DeterminesOutputType = "Cls"))
	void StopToast(TSubclassOf<UMainToast> Cls);
	
private:
	UPROPERTY()
	TMap<TSubclassOf<UMainToast>,TObjectPtr<UMainToast>> AllMainToast;
};


