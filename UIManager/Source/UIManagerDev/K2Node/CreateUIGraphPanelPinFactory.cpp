// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateUIGraphPanelPinFactory.h"

#include "EdGraphSchema_K2.h"
#include "K2Node_CloseUI.h"
#include "K2Node_OpenUI.h"
#include "SGraphPinDataTableRowName.h"
#include "UIManager/Lib/UIManagerLib.h"

TSharedPtr<SGraphPin> FCreateUIGraphPanelPinFactory::CreatePin(UEdGraphPin* InPin) const
{
	if (InPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Name)
	{
		const UObject* Outer = InPin->GetOuter();
		if (Outer->IsA(UK2Node_OpenUI::StaticClass()) || Outer->IsA(UK2Node_CloseUI::StaticClass()))
		{
			return SNew(SGraphPinNameList, InPin, UUIManagerLib::GetAllUINames());
		}
	}
	return nullptr;
}
