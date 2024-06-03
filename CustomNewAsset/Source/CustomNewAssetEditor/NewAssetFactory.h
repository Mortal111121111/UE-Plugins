// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "NewAssetFactory.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMNEWASSETEDITOR_API UNewAssetFactory : public UFactory
{
	GENERATED_BODY()

	UNewAssetFactory();
	
public:
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
