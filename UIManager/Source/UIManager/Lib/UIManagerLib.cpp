// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManagerLib.h"

#include "Blueprint/UserWidget.h"
#include "UIManager/UIManagerDefine.h"
#include "UIManager/UIManagerSubSystem.h"
#include "UIManager/DataTable/UIDataTable.h"
#include "UIManager/Misc/UIManagerInterface.h"
#include "UIManager/Misc/UIManagerSetting.h"

class UUIDataTable;
class UUIManagerSetting;
class FAssetToolsModule;

UUserWidget* UUIManagerLib::OpenUI( UObject* WorldContextObject, FName UIName, FInstancedStruct Params, bool& bIsOpenSucceed )
{
    const TObjectPtr<UWorld> World = WorldContextObject->GetWorld();
    if(World == nullptr)
    {
        bIsOpenSucceed = false;
        return nullptr;
    }

    const TObjectPtr<UUIManagerSubSystem> SubSystem = World->GetGameInstance()->GetSubsystem<UUIManagerSubSystem>();
    if(SubSystem == nullptr)
    {
        bIsOpenSucceed = false;
        return nullptr;
    }

    const TObjectPtr<UUserWidget> NewWidget = SubSystem->CreateUI( UIName, Params );
    bIsOpenSucceed = NewWidget != nullptr;

    return NewWidget;
}

void UUIManagerLib::CloseUI( UObject* WorldContextObject, FName UIName )
{
    const TObjectPtr<UWorld> World = WorldContextObject->GetWorld();
    if(World == nullptr)
    {
        return;
    }

    const TObjectPtr<UUIManagerSubSystem> SubSystem = World->GetGameInstance()->GetSubsystem<UUIManagerSubSystem>();
    if(SubSystem == nullptr)
    {
        return;
    }

    SubSystem->DestroyUI( UIName );
}

bool UUIManagerLib::GetUICreateInfo( FName UIName, FUICreateInfo& OutInfo )
{
    const UUIManagerSetting* Settings = GetDefault<UUIManagerSetting>();
    const TObjectPtr<UUIDataTable> UIDataTable = Cast<UUIDataTable>( Settings->UIDataTablePath.TryLoad() );
    if(UIDataTable == nullptr)
    {
        return false;
    }

    const FUICreateInfo* Info = UIDataTable->FindRow<FUICreateInfo>( UIName, FString() );
    if(Info == nullptr)
    {
        return false;
    }

    OutInfo = *Info;
    return true;
}

UClass* UUIManagerLib::GetUICreateInfoCls( FName UIName )
{
    FUICreateInfo Info;
    if(!GetUICreateInfo( UIName, Info ))
    {
        return nullptr;
    }

    return Info.Cls == nullptr ? nullptr : Info.Cls->GetAuthoritativeClass();
}

TArray<TSharedPtr<FName>> UUIManagerLib::GetAllUINames()
{
    TArray<TSharedPtr<FName>> ResNames;

    const UUIManagerSetting* Settings = GetDefault<UUIManagerSetting>();
    const TObjectPtr<UUIDataTable> UIDataTable = Cast<UUIDataTable>( Settings->UIDataTablePath.TryLoad() );
    if(UIDataTable == nullptr)
    {
        UE_LOG( LogUIManagerPlugin, Error, TEXT("UIDataTable Not Found , Please check UIManagerSetting UIDataTablePath!!!") )
        return ResNames;
    }

    TArray<FName> AllRowNames = UIDataTable->GetRowNames();
    for(const FName ResName : AllRowNames)
    {
        const FUICreateInfo Info = *UIDataTable->FindRow<FUICreateInfo>( ResName, FString() );
        if(!Info.bIsUP)
        {
            ResNames.Add( MakeShareable( new FName( ResName ) ) );
        }
    }

    return ResNames;
}