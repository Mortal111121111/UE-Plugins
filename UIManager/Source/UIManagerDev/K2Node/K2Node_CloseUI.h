// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_CloseUI.generated.h"

/**
 * 
 */
UCLASS()
class UIMANAGERDEV_API UK2Node_CloseUI : public UK2Node
{
    GENERATED_BODY()

private:
    //~ Begin UEdGraphNode Interface.
    virtual void AllocateDefaultPins() override;
    virtual FText GetNodeTitle( ENodeTitleType::Type TitleType ) const override;
    virtual void PinDefaultValueChanged( UEdGraphPin* Pin ) override;
    virtual FText GetTooltipText() const override;
    virtual void ExpandNode( class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph ) override;
    virtual FSlateIcon GetIconAndTint( FLinearColor& OutColor ) const override;
    virtual void PostReconstructNode() override;
    //~ End UEdGraphNode Interface.

    //~ Begin UK2Node Interface
    virtual void GetMenuActions( FBlueprintActionDatabaseRegistrar& ActionRegistrar ) const override;
    virtual FText GetMenuCategory() const override;
    virtual void EarlyValidation( class FCompilerResultsLog& MessageLog ) const override;
    virtual void NotifyPinConnectionListChanged( UEdGraphPin* Pin ) override;
    //~ End UK2Node Interface

private:
    UEdGraphPin* GetThenPin() const;
    UEdGraphPin* GetWorldContextObjectPin() const;
    UEdGraphPin* GetUINamePin() const;

    void SetDefaultValueUINamePin() const;
    void RefreshUINameOptions() const;
};