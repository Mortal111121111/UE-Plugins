// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimerTextFormatter.h"
#include "Components/TextBlock.h"
#include "KTimerText.generated.h"

UENUM(BlueprintType)
enum class ETimerState : uint8
{
	Closed, // 关闭
	Pause,	// 暂停
	Running // 运行中
};

UENUM(BlueprintType)
enum class ETimerCountType : uint8
{
	CountDown, // 倒计时
	Timing	   // 计时
};

UCLASS(Blueprintable)
class UKTimerText : public UTextBlock
{
	GENERATED_BODY()

public:
	UKTimerText(const FObjectInitializer &ObjectInitializer);

	/**
	 * 以倒计时开始 设定时间
	 */
	UFUNCTION(BlueprintCallable, Category = "KTimerText")
	void StartWithCountDown(int32 ColdDownTime);

	/**
	 * 以计时开始 设定基础时间
	 */
	UFUNCTION(BlueprintCallable, Category = "KTimerText")
	void StartWithTiming(int32 BaseTime);

	UFUNCTION(BlueprintCallable, Category = "KTimerText")
	void Cancel();

	UFUNCTION(BlueprintCallable, Category = "KTimerText")
	bool IsRunning();

	UFUNCTION(BlueprintCallable, Category = "KTimerText")
	void Pause();

	UFUNCTION(BlueprintCallable, Category = "KTimerText")
	void Resume();

	/**
	 * 获取倒计时剩余时间
	 */
	UFUNCTION(BlueprintCallable, Category = "KTimerText")
	double GetRemainTime() const;

	/**
	 * 获取从开始到现在 总共经历的时间
	 */
	UFUNCTION(BlueprintCallable, Category = "KTimerText")
	double GetElapsedTime() const;

	virtual void SetText(FText InText) override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnColdDownFinished, bool, bIsInterrupt);
	/**
	 * 倒计时结束时会触发该事件
	 */
	UPROPERTY(BlueprintAssignable, Category = "UKTimerText|Event")
	FOnColdDownFinished ColdDownFinished;

public:
	/**
	 * 格式化字符串
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KTimerText")
	FTimerTextFormatter TimeFormat;

	/**
	 * Timer Delay的时间间隔
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KTimerText")
	double TimerDelay = 0.03;

	/**
	 * 倒计时类型 CountDown 为倒计时 Timing 为计时
	 */
	ETimerCountType TimerType;

	/**
	 * 计时时 基础时间
	 */
	double BaseTiming = 0.0;

	/**
	 * 默认倒计时时间
	 */
	double CountDown = 0.0;

	/**
	 * 使用Implement包装具体细节，如此所有对内容的修改 包括增删属性、函数都不会影响头文件 从而减少头文件修改带来的重新编译数量
	 */
	struct Implement;
	TSharedPtr<Implement> Impl;
};
