// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToastDefine.generated.h"

UENUM()
enum class EToastShowType
{
	// TODO
	/* 多个显示，先出的会向上移动（待实现） */
	Multi,
	/* 单个显示，一个一个出*/
	Single,
};

