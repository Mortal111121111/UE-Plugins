// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGameFuncLib.h"

#include "HaowanSaveGame.h"
#include "HaowanSaveGameSetting.h"
#include "SaveGameSubSystem.h"
#include "UserIndexSaveGame.h"
#include "Kismet/GameplayStatics.h"

USaveGame* USaveGameFuncLib::GetCurrentUserSaveGameBySlot(UObject* WorldContextObject, TSubclassOf<USaveGame> Cls, FString Slot)
{
	const int UserIndex = GetCurrentUserIndex(WorldContextObject);
	if( UserIndex <= 0 )
	{
		UE_LOG(LogHaowanSaveGame,Log,TEXT("UserIndex = %d , Error UserIndex! "),UserIndex);
		return nullptr;
	}
	
	return GetSaveGameBySlot(Cls,Slot,UserIndex);
}

USaveGame* USaveGameFuncLib::GetSaveGameBySlot(TSubclassOf<USaveGame> Cls, FString Slot, int UserIndex)
{
	const FString NewSlot = GetSaveGameSlot(Slot,UserIndex);
	const TObjectPtr<USaveGame> OutSaveGame = UGameplayStatics::LoadGameFromSlot(NewSlot,0);
	if(OutSaveGame == nullptr)
	{
		UE_LOG(LogHaowanSaveGame,Log,TEXT("UserIndex = %d , Error UserIndex! "),UserIndex);
		return nullptr;
	}

	return OutSaveGame;
}

bool USaveGameFuncLib::CheckCurrentUserSaveGameExist(UObject* WorldContextObject, FString Slot)
{
	const int UserIndex = GetCurrentUserIndex(WorldContextObject);
	if( UserIndex <= 0 )
	{
		UE_LOG(LogHaowanSaveGame,Log,TEXT("UserIndex = %d , Error UserIndex! "),UserIndex);
		return false;
	}

	return CheckSaveGameExist(Slot,UserIndex);
}

bool USaveGameFuncLib::CheckSaveGameExist(FString Slot, int UserIndex)
{
	const FString NewSlot = GetSaveGameSlot(Slot,UserIndex);
	return UGameplayStatics::DoesSaveGameExist(NewSlot,0);
}

bool USaveGameFuncLib::SaveGameBySlot(USaveGame* InSaveGame, FString Slot, int UserIndex)
{
	if( InSaveGame == nullptr )
	{
		UE_LOG(LogHaowanSaveGame,Log,TEXT("InSaveGame is Invalid! "));
		return false;
	}
	
	const FString NewSlot = GetSaveGameSlot(Slot,UserIndex);
	UGameplayStatics::SaveGameToSlot(InSaveGame,NewSlot,0);
	
	return true;
}

bool USaveGameFuncLib::SaveCurrentUserGameBySlot(UObject* WorldContextObject, USaveGame* InSaveGame, FString Slot)
{
	const int UserIndex = GetCurrentUserIndex(WorldContextObject);
	if( UserIndex <= 0 )
	{
		return false;
	}
	
	return SaveGameBySlot(InSaveGame,Slot,UserIndex);
}


bool USaveGameFuncLib::DestroyCurrentUserSaveGame(UObject* WorldContextObject, FString Slot)
{
	const int UserIndex = GetCurrentUserIndex(WorldContextObject);
	if( UserIndex <= 0 )
	{
		return false;
	}

	const FString NewSlot = GetSaveGameSlot(Slot,UserIndex);
	UGameplayStatics::DeleteGameInSlot(NewSlot,0);
	return true;
}

bool USaveGameFuncLib::DestroySaveGame(const FString Slot, const int UserIndex)
{
	const FString NewSlot = GetSaveGameSlot(Slot,UserIndex);
	UGameplayStatics::DeleteGameInSlot(NewSlot,0);
	return true;
}

USaveGame* USaveGameFuncLib::CreateSaveGameByCls(TSubclassOf<USaveGame> Cls)
{
	return UGameplayStatics::CreateSaveGameObject(Cls);
}

// UserIndex
bool USaveGameFuncLib::AddUserIndex()
{
	const TObjectPtr<UUserIndexSaveGame> UserIndexSaveGame = GetUserIndexSaveGame();
	if( UserIndexSaveGame == nullptr )
	{
		return false;
	}
	
	const UHaowanSaveGameSetting* Setting = GetDefault<UHaowanSaveGameSetting>();
	const FString SlotName = Setting->UserIndexSaveGameSlotName;
	
	const int CurrentIndex = ++ UserIndexSaveGame->CurrentIndex;
	UserIndexSaveGame->UserIndexList.Add(CurrentIndex);
	UGameplayStatics::SaveGameToSlot(UserIndexSaveGame,SlotName,0);

	UE_LOG(LogHaowanSaveGame,Log,TEXT("AddUserIndex Succeed! , UserIndex = %d"),CurrentIndex);
	return true;
}

bool USaveGameFuncLib::DeleteUserIndex(UObject* WorldContextObject, int UserIndex)
{
	const TObjectPtr<UUserIndexSaveGame> UserIndexSaveGame = GetUserIndexSaveGame();
	if( UserIndexSaveGame == nullptr )
	{
		return false;
	}

	const UHaowanSaveGameSetting* Setting = GetDefault<UHaowanSaveGameSetting>();
	const FString SlotName = Setting->UserIndexSaveGameSlotName;

	// 删除 Index 记录
	UserIndexSaveGame->UserIndexList.Remove(UserIndex);
	UGameplayStatics::SaveGameToSlot(UserIndexSaveGame,SlotName,0);

	// 清理所有相关 sav 文件
	for (FString SaveNameSlot: Setting->SaveNameSlotList)
	{
		const FString NewSlot = GetSaveGameSlot(SlotName,UserIndex);
		UGameplayStatics::DeleteGameInSlot(NewSlot,0);
	}
	
	UE_LOG(LogHaowanSaveGame,Log,TEXT("RemoveUserIndex Succeed! , UserIndex = %d"),UserIndex);
	return true;
}

bool USaveGameFuncLib::CheckUserIndex(int UserIndex)
{
	const TObjectPtr<UUserIndexSaveGame> UserIndexSaveGame = GetUserIndexSaveGame();
	if( UserIndexSaveGame == nullptr )
	{
		return false;
	}
	
	return UserIndexSaveGame->UserIndexList.Find(UserIndex) != -11;
}

TArray<int> USaveGameFuncLib::GetAllUserIndex()
{
	TArray<int> OutUserIndex;
	const TObjectPtr<UUserIndexSaveGame> UserIndexSaveGame = GetUserIndexSaveGame();
	if( UserIndexSaveGame == nullptr )
	{
		return OutUserIndex;
	}

	return UserIndexSaveGame->UserIndexList;
}

void USaveGameFuncLib::SetCurrentUserIndex(UObject* WorldContextObject, int UserIndex)
{
	const TObjectPtr<UWorld> World = WorldContextObject->GetWorld();
	if( World == nullptr )
	{
		return ;
	}

	const TObjectPtr<USaveGameSubSystem> SubSystem = World->GetGameInstance()->GetSubsystem<USaveGameSubSystem>();
	const TObjectPtr<UUserIndexSaveGame> UserIndexSaveGame = GetUserIndexSaveGame();
	if( SubSystem == nullptr || UserIndexSaveGame == nullptr )
	{
		return ;
	}

	const UHaowanSaveGameSetting* Setting = GetDefault<UHaowanSaveGameSetting>();
	
	SubSystem->SetCurrentUserIndex(UserIndex);
	UserIndexSaveGame->CurrentUserIndex = UserIndex;
	UGameplayStatics::SaveGameToSlot(UserIndexSaveGame,Setting->UserIndexSaveGameSlotName,0);
	UE_LOG(LogHaowanSaveGame,Log,TEXT("Set CurrentUserIndex Succeed! , UserIndex = %d"),UserIndex);
}

int USaveGameFuncLib::GetCurrentUserIndex(const UObject* WorldContextObject)
{
	const TObjectPtr<UWorld> World = WorldContextObject->GetWorld();
	if( World == nullptr )
	{
		return -1;
	}

	const TObjectPtr<USaveGameSubSystem> SubSystem = World->GetGameInstance()->GetSubsystem<USaveGameSubSystem>();
	if( SubSystem == nullptr )
	{
		return -1;
	}

	return SubSystem->GetCurrentUserIndex();
}

// Util
UUserIndexSaveGame* USaveGameFuncLib::GetUserIndexSaveGame()
{
	const UHaowanSaveGameSetting* Setting = GetDefault<UHaowanSaveGameSetting>();
	const FString SlotName = Setting->UserIndexSaveGameSlotName;
	TObjectPtr<USaveGame> OutSaveGame = UGameplayStatics::LoadGameFromSlot(SlotName,0);
	if(OutSaveGame == nullptr)
	{
		OutSaveGame = CreateSaveGameByCls(UUserIndexSaveGame::StaticClass());
	}

	return Cast<UUserIndexSaveGame>(OutSaveGame);
}

FString USaveGameFuncLib::GetSaveGameSlot(const FString& Slot, const int UserIndex)
{
	return Slot + "_" + FString::FromInt(UserIndex);
}

