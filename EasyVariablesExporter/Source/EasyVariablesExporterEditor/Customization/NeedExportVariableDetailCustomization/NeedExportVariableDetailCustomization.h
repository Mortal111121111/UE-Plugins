// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Blueprint.h"
#include "IDetailCustomization.h"
#include "SMyBlueprint.h"
#include "Styling/SlateTypes.h"
#include "UObject/WeakFieldPtr.h"

class IDetailLayoutBuilder;
class IBlueprintEditor;
class UBlueprint;

class EASYVARIABLESEXPORTEREDITOR_API NeedExportVariableDetailCustomization  : public IDetailCustomization
{

public:
	NeedExportVariableDetailCustomization()
	{
		
	}

	NeedExportVariableDetailCustomization(TSharedPtr<IBlueprintEditor> InBlueprintEditor, UBlueprint* Blueprint)
		: BlueprintEditorPtr(InBlueprintEditor)
		, Blueprint(Blueprint)
	{
		if(FBlueprintEditor* BlueprintEditor = static_cast<FBlueprintEditor*>(InBlueprintEditor.Get()))
		{
			MyBlueprint = BlueprintEditor->GetMyBlueprintWidget();
		}
	}

	virtual ~NeedExportVariableDetailCustomization() override
	{
		if(MyBlueprint.IsValid())
		{
			// Remove the callback delegate we registered for
			TWeakPtr<FBlueprintEditor> BlueprintEditor = MyBlueprint.Pin()->GetBlueprintEditor();
			if( BlueprintEditor.IsValid() )
			{
				BlueprintEditor.Pin()->OnRefresh().RemoveAll(this);
			}
		}
	}
	
public:
	static TSharedPtr<IDetailCustomization> MakeInstance(TSharedPtr<IBlueprintEditor> InBlueprintEditor);
	// static TSharedRef<class IDetailCustomization> MakeInstance(TWeakPtr<SMyBlueprint> InMyBlueprint);
	
public:
	UK2Node_Variable* EdGraphSelectionAsVar() const;
	bool IsALocalVariable(FProperty* VariableProperty) const;
	bool IsAUserVariable(FProperty* VariableProperty) const;
	bool IsABlueprintVariable(FProperty* VariableProperty) const;
	FProperty* SelectionAsProperty() const;
	FName GetVariableName() const;
	void OnPostEditorRefresh();
	UBlueprint* GetPropertyOwnerBlueprint() const;
	EVisibility NeedExportVisibility() const;
	ECheckBoxState OnGetNeedExportCheckboxState() const;
	void OnNeedExportChanged(ECheckBoxState InNewState);

public:
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

protected:
	UBlueprint* GetBlueprintObj() const { return Blueprint.Get(); }

protected:

	/** The Blueprint editor we are embedded in */
	TWeakPtr<IBlueprintEditor> BlueprintEditorPtr;

	
	TWeakObjectPtr<UBlueprint> Blueprint;
	
	/** Cached property for the variable we are affecting */
	TWeakFieldPtr<FProperty> CachedVariableProperty;

	/** Cached name for the variable we are affecting */
	FName CachedVariableName;

	TWeakPtr<SMyBlueprint> MyBlueprint;
};
