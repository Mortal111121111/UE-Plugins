// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "SkipNumberText.generated.h"

/**
 * 数值变化文本框
 */
UCLASS( Abstract )
class UIEXTENSION_API USkipNumberText : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

public:
    UFUNCTION( BlueprintCallable )
    void InitSkipNumber( int32 InNumberValue );

    UFUNCTION( BlueprintCallable )
    void StartUpdateSkipNumber( int32 InChangedValue );

    UFUNCTION( BlueprintCallable )
    void AddSkipNumber( int32 InNumberValue );

private:
    UFUNCTION()
    void UpdateSkipNumberText();
    void EndUpdateSkipNumberText();
    void BuildAnim( UWidget* BindingObject );

public:
    UPROPERTY( BlueprintReadWrite, meta=(BindWidget) )
    TObjectPtr<UTextBlock> TxtNumberText;

    UPROPERTY( BlueprintReadWrite )
    int32 NumberValue;

    UPROPERTY( BlueprintReadWrite )
    TObjectPtr<UWidgetAnimation> SimpleAnim;

    /* 跳动次数 */
    UPROPERTY( BlueprintReadWrite, EditAnywhere, Category="Config" )
    int32 SkipCounts;

    /* 跳动总时间 */
    UPROPERTY( BlueprintReadWrite, EditAnywhere, Category="Config" )
    float UpdateTotalTime = 1;

    /* 浮动最大值 */
    UPROPERTY( BlueprintReadWrite, EditAnywhere, Category="Config" )
    int32 FloatMaxValue = 3;

    UPROPERTY( BlueprintReadWrite, EditAnywhere, Category="AnimConfig" )
    TArray<FVector2D> AnimKeys;

    UPROPERTY( BlueprintReadWrite, EditAnywhere, Category="Config" )
    float AnimTime = 1;

private:
    int32 UpdateValue;
    int32 UpdateCurrentValue;
    FTimerHandle UpdateTimerHandle;
};