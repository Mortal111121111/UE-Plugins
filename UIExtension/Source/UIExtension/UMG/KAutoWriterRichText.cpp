// Fill out your copyright notice in the Description page of Project Settings.

#include "KAutoWriterRichText.h"
#include "Components/RichTextBlock.h"
#include "Framework/Text/RichTextMarkupProcessing.h"
#include "UIExtension/UIExtensionDefine.h"
#include "Widgets/Text/SRichTextBlock.h"

const float UPDATE_TIMER_DELAY = 0.03f; // 自动打字机更新TimerDelay时间间隔
const float DEFAULT_SHOW_NUM_SECONDS = 1.0f; // 默认每秒显示文本个数

FTagWithContent::FTagWithContent(const FString& NodeTag, const FString& NodeContent)
{
	Tag = NodeTag;
	Content = NodeContent;
	bIsLineFeed = false;
}

FString FTagWithContent::GetContentWithWordNum(int32 WordNum) const
{
	static const FString EndTag = TEXT("</>");
	static const FString LineFeed = TEXT("\r\n");
	FString Result;
	WordNum = WordNum >= Content.Len() ? Content.Len() : WordNum;
	if (Tag.IsEmpty())
	{
		Result = Content.Left(WordNum);
	}
	else
	{
		Result = Tag + Content.Left(WordNum) + EndTag;
	}
	// 当前仅当需要全部显示时才需要Append换行
	if (bIsLineFeed && WordNum >= Content.Len())
	{
		Result.Append(LineFeed);
	}
	return Result;
}

struct UKAutoWriterRichText::Implement
{
public:
	Implement(UKAutoWriterRichText* InOwner)
	{
		Owner = InOwner;
		TimeCalTool = MakeShared<FTimingTool>(InOwner);
		CurrentAutoWriteState = EAutoWriteState::Closed;
		CurrentShowWordCount = 0;
		bIsDispatchCancelEvent = false;
	}

	void SetTextImpl(const FText& InText)
	{
		Owner->Super::SetText(InText);
	}

	int32 CalDisplayTextLength() // 获得当前文本的长度
	{
		int32 Length = 0;
		for (const FTagWithContent& Item : TagWithContentArray)
		{
			Length += Item.Content.Len();
		}
		return Length;
	}

	void InitWriterInfo() // 初始化参数
	{
		// 已经把SetText接口屏蔽掉了 所以GetText最初获得的是蓝图中设置的数据
		OriginText = OriginText.IsEmpty() ? Owner->GetText() : OriginText;

		const FString& TextContent = OriginText.ToString();
		UKAutoWriterRichText::ParseTagWithContentArray(TextContent, TagWithContentArray);
		CurrentAutoWriteState = EAutoWriteState::Closed;
		CurrentShowWordCount = 0;
		ContentLength = CalDisplayTextLength();
	}

	FString GetDisplayWords(int32 WordNum) // 根据数目显示需要显示的文本
	{
		FString Result;
		int32 CurWordNum = 0;
		for (const auto& Item : TagWithContentArray)
		{
			if (CurWordNum + Item.Content.Len() < WordNum)
			{
				Result = Result + Item.GetContentWithWordNum(Item.Content.Len());
				CurWordNum += Item.Content.Len();
			}
			else
			{
				Result = Result + Item.GetContentWithWordNum(WordNum - CurWordNum);
				break;
			}
		}

		return Result;
	}

	void UpdateRichTextContent() // Timer回调 更新文本内容
	{
		if (CurrentAutoWriteState != EAutoWriteState::Running)
		{
			// 不做处理
			return;
		}

		/* 计算当前应该显示多少字符
		 * 如果单纯用 开始Run的时间与当前时间的时间差 * ShowWordEverySecond 计算显示文本个数
		 * 那么当 播放倍率*2 的时候 显示的文本数量会瞬间变为俩倍
		 * 那么当 播放倍率*0.5 的时候 显示的文本数量会瞬间减半 这是很明显的显示bug
		 * 所以用 CurrentShowWordCount 之前的显示文本不受影响，并且每次只计算与上次Timer执行之间的时间差 保证只有当前Timer的需要被计算
		 */
		float WordCount = CurrentShowWordCount + TimeCalTool->GetElapsedTime() * Owner->ShowWordEverySecond;
		WordCount = WordCount >= 1.0f ? WordCount : 1;
		const FString ShowContent = GetDisplayWords(WordCount);

		CurrentShowWordCount = WordCount;
		TimeCalTool->Start();

		SetTextImpl(FText::FromString(ShowContent));

		// 显示数量超过最多文本数量 进入关闭状态
		if (WordCount >= ContentLength)
		{
			Cancel();
		}
	}

	void UpdateAutoWriteState(EAutoWriteState nextState, bool bIsDispatch = true) // 发送事件、更新状态、更新定时器的开启/关闭
	{
		CurrentAutoWriteState = nextState;
		Owner->GetWorld()->GetTimerManager().ClearTimer(AutoWriteTimerHandle);
		if (CurrentAutoWriteState == EAutoWriteState::Running)
		{
			Owner->GetWorld()->GetTimerManager().SetTimer(AutoWriteTimerHandle, FTimerDelegate::CreateRaw(this, &Implement::UpdateRichTextContent), UPDATE_TIMER_DELAY, true);
		}
		if (bIsDispatch)
		{
			Owner->AutoWriteStateChanged.Broadcast(GetCurrentAutoWriteState(), nextState);
		}
	}

	const FText& GetOriginText() const
	{
		return OriginText;
	}

	void SetOriginText(const FText& InText)
	{
		OriginText = InText;
	}

	EAutoWriteState GetCurrentAutoWriteState() const
	{
		return CurrentAutoWriteState;
	}

	void ReleaseSlateResources(bool bReleaseChildren)
	{
		if (!Owner)
		{
			return;
		}
		// 防止CDO销毁清除资源 那时没有World
		if (Owner->GetWorld())
		{
			Owner->GetWorld()->GetTimerManager().ClearTimer(AutoWriteTimerHandle);
		}
		Owner->Super::ReleaseSlateResources(bReleaseChildren);
	}

	void Run()
	{
		if (CurrentAutoWriteState != EAutoWriteState::Closed)
		{
			// 主动结束上个就不发送事件了 避免外界刚 Run 就收到结束的事件
			Cancel(false);
		}
		InitWriterInfo();
		TimeCalTool->Start();
		UpdateAutoWriteState(EAutoWriteState::Running);
		UpdateRichTextContent(); // Timer会在指定时间后才会执行 需要手动立即执行一次
	}

	void Cancel(bool bIsDispatchEvent = true)
	{
		if (CurrentAutoWriteState == EAutoWriteState::Closed)
		{
			return;
		}
		TimeCalTool->Reset();
		if (Owner->bIsShowOriginInCancel)
		{
			SetTextImpl(OriginText);
		}
		UpdateAutoWriteState(EAutoWriteState::Closed, bIsDispatchEvent);
	}

	void Pause()
	{
		if (CurrentAutoWriteState != EAutoWriteState::Running)
		{
			// 只有运行状态才能进入Stop
			return;
		}
		TimeCalTool->Pause();
		UpdateAutoWriteState(EAutoWriteState::Stopped);
	}

	void Resume()
	{
		if (CurrentAutoWriteState != EAutoWriteState::Stopped)
		{
			// 只有关闭状态才能进入Stop
			return;
		}
		TimeCalTool->Continue();
		UpdateAutoWriteState(EAutoWriteState::Running);
	}

private:
	TArray<FTagWithContent> TagWithContentArray; // tag 与 内容的数组

	EAutoWriteState CurrentAutoWriteState; // 当前状态 暂停、运行、关闭

	int32 ContentLength; // 总文字个数

	float CurrentShowWordCount; // 当前显示文本个数

	TSharedPtr<FTimingTool> TimeCalTool; // 记录时间工具

	FTimerHandle AutoWriteTimerHandle; // 定时刷新文本 Handle

	FText OriginText; // 源文本 为了防止SetText对源文本产生影响 新建一个属性存储源文本

	bool bIsDispatchCancelEvent;

	UKAutoWriterRichText* Owner;
};

UKAutoWriterRichText::UKAutoWriterRichText(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), Impl(new Implement(this))
{
	ShowWordEverySecond = DEFAULT_SHOW_NUM_SECONDS;
	bIsShowOriginInCancel = true;
}

void UKAutoWriterRichText::ParseTagWithContentArray(const FString& Input, TArray<FTagWithContent>& Result)
{
	Result.Empty();
	TArray<FTextLineParseResults> ParseResult;
	FString Output;
	const auto Parser = FDefaultRichTextMarkupParser::GetStaticInstance();


	static const TCHAR TagFormatStr[] = TEXT("<%s>");
	// Output中剔除了Input中的 \r\n 部分，ParseResult中的字符范围也是剔除之后的序号
	Parser->Process(ParseResult, Input, Output);

	for (auto& ResultItem : ParseResult)
	{
		for (const auto& Item : ResultItem.Runs)
		{
			auto Tag = Item.Name;
			FString Content;
			if (Tag.IsEmpty())
			{
				Content = Output.Mid(Item.OriginalRange.BeginIndex, Item.OriginalRange.EndIndex - Item.OriginalRange.BeginIndex);
			}
			else
			{
				Tag = FString::Printf(TagFormatStr, *Tag);
				Content = Output.Mid(Item.ContentRange.BeginIndex, Item.ContentRange.EndIndex - Item.ContentRange.BeginIndex);
			}
			Result.Push(FTagWithContent(Tag, Content));
		}
		if (Result.Num() > 0)
		{
			// 标记每行最后一个的换行
			Result.Last().bIsLineFeed = true;
		}
	}

	// 去除最后一个无效的回车
	if (Result.Num() > 0)
	{
		Result.Last().bIsLineFeed = false;
	}
}

void UKAutoWriterRichText::RunWithText(const FText& OriginText)
{
	Impl->SetOriginText(OriginText);
	Impl->Run();
}

void UKAutoWriterRichText::ReleaseSlateResources(bool bReleaseChildren)
{
	Impl->ReleaseSlateResources(bReleaseChildren);
}

EAutoWriteState UKAutoWriterRichText::GetState() const
{
	return Impl->GetCurrentAutoWriteState();
}

void UKAutoWriterRichText::SetText(const FText& InText)
{
	UE_LOG(LogTemp, Warning, TEXT("AutoWriteRichText Can Not Use SetText Functin."));
	// Super::SetText(InText);
}

void UKAutoWriterRichText::Run()
{
	Impl->Run();
}

void UKAutoWriterRichText::Pause()
{
	Impl->Pause();
}

void UKAutoWriterRichText::Resume()
{
	Impl->Resume();
}

void UKAutoWriterRichText::Cancel()
{
	Impl->Cancel();
}

const FText& UKAutoWriterRichText::GetOriginText() const
{
	return Impl->GetOriginText();
}
