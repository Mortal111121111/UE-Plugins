// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlock.h"

#include "KAutoWriterRichText.generated.h"

UENUM(BlueprintType)
enum class EAutoWriteState : uint8
{
	Closed,
	Stopped,
	Running
};

struct FTagWithContent
{
	FTagWithContent(const FString& NodeTag, const FString& NodeContent);

	FString Tag;

	FString Content;

	bool bIsLineFeed = false; // 是否是换行 如果是换行 tag和content都为空

	bool operator==(const FTagWithContent& Other)
	{
		return Content.Equals(Other.Content) && Tag.Equals(Other.Tag) && Other.bIsLineFeed == bIsLineFeed;
	}

	// 获取需要显示的内容 带Tag
	FString GetContentWithWordNum(int32 WordNum) const;
};

/**
 *
 */
UCLASS(Blueprintable)
class UKAutoWriterRichText : public URichTextBlock 
{
public:
	GENERATED_BODY()

public:
	UKAutoWriterRichText(const FObjectInitializer& ObjectInitializer);

	// 解析输入的富文本结构
	static void ParseTagWithContentArray(const FString& Input, TArray<FTagWithContent>& Result);
	
	UFUNCTION(BlueprintCallable, Category = " AutoWrite")
	void RunWithText(const FText& OriginText); // 开始自动打字

	UFUNCTION(BlueprintCallable, Category = "AutoWrite")
	void Run(); // 开始自动打字

	UFUNCTION(BlueprintCallable, Category = "AutoWrite")
	void Pause(); // 暂停自动打字 对应Resume使用

	UFUNCTION(BlueprintCallable, Category = "AutoWrite")
	void Resume(); // 继续自动打字 对应Pause使用

	UFUNCTION(BlueprintCallable, Category = "AutoWrite")
	void Cancel(); // 关闭自动打字 清除数据 

	UFUNCTION(BlueprintPure, Category = "AutoWrite")
	const FText& GetOriginText() const; // 获得自动打字的文本

	UFUNCTION(BlueprintPure, Category = "AutoWrite")
	EAutoWriteState GetState() const; // 获取当前运行状态

	virtual void SetText(const FText& InText) override;

	virtual void ReleaseSlateResources(bool bReleaseChildren) override; // 释放资源

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAutoWriteStateChanged, EAutoWriteState, CurrenState, EAutoWriteState, NextState);

	/**
	 * 每次状态变更时都会触发该事件
	 */
	UPROPERTY(BlueprintAssignable, Category = "AutoWrite|Event")
	FOnAutoWriteStateChanged AutoWriteStateChanged;

	/**
	 * 每秒显示多少文字
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoWriteConfig")
	float ShowWordEverySecond;

	/**
	 * 在进入Cancel状态时 展示所有内容
	 * 适用于在自动打字过程中主动Cancel时显示所有内容
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AutoWriteConfig")
	bool bIsShowOriginInCancel;

protected:
	/**
	 * 使用Implement包装具体细节，如此所有对内容的修改 包括增删属性、函数都不会影响头文件 从而减少头文件修改带来的重新编译数量
	 */
	struct Implement;
	TSharedPtr<Implement> Impl;
};
