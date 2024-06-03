// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManagerSubSystem.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Lib/UIManagerLib.h"

UUserWidget* UUIManagerSubSystem::FindUI(FName UIName) const
{
	if(!UIInfoList.Contains(UIName)) 
	{
		return nullptr;
	}
	const FUIInfo Info = *UIInfoList.Find(UIName);
	return Info.Widget;
}

UUserWidget* UUIManagerSubSystem::CreateUI(FName UIName)
{
	FUICreateInfo Info;
	const TObjectPtr<UWorld> World = GetWorld();
	if(World == nullptr || !UUIManagerLib::GetUICreateInfo(UIName,Info))
	{
		return nullptr;
	}
	
	UUserWidget* NewWidget = UWidgetBlueprintLibrary::Create(World,Info.Cls,nullptr);
	NewWidget->AddToViewport(Info.ZOrder);
	if(Info.bIsOpenPauseGame)
	{
		UGameplayStatics::SetGamePaused(World,true);
	}
	UE_LOG(LogUIManagerPlugin,Log,TEXT("%s , Create Succeed"),*UIName.ToString());
	
	UIInfoList.Add(UIName,FUIInfo(NewWidget,Info));
	return NewWidget;
}

void UUIManagerSubSystem::DestroyUI(FName UIName)
{
	const TObjectPtr<UWorld> World = GetWorld();
	if(World == nullptr || !UIInfoList.Contains(UIName))
	{
		return;
	}
	
	const FUIInfo Info = *UIInfoList.Find(UIName);
	const TObjectPtr<UUserWidget> Widget = Info.Widget;
	Widget->RemoveFromParent();
	if(Info.CreateInfo.bIsOpenPauseGame)
	{
		UGameplayStatics::SetGamePaused(World,false);
	}
	UIInfoList.Remove(UIName);
}

void UUIManagerSubSystem::DestroyAllUI()
{
	for (const TTuple<FName, FUIInfo> InfoList : UIInfoList)
	{
		DestroyUI(InfoList.Key);
	}
}
