// Fill out your copyright notice in the Description page of Project Settings.


#include "NewAssetFactory.h"

#include "CustomNewAsset/MyNewAsset.h"

UNewAssetFactory::UNewAssetFactory()
{
	SupportedClass = UMyNewAsset::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UNewAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UMyNewAsset>(InParent,InClass, InName, Flags, Context);
}

