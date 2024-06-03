// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_CloseUI.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "EditorCategoryUtils.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "UIManager/Lib/UIManagerLib.h"

namespace CloseUIHelper
{
	const FName WorldPinName = "WorldContextObject";
	const FName UINamePinName = "UIName";
}


void UK2Node_CloseUI::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();

	// Input
	
	// Add execution pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, UObject::StaticClass(), CloseUIHelper::WorldPinName);

	// UI Name pin
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, CloseUIHelper::UINamePinName);

	// Output
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);

	Super::AllocateDefaultPins();
}

FText UK2Node_CloseUI::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString("Close UI");
}

void UK2Node_CloseUI::PinDefaultValueChanged(UEdGraphPin* Pin)
{
	Super::PinDefaultValueChanged(Pin);
}

FText UK2Node_CloseUI::GetTooltipText() const
{
	return FText::FromString("");
}

void UK2Node_CloseUI::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	// FUNCTION NODE
	const FName FunctionName = GET_FUNCTION_NAME_CHECKED(UUIManagerLib, CloseUI);
	UK2Node_CallFunction* OpenUIFunction = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	OpenUIFunction->FunctionReference.SetExternalMember(FunctionName, UUIManagerLib::StaticClass());
	OpenUIFunction->AllocateDefaultPins();

	// Link
	CompilerContext.MovePinLinksToIntermediate(*GetExecPin(), *(OpenUIFunction->GetExecPin()));
	
	UEdGraphPin* WorldContextObjectFuncPin = OpenUIFunction->FindPinChecked(TEXT("WorldContextObject"));
	CompilerContext.MovePinLinksToIntermediate(*GetWorldContextObjectPin(), *WorldContextObjectFuncPin);

	UEdGraphPin* UINameFuncPin = OpenUIFunction->FindPinChecked(TEXT("UIName"));
	CompilerContext.MovePinLinksToIntermediate(*GetUINamePin(), *UINameFuncPin);
	
	UEdGraphPin* ThenFuncPin = OpenUIFunction->GetThenPin();
	CompilerContext.MovePinLinksToIntermediate(*GetThenPin(), *ThenFuncPin);
	
	BreakAllNodeLinks();
}

FSlateIcon UK2Node_CloseUI::GetIconAndTint(FLinearColor& OutColor) const
{
	OutColor = GetNodeTitleColor();
	static FSlateIcon Icon("EditorStyle", "Kismet.AllClasses.FunctionIcon");
	return Icon;
}

void UK2Node_CloseUI::PostReconstructNode()
{
	Super::PostReconstructNode();
}

void UK2Node_CloseUI::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	const UClass* ActionKey = GetClass();
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

FText UK2Node_CloseUI::GetMenuCategory() const
{
	return FEditorCategoryUtils::GetCommonCategory(FCommonEditorCategory::Utilities);;
}

void UK2Node_CloseUI::EarlyValidation(FCompilerResultsLog& MessageLog) const
{
	Super::EarlyValidation(MessageLog);
}

void UK2Node_CloseUI::NotifyPinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::NotifyPinConnectionListChanged(Pin);

	RefreshUINameOptions();
}

UEdGraphPin* UK2Node_CloseUI::GetThenPin() const
{
	UEdGraphPin* Pin = FindPinChecked(UEdGraphSchema_K2::PN_Then);
	check(Pin->Direction == EGPD_Output);
	return Pin;
}

UEdGraphPin* UK2Node_CloseUI::GetWorldContextObjectPin() const
{
	UEdGraphPin* Pin = FindPinChecked(CloseUIHelper::WorldPinName);
	check(Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_CloseUI::GetUINamePin() const
{
	UEdGraphPin* Pin = FindPinChecked(CloseUIHelper::UINamePinName);
	check(Pin->Direction == EGPD_Input);
	return Pin;
}

void UK2Node_CloseUI::SetDefaultValueUINamePin() const
{
	UEdGraphPin* UINamePin = GetUINamePin();
	UINamePin->DefaultValue = "None";
}

void UK2Node_CloseUI::RefreshUINameOptions() const
{
	SetDefaultValueUINamePin();

	UEdGraph* Graph = GetGraph();
	Graph->NotifyGraphChanged();
}
