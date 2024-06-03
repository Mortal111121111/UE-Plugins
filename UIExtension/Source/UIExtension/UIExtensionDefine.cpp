// Fill out your copyright notice in the Description page of Project Settings.


#include "UIExtensionDefine.h"

#include "Kismet/KismetSystemLibrary.h"

FTimingTool::FTimingTool(UObject *Owner)
	: Owner(Owner)
{
}

void FTimingTool::Start()
{
	if (!Owner.IsValid())
	{
		return;
	}
	Reset();
	StartTime = GetSeconds();
}

void FTimingTool::Pause()
{
	if (bIsPaused || !Owner.IsValid())
	{
		return;
	}
	bIsPaused = true;
	PauseTime = GetSeconds();
}

void FTimingTool::Continue()
{
	if (!bIsPaused || !Owner.IsValid())
	{
		return;
	}
	bIsPaused = false;
	const auto CurrentTime = GetSeconds();
	TotalPausedTime += CurrentTime - PauseTime;
}

void FTimingTool::Reset()
{
	StartTime = 0;
	PauseTime = 0;
	TotalPausedTime = 0;
	bIsPaused = false;
}

float FTimingTool::GetElapsedTime() const
{
	const float PausingTime = bIsPaused ? GetSeconds() - PauseTime : 0;
	return GetSeconds() - StartTime - TotalPausedTime - PausingTime;
}

float FTimingTool::GetSeconds() const
{
	return UKismetSystemLibrary::GetGameTimeInSeconds(Owner.Get());
}
