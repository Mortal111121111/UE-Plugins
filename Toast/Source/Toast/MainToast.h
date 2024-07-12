// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemToast.h"
#include "ToastDefine.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"
#include "MainToast.generated.h"

UCLASS()
class TOAST_API UMainToast : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void Construct_Implementation() ;
	
	void AddItem(UObject* ItemData); 
	void StopShowItem();

private:
	UItemToast* CreateItem(UObject* ItemData);
	
	void StartShowItem(UItemToast* ItemToast);
	void OnToastShowFinish(UItemToast* ItemToast);
	
protected:
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UCanvasPanel* CvsRoot;

	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UVerticalBox* VboxShowToast;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	TSubclassOf<UItemToast> ItemToastCls;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	int ToastShowList = 3;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	EToastShowType ShowType = EToastShowType::Single;
	
private:
	UPROPERTY()
	TSet<TObjectPtr<UItemToast>> ToastList;
	
	bool IsShowing = false;

};
