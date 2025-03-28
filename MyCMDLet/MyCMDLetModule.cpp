// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyCMDLetModule.h"

#define LOCTEXT_NAMESPACE "MyCMDLetModule"

void FMyCMDLetModule::StartupModule()
{
}

void FMyCMDLetModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMyCMDLetModule, MyCMDLetEditorMode)