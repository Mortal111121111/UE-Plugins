// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserIndexSaveGame.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SaveGameFuncLib.generated.h"

/**
 * 
 */
UCLASS()
class HAOWANSAVEGAME_API USaveGameFuncLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// SaveGame
	UFUNCTION(BlueprintCallable,meta = (DeterminesOutputType = "Cls"))
	static USaveGame* CreateSaveGameByCls(TSubclassOf<USaveGame> Cls);
	
	UFUNCTION(BlueprintCallable,meta = (WorldContext="WorldContextObject"))
	static bool DestroySaveGame(UObject* WorldContextObject,FString Slot);
	
	UFUNCTION(BlueprintCallable,meta = (DeterminesOutputType = "Cls",WorldContext="WorldContextObject"))
	static USaveGame* GetSaveGameBySlot(UObject* WorldContextObject,TSubclassOf<USaveGame> Cls , FString Slot);

	UFUNCTION(BlueprintCallable,meta = (WorldContext="WorldContextObject"))
	static bool CheckSaveGameExist(UObject* WorldContextObject,FString Slot);

	UFUNCTION(BlueprintCallable,meta = (WorldContext="WorldContextObject"))
	static bool SaveGameBySlot(UObject* WorldContextObject,USaveGame* InSaveGame , FString Slot);

	// UserIndex

	/* 新增一个用户 id */
	UFUNCTION(BlueprintCallable)
	static bool AddUserIndex();

	/* 删除一个用户 id */
	UFUNCTION(BlueprintCallable,meta = (WorldContext="WorldContextObject"))
	static bool DeleteUserIndex(UObject* WorldContextObject,int UserIndex);

	/* 获取所有用户 id */
	UFUNCTION(BlueprintCallable)
	static TArray<int> GetAllUserIndex();

	UFUNCTION(BlueprintCallable,meta = (WorldContext="WorldContextObject"))
	static void SetCurrentUserIndex(UObject* WorldContextObject,int UserIndex);
	
	UFUNCTION(BlueprintCallable,BlueprintPure,meta = (WorldContext="WorldContextObject"))
	static int GetCurrentUserIndex(const UObject* WorldContextObject);
	
	static UUserIndexSaveGame* GetUserIndexSaveGame();

private:
	static FString GetSaveGameSlotNameByIndex(const UObject* WorldContextObject, const FString& Slot);
};
