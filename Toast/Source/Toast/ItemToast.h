#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemToast.generated.h"

DECLARE_DELEGATE(FToastShowFinishEvent)

UCLASS()
class TOAST_API UItemToast : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void InitItemToast(UObject* InitData);
	
	UFUNCTION(BlueprintNativeEvent)
	void ShowItemToast();

private:
	UFUNCTION(BlueprintCallable,meta=(AllowPrivateAccess=true))
	void ShowFinish();

public:
	FToastShowFinishEvent ToastShowFinishEvent;
};
