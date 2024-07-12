// Fill out your copyright notice in the Description page of Project Settings.


#include "ToastSystem.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

void UToastSystem::Deinitialize()
{
	Super::Deinitialize();
	DestroyAllToast();
}

void UToastSystem::CreateToast(UObject* WorldContextObject , TSubclassOf<UMainToast> Cls)
{
	const TObjectPtr<UWorld> World = WorldContextObject->GetWorld();
	if( World == nullptr )
	{
		return;
	}

	const TObjectPtr<UMainToast> MainToast = Cast<UMainToast>(UWidgetBlueprintLibrary::Create(World,Cls,nullptr));
	MainToast->AddToViewport(0);
	AllMainToast.Add(Cls,MainToast);
}

void UToastSystem::DestroyAllToast()
{
	for (const TTuple<TSubclassOf<UMainToast>, TObjectPtr<UMainToast>> MainToast : AllMainToast)
	{
		MainToast.Value->RemoveFromParent();
	}
	AllMainToast.Empty();
}

void UToastSystem::ShowToast(TSubclassOf<UMainToast> Cls, UObject* ToastData)
{
	if( !AllMainToast.Contains(Cls) )
	{
		return;
	}

	const TObjectPtr<UMainToast> MainToast = AllMainToast.FindChecked(Cls);
	MainToast->AddItem(ToastData);
}

void UToastSystem::StopToast(TSubclassOf<UMainToast> Cls)
{
	if( !AllMainToast.Contains(Cls) )
	{
		return;
	}

	const TObjectPtr<UMainToast> MainToast = AllMainToast.FindChecked(Cls);
	MainToast->StopShowItem();
}
