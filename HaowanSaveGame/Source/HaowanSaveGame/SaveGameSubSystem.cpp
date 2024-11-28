// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGameSubSystem.h"

#include "HaowanSaveGame.h"
#include "SaveGameFuncLib.h"
#include "UserIndexSaveGame.h"

void USaveGameSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const TObjectPtr<UUserIndexSaveGame> SaveGame = USaveGameFuncLib::GetUserIndexSaveGame();
	if( SaveGame != nullptr )
	{
		CurrentUserIndex = SaveGame->CurrentUserIndex;
		UE_LOG(LogHaowanSaveGame,Log,TEXT("CurrentUserIndex Init Succeed! , UserIndex = %d"),CurrentUserIndex);
	}
}

void USaveGameSubSystem::Deinitialize()
{
	Super::Deinitialize();
}

int USaveGameSubSystem::GetCurrentUserIndex() const
{
	return CurrentUserIndex;
}

void USaveGameSubSystem::SetCurrentUserIndex(int InUserIndex)
{
	const int OldUserIndex = CurrentUserIndex;
	CurrentUserIndex = InUserIndex;
	UserIndexChangedEvent.Broadcast(InUserIndex,OldUserIndex);
}
