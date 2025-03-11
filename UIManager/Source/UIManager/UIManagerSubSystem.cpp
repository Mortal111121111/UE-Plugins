// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManagerSubSystem.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Lib/UIManagerLib.h"
#include "Misc/UIManagerInterface.h"

UUserWidget* UUIManagerSubSystem::FindUI( FName UIName ) const
{
    if(!UIInfoList.Contains( UIName ))
    {
        return nullptr;
    }
    const FUIInfo Info = *UIInfoList.Find( UIName );
    return Info.Widget.Get();
}

UUserWidget* UUIManagerSubSystem::CreateUI( FName UIName, const FInstancedStruct& Params )
{
    // Remove 上一个
    if(UIInfoList.Contains( UIName ))
    {
        DestroyUI( UIName );
        UE_LOG( LogUIManagerPlugin, Warning, TEXT("%s , Created"), *UIName.ToString() );
    }

    FUICreateInfo CreateInfo;
    const TObjectPtr<UWorld> World = GetWorld();
    TObjectPtr<APlayerController> Controller = UGameplayStatics::GetPlayerController( World, 0 );

    if(World == nullptr || Controller == nullptr || !UUIManagerLib::GetUICreateInfo( UIName, CreateInfo ))
    {
        return nullptr;
    }

    FUIInfo UIInfo;
    UIInfo.bIsResetPauseGame = UGameplayStatics::IsGamePaused( World );
    if(CreateInfo.bIsOpenPauseGame && !UIInfo.bIsResetPauseGame)
    {
        UGameplayStatics::SetGamePaused( GetWorld(), true );
    }

    // TODO 后续增加相关监听事件，支持特判手柄是否显示鼠标功能
    UIInfo.bIsResetShowMouse = Controller == nullptr ? false : Controller->ShouldShowMouseCursor();
    Controller->SetShowMouseCursor( CreateInfo.bIsShowMouse );

    TObjectPtr<UUserWidget> NewWidget = UWidgetBlueprintLibrary::Create( World, CreateInfo.Cls, nullptr );
    if(NewWidget == nullptr)
    {
        UE_LOG( LogUIManagerPlugin, Log, TEXT("%s , Create Failed"), *UIName.ToString() );
        return nullptr;
    }

    UIInfo.Widget = NewWidget;
    UIInfoList.Add( UIName, UIInfo );
    NewWidget->AddToViewport( CreateInfo.ZOrder );
    InitUI( NewWidget, Params );

    UE_LOG( LogUIManagerPlugin, Log, TEXT("%s , Create Succeed"), *UIName.ToString() );
    return NewWidget;
}

void UUIManagerSubSystem::DestroyUI( FName UIName )
{
    const TObjectPtr<UWorld> World = GetWorld();
    if(World == nullptr || !UIInfoList.Contains( UIName ))
    {
        return;
    }

    const FUIInfo Info = UIInfoList.FindChecked( UIName );
    const TObjectPtr<UUserWidget> Widget = Info.Widget.Get();

    if(!IsValid( Widget ))
    {
        UIInfoList.Remove( UIName );
        return;
    }

    UGameplayStatics::SetGamePaused( World, Info.bIsResetPauseGame );

    TObjectPtr<APlayerController> Controller = UGameplayStatics::GetPlayerController( World, 0 );
    Controller->SetShowMouseCursor( Info.bIsResetShowMouse );

    UnInitUI( Widget );
    UIInfoList.Remove( UIName );

    Widget->RemoveFromParent();
}

void UUIManagerSubSystem::DestroyAllUI()
{
    for(const TTuple<FName, FUIInfo> InfoList : UIInfoList)
    {
        DestroyUI( InfoList.Key );
    }
}

void UUIManagerSubSystem::InitUI( UUserWidget* UI, const FInstancedStruct& Params )
{
    if(UI != nullptr && UI->GetClass()->ImplementsInterface( UUIManagerInterface::StaticClass() ))
    {
        IUIManagerInterface* IM = Cast<IUIManagerInterface>( UI );
        IM->OnOpenUI( Params );
    }
}

void UUIManagerSubSystem::UnInitUI( UUserWidget* UI )
{
    if(UI != nullptr && UI->GetClass()->ImplementsInterface( UUIManagerInterface::StaticClass() ))
    {
        IUIManagerInterface* IM = Cast<IUIManagerInterface>( UI );
        IM->OnCloseUI();
    }
}