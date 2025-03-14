// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InstancedStruct.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UIManager/UIManagerDefine.h"
#include "UIManagerLib.generated.h"

/**
 * 
 */
UCLASS()
class UIMANAGER_API UUIManagerLib : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    //BlueprintInternalUseOnly=true,ExpandBoolAsExecs = "bIsOpenSucceed"
    UFUNCTION( BlueprintCallable, meta=(BlueprintInternalUseOnly=true, WorldContext="WorldContextObject") )
    static UUserWidget* OpenUI( UObject* WorldContextObject, FName UIName, FInstancedStruct Params, bool& bIsOpenSucceed );

    UFUNCTION( BlueprintCallable, meta=(BlueprintInternalUseOnly=true, WorldContext="WorldContextObject") )
    static void CloseUI( UObject* WorldContextObject, FName UIName );

    static bool GetUICreateInfo( FName UIName, FUICreateInfo& OutInfo );
    static TArray<TSharedPtr<FName>> GetAllUINames();
    static UClass* GetUICreateInfoCls( FName UIName );
};