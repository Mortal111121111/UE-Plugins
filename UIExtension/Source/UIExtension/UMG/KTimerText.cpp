// Fill out your copyright notice in the Description page of Project Settings.

#include "KTimerText.h"

#include "UIExtension/UIExtensionDefine.h"


struct UKTimerText::Implement
{
public:
	Implement(UKTimerText *InOwn)
	{
		Owner = InOwn;
		CurrentState = ETimerState::Closed;
		TimeCalTool = MakeShared<FTimingTool>(InOwn);
	}

	void Start()
	{
		if (CurrentState != ETimerState::Closed)
		{
			Cancel(true);
		}

		TimeCalTool->Start();
		UpdateState(ETimerState::Running);
		UpdateText(); // 手动调用一次 定时器会在下一次时间到的时候调用 有延迟
	}

	void Pause()
	{
		if (CurrentState != ETimerState::Running)
		{
			UE_LOG(LogTemp, Warning, TEXT("ColdDownTimeText Current Is Not Running, Can Not Pause"));
			return;
		}
		TimeCalTool->Pause();
		UpdateState(ETimerState::Pause);
	}

	void Resume()
	{
		if (CurrentState != ETimerState::Pause)
		{
			UE_LOG(LogTemp, Warning, TEXT("ColdDownTimeText Current Is Not Pause, Can Not Resume"));
			return;
		}
		TimeCalTool->Continue();
		UpdateState(ETimerState::Running);
	}

	void Cancel(bool bIsInterrupt = false)
	{
		if (CurrentState == ETimerState::Closed)
		{
			UE_LOG(LogTemp, Warning, TEXT("ColdDownTimeText Current Was Closed, Can Not Cancel"));
			return;
		}
		TimeCalTool->Reset();
		UpdateState(ETimerState::Closed);
		Owner->ColdDownFinished.Broadcast(bIsInterrupt);
	}

	ETimerState GetState() const
	{
		return CurrentState;
	}

	double GetRemainTime() const
	{
		double RemainTime = Owner->CountDown - TimeCalTool->GetElapsedTime();
		RemainTime = (RemainTime - 0) < 1e-6 ? 0 : RemainTime;
		return RemainTime;
	}

	double GetElapsedTime() const
	{
		return TimeCalTool->GetElapsedTime();
	}

	double GetDisplayTime() const
	{
		switch (Owner->TimerType)
		{
		case ETimerCountType::Timing:
			return GetElapsedTime() + Owner->BaseTiming;
		case ETimerCountType::CountDown:
			return GetRemainTime();
		}
		return 0;
	}

	void UpdateText()
	{
		Owner->Super::SetText(Owner->TimeFormat.FormatItemWithMap(GetDisplayTime()));
		if (ShouldCancel())
		{
			Cancel();
		}
	}

protected:
	void UpdateState(ETimerState NewState)
	{
		CurrentState = NewState;
		Owner->GetWorld()->GetTimerManager().ClearTimer(ColdDownTimerHandle);
		if (CurrentState == ETimerState::Running)
		{
			Owner->GetWorld()->GetTimerManager().SetTimer(ColdDownTimerHandle, FTimerDelegate::CreateRaw(this, &Implement::UpdateText), Owner->TimerDelay, true);
		}
	}

	bool ShouldCancel() const
	{
		switch (Owner->TimerType)
		{
		case ETimerCountType::Timing:
			// 暂时不做处理
			break;
		case ETimerCountType::CountDown:
			return GetRemainTime() <= 0;
		}
		return false;
	}

private:
	TSharedPtr<FTimingTool> TimeCalTool; // 记录时间工具

	ETimerState CurrentState;

	UKTimerText *Owner;

	FTimerHandle ColdDownTimerHandle;
};

UKTimerText::UKTimerText(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer), Impl(new Implement(this))
{
}

void UKTimerText::StartWithCountDown(int32 ColdDownTime)
{
	TimeFormat.UpdateData();
	CountDown = ColdDownTime;
	TimerType = ETimerCountType::CountDown;
	Impl->Start();
}

void UKTimerText::StartWithTiming(int32 BaseTime)
{
	TimeFormat.UpdateData();
	BaseTiming = BaseTime;
	TimerType = ETimerCountType::Timing;
	Impl->Start();
}

void UKTimerText::Cancel()
{
	Impl->Cancel(true);
}

bool UKTimerText::IsRunning()
{
	return Impl->GetState() == ETimerState::Running;
}

void UKTimerText::Pause()
{
	Impl->Pause();
}

void UKTimerText::Resume()
{
	Impl->Resume();
}

double UKTimerText::GetRemainTime() const
{
	return Impl->GetRemainTime();
}

double UKTimerText::GetElapsedTime() const
{
	return Impl->GetElapsedTime();
}

void UKTimerText::SetText(FText InText)
{
	UE_LOG(LogTemp, Warning, TEXT("ColdDownTimeText Can Not SetText"));
	// Super::SetText(InText);
}
