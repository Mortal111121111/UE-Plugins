// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

struct FTimingTool
{
	FTimingTool(UObject* Owner); // 初始化计时工具

	void Start(); // 开始计时

	void Pause(); // 暂停计时

	void Continue(); // 继续计时

	void Reset(); // 清除计时

	float GetElapsedTime() const; // 获得从开始计时到现在总归计算的时间

private:
	float GetSeconds() const; // 封装获取当前时间的接口

private:
	float StartTime{0}; // 开始时间
	float PauseTime{0}; // 暂停开始的时间
	float TotalPausedTime{0}; // 总共暂停时间 单位秒
	bool bIsPaused{false}; // 当前是否正在暂停
	TWeakObjectPtr<UObject> Owner; // 需要UObject获取当前时间
};
