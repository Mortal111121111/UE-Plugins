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

	UFUNCTION(BlueprintCallable)
	static bool DestroySaveGame(FString Slot,int UserIndex);
	
	UFUNCTION(BlueprintCallable,meta = (WorldContext="WorldContextObject"))
	static bool DestroyCurrentUserSaveGame(UObject* WorldContextObject,FString Slot);
	
	UFUNCTION(BlueprintCallable,meta = (DeterminesOutputType = "Cls"))
	static USaveGame* GetSaveGameBySlot(TSubclassOf<USaveGame> Cls , FString Slot,int UserIndex);
	
	UFUNCTION(BlueprintCallable,meta = (DeterminesOutputType = "Cls",WorldContext="WorldContextObject"))
	static USaveGame* GetCurrentUserSaveGameBySlot(UObject* WorldContextObject,TSubclassOf<USaveGame> Cls , FString Slot);

	UFUNCTION(BlueprintCallable)
	static bool CheckSaveGameExist(FString Slot,int UserIndex);
	
	UFUNCTION(BlueprintCallable,meta = (WorldContext="WorldContextObject"))
	static bool CheckCurrentUserSaveGameExist(UObject* WorldContextObject,FString Slot);

	UFUNCTION(BlueprintCallable)
	static bool SaveGameBySlot(USaveGame* InSaveGame , FString Slot , int UserIndex);
	
	UFUNCTION(BlueprintCallable,meta = (WorldContext="WorldContextObject"))
	static bool SaveCurrentUserGameBySlot(UObject* WorldContextObject,USaveGame* InSaveGame , FString Slot);

	// UserIndex

	/* 新增一个用户 id */
	UFUNCTION(BlueprintCallable)
	static bool AddUserIndex();

	/* 删除一个用户 id */
	UFUNCTION(BlueprintCallable,meta = (WorldContext="WorldContextObject"))
	static bool DeleteUserIndex(UObject* WorldContextObject,int UserIndex);

	/* 获取所有用户 id */
	UFUNCTION(BlueprintCallable)
	static bool CheckUserIndex(int UserIndex);
	
	/* 获取所有用户 id */
	UFUNCTION(BlueprintCallable)
	static TArray<int> GetAllUserIndex();

	UFUNCTION(BlueprintCallable,meta = (WorldContext="WorldContextObject"))
	static void SetCurrentUserIndex(UObject* WorldContextObject,int UserIndex);
	
	UFUNCTION(BlueprintCallable,BlueprintPure,meta = (WorldContext="WorldContextObject"))
	static int GetCurrentUserIndex(const UObject* WorldContextObject);
	
	static UUserIndexSaveGame* GetUserIndexSaveGame();

private:
	static FString GetSaveGameSlot(const FString& Slot , const int UserIndex);
};
