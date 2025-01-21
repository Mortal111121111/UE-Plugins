// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_OpenUI.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "EditorCategoryUtils.h"
#include "K2Node_CallFunction.h"
#include "K2Node_IfThenElse.h"
#include "KismetCompiler.h"
#include "Blueprint/UserWidget.h"
#include "UIManager/Lib/UIManagerLib.h"

class UK2Node_CallFunction;

namespace OpenUIHelper
{
	const FName WorldPinName = "WorldContextObject";
	const FName UINamePinName = "UIName";
	const FName ParamsPinName = "Params";
	const FName CreateFailedPinName = "CreateFailed";
}


void UK2Node_OpenUI::AllocateDefaultPins()
{
	// Input
	
	// Add execution pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, UObject::StaticClass(), OpenUIHelper::WorldPinName);

	// UI Name pin
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, OpenUIHelper::UINamePinName);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, OpenUIHelper::ParamsPinName);

	// Output
	UEdGraphPin* SucceedPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
	SucceedPin->PinFriendlyName = FText::FromString("CreateSucceed");
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, OpenUIHelper::CreateFailedPinName);

	// Result pin
	UEdGraphPin* ResultPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Object, UUserWidget::StaticClass(),UEdGraphSchema_K2::PN_ReturnValue);
	ResultPin->PinFriendlyName = FText::FromString("Widget");

	Super::AllocateDefaultPins();
}

void UK2Node_OpenUI::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	// FUNCTION NODE
	const FName FunctionName = GET_FUNCTION_NAME_CHECKED(UUIManagerLib, OpenUI);
	UK2Node_CallFunction* OpenUIFunction = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	OpenUIFunction->FunctionReference.SetExternalMember(FunctionName, UUIManagerLib::StaticClass());
	OpenUIFunction->AllocateDefaultPins();

	// Link
	CompilerContext.MovePinLinksToIntermediate(*GetExecPin(), *(OpenUIFunction->GetExecPin()));
	
	UEdGraphPin* WorldContextObjectFuncPin = OpenUIFunction->FindPinChecked(OpenUIHelper::WorldPinName);
	CompilerContext.MovePinLinksToIntermediate(*GetWorldContextObjectPin(), *WorldContextObjectFuncPin);

	UEdGraphPin* UINameFuncPin = OpenUIFunction->FindPinChecked(OpenUIHelper::UINamePinName);
	CompilerContext.MovePinLinksToIntermediate(*GetUINamePin(), *UINameFuncPin);

	UEdGraphPin* ParamsFuncPin = OpenUIFunction->FindPinChecked(OpenUIHelper::ParamsPinName);
	CompilerContext.MovePinLinksToIntermediate(*GetParamsPin(), *ParamsFuncPin);

	UEdGraphPin* OutWidgetFuncPin = OpenUIFunction->FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
	UEdGraphPin* ResultPin = GetResultPin();
	OutWidgetFuncPin->PinType = ResultPin->PinType;
	CompilerContext.MovePinLinksToIntermediate(*ResultPin, *OutWidgetFuncPin);

	//BRANCH NODE
	UK2Node_IfThenElse* BranchNode = CompilerContext.SpawnIntermediateNode<UK2Node_IfThenElse>(this, SourceGraph);
	BranchNode->AllocateDefaultPins();

	UEdGraphPin* FunctionOpenSucceedPin = OpenUIFunction->FindPinChecked(TEXT("bIsOpenSucceed"));
	UEdGraphPin* FunctionThenPin = OpenUIFunction->GetThenPin();
	FunctionThenPin->MakeLinkTo(BranchNode->GetExecPin());
	FunctionOpenSucceedPin->MakeLinkTo(BranchNode->GetConditionPin());

	CompilerContext.MovePinLinksToIntermediate(*GetThenPin(), *(BranchNode->GetThenPin()));
	CompilerContext.MovePinLinksToIntermediate(*GetFailedPin(), *(BranchNode->GetElsePin()));
	
	BreakAllNodeLinks();
}

FSlateIcon UK2Node_OpenUI::GetIconAndTint(FLinearColor& OutColor) const
{
	OutColor = GetNodeTitleColor();
	static FSlateIcon Icon("EditorStyle", "Kismet.AllClasses.FunctionIcon");
	return Icon;
}

void UK2Node_OpenUI::PostReconstructNode()
{
	Super::PostReconstructNode();
}

void UK2Node_OpenUI::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	const UClass* ActionKey = GetClass();
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

FText UK2Node_OpenUI::GetMenuCategory() const
{
	return FEditorCategoryUtils::GetCommonCategory(FCommonEditorCategory::Utilities);;
}

void UK2Node_OpenUI::EarlyValidation(FCompilerResultsLog& MessageLog) const
{
	Super::EarlyValidation(MessageLog);
}

void UK2Node_OpenUI::NotifyPinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::NotifyPinConnectionListChanged(Pin);

	RefreshUINameOptions();
	RefreshOutputPinType();
}

UEdGraphPin* UK2Node_OpenUI::GetThenPin() const
{
	UEdGraphPin* Pin = FindPinChecked(UEdGraphSchema_K2::PN_Then);
	check(Pin->Direction == EGPD_Output);
	return Pin;
}

UEdGraphPin* UK2Node_OpenUI::GetWorldContextObjectPin() const
{
	UEdGraphPin* Pin = FindPinChecked(OpenUIHelper::WorldPinName);
	check(Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_OpenUI::GetUINamePin() const
{
	UEdGraphPin* Pin = FindPinChecked(OpenUIHelper::UINamePinName);
	check(Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_OpenUI::GetParamsPin() const
{
	UEdGraphPin* Pin = FindPinChecked(OpenUIHelper::ParamsPinName);
	check(Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_OpenUI::GetFailedPin() const
{
	UEdGraphPin* Pin = FindPinChecked(OpenUIHelper::CreateFailedPinName);
	check(Pin->Direction == EGPD_Output);
	return Pin;
}

UEdGraphPin* UK2Node_OpenUI::GetResultPin() const
{
	UEdGraphPin* Pin = FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
	check(Pin->Direction == EGPD_Output);
	return Pin;
}

void UK2Node_OpenUI::SetDefaultValueUINamePin() const
{
	UEdGraphPin* UINamePin = GetUINamePin();
	UINamePin->DefaultValue = "None";
}

void UK2Node_OpenUI::RefreshUINameOptions() const
{
	SetDefaultValueUINamePin();

	UEdGraph* Graph = GetGraph();
	Graph->NotifyGraphChanged();
}

void UK2Node_OpenUI::RefreshOutputPinType() const
{
	const TObjectPtr<UClass> OldCls = static_cast<UClass*>(GetResultPin()->PinType.PinSubCategoryObject.Get());

	const UEdGraphPin* UINamePin = GetUINamePin();
	if(UINamePin != nullptr && UINamePin->DefaultValue != "None")
	{
		const TObjectPtr<UClass> NewCls = UUIManagerLib::GetUICreateInfoCls(FName(UINamePin->DefaultValue));
		if(OldCls != NewCls)
		{
			UEdGraphPin* ResultPin = GetResultPin();
			ResultPin->PinType.PinSubCategoryObject = NewCls;
		}
	}
}

FText UK2Node_OpenUI::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString("Open UI by DT_UIManager Config");
}

void UK2Node_OpenUI::PinDefaultValueChanged(UEdGraphPin* Pin)
{
	if (Pin != nullptr && Pin->PinName == OpenUIHelper::UINamePinName)
	{
		RefreshOutputPinType();
	}
}

FText UK2Node_OpenUI::GetTooltipText() const
{
	return FText::FromString("");
}



