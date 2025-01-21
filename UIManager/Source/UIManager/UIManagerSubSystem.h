// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIManagerDefine.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UIManagerSubSystem.generated.h"

struct FInstancedStruct;
/**
 * 
 */
UCLASS()
class UIMANAGER_API UUIManagerSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UUserWidget* FindUI(FName UIName) const;
	UUserWidget* CreateUI(FName UIName,const FInstancedStruct& Params);
	void DestroyUI(FName UIName);
	void DestroyAllUI();

private:
	void InitUI(UUserWidget* UI,const FInstancedStruct& Params);
	void UnInitUI(UUserWidget* UI);
	
private:
	TMap<FName,FUIInfo> UIInfoList;
};
