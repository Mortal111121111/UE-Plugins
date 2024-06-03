// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimerTextFormatter.generated.h"

UENUM(BlueprintType)
enum class ETimeFormatType : uint8
{
	FormatByText 		UMETA(DisplayName="通过FormatText配置格式化"),
	FormatByMap			UMETA(DisplayName="通过FormatMap配置格式化"),
};

// 涉及到秒转换到各个时间 所以一定要从大到小
UENUM(BlueprintType)
enum class ETimeUnitItem : uint8
{
	Day 		UMETA(DisplayName="天"),// 天
	Hour		UMETA(DisplayName="小时"),// 小时
	Minute		UMETA(DisplayName="分钟"),// 分钟
	Second		UMETA(DisplayName="秒"),// 秒
	MilliSecond	UMETA(DisplayName="毫秒"),// 毫秒
};

UENUM(BlueprintType)
enum class ETimerFormatRule : uint8
{
	Normal		UMETA(DisplayName="正常解析"),// 正常解析
	NoShowEmpty	UMETA(DisplayName="不显示为空的时间"),// 不显示为空的时间 0天1小时0分钟0秒 => 1小时0分钟1秒
};

struct FTimeUnitInfo
{
	FTimeUnitInfo()
	{}
	FTimeUnitInfo(const FString& Key, const int32 &PlaceHoldNum)
		: FormatKey(Key), PlaceHold(PlaceHoldNum)
	{}
	FString FormatKey = FString();
	int32 TimeValue = 0;
	int32 PlaceHold = 0;
};

USTRUCT(BlueprintType)
struct FTimerTextFormatterText
{
	GENERATED_BODY()
	
public:
	FTimerTextFormatterText();
	
	FText FormatItemWithMap(double TotalSeconds);

	void InitData();
	
public:	
	/**
	 * 格式化Timer 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TimerTextFormatterText");
	FText FormatTimeText;

private:
	TMap<ETimeUnitItem, FTimeUnitInfo> TimeUnitInfoMap;
};

USTRUCT(BlueprintType)
struct FTimerTextFormatterMap
{
	GENERATED_BODY()
	
public:
	FTimerTextFormatterMap();
	
	FText FormatItemWithMap(double TotalSeconds);

	void InitData();
	
private:
	TArray<ETimeUnitItem> GetShowTimeUnitSequence();

	FString ComposeFormatString();
	
public:
	/**
	 * 通过格式化字符数组 来格式化时间 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TimerTextFormatterMap");
	TMap<ETimeUnitItem, FText> FormatMap;

	/**
	 * 拼接时间字符串的顺序
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TimerTextFormatterMap");
	TArray<ETimeUnitItem> FormatSequence;
	
	/**
	 * 最终格式化的样式 最终将内容填写在 {Final} 中，所以必须要有 {Final}
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TimerTextFormatterMap");
	FText FinalFormatTimeText;
	
	/**
	 * 在格式化字符串的规则
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TimerTextFormatterMap");
	ETimerFormatRule FormatRule = ETimerFormatRule::Normal;

	/**
	 * 最少展示多少个时间词条
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TimerTextFormatterMap", meta=(ClampMin=2));
	int32 LessShowNum = 2;

private:
	TMap<ETimeUnitItem, FTimeUnitInfo> TimeUnitInfoMap;
};

USTRUCT(BlueprintType)
struct FTimerTextFormatter
{
	GENERATED_BODY()

	void UpdateData();

	FText FormatItemWithMap(double TotalSeconds);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TimerTextFormatter", meta=(ClampMin=2));
	ETimeFormatType FormatType = ETimeFormatType::FormatByText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TimerTextFormatter", meta=(ClampMin=2));
	FTimerTextFormatterMap FormatterMap;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TimerTextFormatter", meta=(ClampMin=2));
	FTimerTextFormatterText FormatterText;
};