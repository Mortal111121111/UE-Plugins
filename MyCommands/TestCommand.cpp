// Fill out your copyright notice in the Description page of Project Settings.


#include "TestCommand.h"

#define LOCTEXT_NAMESPACE "FTestYaksueModule"

FTestCommands::FTestCommands()
	:TCommands<FTestCommands>("TestCommands", NSLOCTEXT("Contexts", "TestYaksueCommands", "TestYaksue Plugin"), NAME_None,FName(*FString("TestCommands")))
{
}

void FTestCommands::RegisterCommands()
{
	UI_COMMAND(CommandA, "TestCommands", "Execute TestCommands CommandA", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
