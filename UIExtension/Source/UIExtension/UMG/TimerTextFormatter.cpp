// Fill out your copyright notice in the Description page of Project Settings.


#include "TimerTextFormatter.h"
#include "Internationalization/Regex.h"

#define LOCTEXT_NAMESPACE "FTimerTextFormatter"

constexpr int32 DefaultPlaceHoldNum = 3;

static const FString ReTimerUnit = TEXT("\\{(%(\\d)([d|H|M|S|f]))\\}");

// 各个时间单位对应的秒数
static const TMap<ETimeUnitItem, int32> TimerTranslateMap = {
	{ETimeUnitItem::Day, 60 * 60 * 24},
	{ETimeUnitItem::Hour, 60 * 60},
	{ETimeUnitItem::Minute, 60},
	{ETimeUnitItem::Second, 1}
};

// 各个时间单位的格式化键名 与其对应的 枚举
static const TMap<FString, ETimeUnitItem> TimeUnitKeyWithType = {
	{TEXT("d"), ETimeUnitItem::Day},
	{TEXT("H"), ETimeUnitItem::Hour},
	{TEXT("M"), ETimeUnitItem::Minute},
	{TEXT("S"), ETimeUnitItem::Second},
	{TEXT("f"), ETimeUnitItem::MilliSecond},
};

// 将时间单位的键名转换为对应的枚举
static ETimeUnitItem ConvertFormatStrToTimeUnit(const FString& FormatStr)
{
	const auto& TimeUnit = TimeUnitKeyWithType.Find(FormatStr);
	if (TimeUnit != nullptr)
	{
		return *TimeUnit;
	}
	UE_LOG(LogTemp, Error, TEXT("Can not ConvertFormatStrToTimeUnit, This FormatStr is not valid"));
	return ETimeUnitItem::Day;
}

// 格式化时间字符串
static FString FormatValueWithPlaceNum(int32 PlaceNum, int32 Value)
{
	static constexpr TCHAR NonePlaceHoldResult[] = TEXT("%i");
	static constexpr TCHAR PlaceHoldResultOrigin[] = TEXT("%0*i");

	if (PlaceNum > 0)
	{
		return FString::Printf(PlaceHoldResultOrigin, PlaceNum, Value);
	}
	else
	{
		return FString::Printf(NonePlaceHoldResult, Value);
	}
}

// 根据秒 计算出各个时间单位的值
static void SetFormatUnitInfoValue(TMap<ETimeUnitItem, FTimeUnitInfo>& FormatMap, double TotalSeconds)
{
	for (auto& TimerUnitInfo : FormatMap)
	{
		const auto SecondTo = TimerTranslateMap.Find(TimerUnitInfo.Key);
		if (SecondTo == nullptr)
		{
			continue;
		}
		const int32 TimeItemValue = TotalSeconds / *SecondTo;
		TotalSeconds -= TimeItemValue * (*SecondTo);

		TimerUnitInfo.Value.TimeValue = TimeItemValue;
	}

	// 单独计算毫秒的值
	auto MilliSecondInfo = FormatMap.Find(ETimeUnitItem::MilliSecond);
	if (MilliSecondInfo != nullptr)
	{
		// 获取小数点位 
		const int32 MilliSecond = FMath::Fractional(TotalSeconds) * pow(10, MilliSecondInfo->PlaceHold);
		MilliSecondInfo->TimeValue = MilliSecond;
	}
}

// 根据配置 替换指定的key 为时间项
static FText FormatTimerText(const FText& OriginFormatText, TMap<ETimeUnitItem, FTimeUnitInfo>& TimeUnitInfo)
{
	FFormatNamedArguments NameArgs;
	for (auto& TimerUnitInfo : TimeUnitInfo)
	{
		FString TimeUnitString = FormatValueWithPlaceNum(TimerUnitInfo.Value.PlaceHold, TimerUnitInfo.Value.TimeValue);
		NameArgs.Add(TimerUnitInfo.Value.FormatKey, FText::FromString(TimeUnitString));
	}

	return FText::Format(OriginFormatText, NameArgs);
}

FTimerTextFormatterText::FTimerTextFormatterText()
{
	FormatTimeText = LOCTEXT("Text FormatTimeText", "remain: {%4d} D {%4H} Hour {%4M} M {%4S} S {%4f} MS ");
}

FText FTimerTextFormatterText::FormatItemWithMap(double TotalSeconds)
{
	// 计算各个时间单位的值
	SetFormatUnitInfoValue(TimeUnitInfoMap, TotalSeconds);
	// 将时间单位的值转化成Text输出
	return FormatTimerText(FormatTimeText, TimeUnitInfoMap);
}

void FTimerTextFormatterText::InitData()
{
	const FString& OriginFormatString = FormatTimeText.ToString();

	static FRegexPattern MatherPatter(ReTimerUnit);
	FRegexMatcher Matcher(MatherPatter, OriginFormatString);

	// 预处理数据 计算出各个 时间单位 的键、占位数值
	while (Matcher.FindNext())
	{
		FString ContentKey = *Matcher.GetCaptureGroup(1);
		int32 PlaceHoldNum = FCString::Atoi(*Matcher.GetCaptureGroup(2));
		FString FormatKey = Matcher.GetCaptureGroup(3);
		ETimeUnitItem TimerUnitType = ConvertFormatStrToTimeUnit(FormatKey);
		TimeUnitInfoMap.Add(TimerUnitType, {ContentKey, PlaceHoldNum});
	}
}

FTimerTextFormatterMap::FTimerTextFormatterMap()
{
	FormatMap = {
		{ETimeUnitItem::Day, LOCTEXT("Map Day", "{%3d} Day")},
		{ETimeUnitItem::Hour, LOCTEXT("Map Hour", "{%3H} Hour")},
		{ETimeUnitItem::Minute, LOCTEXT("Map Minute", "{%3M} Minute")},
		{ETimeUnitItem::Second, LOCTEXT("Map Second", "{%3S} Second")},
		{ETimeUnitItem::MilliSecond, LOCTEXT("Map MilliSecond", "{%3f} MilliSecond")},
	};

	FinalFormatTimeText = LOCTEXT("Map FinalFormatTime", "Remain：{Final}……");

	FormatSequence = {
		ETimeUnitItem::Day, ETimeUnitItem::Hour, ETimeUnitItem::Minute, ETimeUnitItem::Second, ETimeUnitItem::MilliSecond
	};
}

FText FTimerTextFormatterMap::FormatItemWithMap(double TotalSeconds)
{
	// 计算各个时间单位的值
	SetFormatUnitInfoValue(TimeUnitInfoMap, TotalSeconds);
	const FString Result = ComposeFormatString();
	const FText TimerContent = FormatTimerText(FText::FromString(Result), TimeUnitInfoMap);
	FFormatNamedArguments NameArgs;
	NameArgs.Add(TEXT("Final"), TimerContent);
	return FText::Format(FinalFormatTimeText, NameArgs);
}

TArray<ETimeUnitItem> FTimerTextFormatterMap::GetShowTimeUnitSequence()
{
	if (FormatRule == ETimerFormatRule::Normal)
	{
		return FormatSequence;
	}

	auto Result = FormatSequence;

	// 因为是Map 所以Key按顺序来的 
	for (const auto& TimeUnitInfo : TimeUnitInfoMap)
	{
		if (LessShowNum >= Result.Num())
		{
			break;
		}

		if (TimeUnitInfo.Value.TimeValue == 0)
		{
			Result.Remove(TimeUnitInfo.Key);
		}
		else
		{
			break;
		}
	}

	return Result;
}

FString FTimerTextFormatterMap::ComposeFormatString()
{
	auto FormatItemSeq = GetShowTimeUnitSequence();
	FString Result;
	for (const auto& TimeItem : FormatItemSeq)
	{
		const auto FormatText = FormatMap.Find(TimeItem);
		if (FormatText == nullptr)
		{
			continue;
		}
		Result.Append(FormatText->ToString());
	}
	return Result;
}

void FTimerTextFormatterMap::InitData()
{
	static FRegexPattern MatherPatter(ReTimerUnit);

	// 预处理数据 计算出各个 时间单位 的键、占位数值
	for (const auto& TimeUnitItem : FormatMap)
	{
		const FText FormatText = TimeUnitItem.Get<1>();
		const FString& OriginFormatString = FormatText.ToString();
		FRegexMatcher Matcher(MatherPatter, OriginFormatString);

		if (Matcher.FindNext())
		{
			FString ContentKey = *Matcher.GetCaptureGroup(1);
			int32 PlaceHoldNum = FCString::Atoi(*Matcher.GetCaptureGroup(2));;
			FString FormatKey = Matcher.GetCaptureGroup(3);
			ETimeUnitItem TimerUnitType = ConvertFormatStrToTimeUnit(FormatKey);
			TimeUnitInfoMap.Add(TimerUnitType, {ContentKey, PlaceHoldNum});
		}
	}
}

void FTimerTextFormatter::UpdateData()
{
	FormatterText.InitData();
	FormatterMap.InitData();
}

FText FTimerTextFormatter::FormatItemWithMap(double TotalSeconds)
{
	switch (FormatType)
	{
	case ETimeFormatType::FormatByMap:
		return FormatterMap.FormatItemWithMap(TotalSeconds);
	case ETimeFormatType::FormatByText:
		return FormatterText.FormatItemWithMap(TotalSeconds);
	}
	return FText::FromString(FString());
}

#undef LOCTEXT_NAMESPACE
