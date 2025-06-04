#include "NeedExportVariableDetailCustomization.h"

#include "Kismet2/BlueprintEditorUtils.h"
#include "DetailLayoutBuilder.h"
#include "BlueprintEditorModule.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "K2Node_Variable.h"
#include "EasyVariablesExporter/EasyVariablesExporterDefine.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SCheckBox.h"
#include "Engine/BlueprintGeneratedClass.h"

#define LOCTEXT_NAMESPACE "NeedExportVariableDetailCustomization"

TSharedPtr<IDetailCustomization> NeedExportVariableDetailCustomization::MakeInstance(TSharedPtr<IBlueprintEditor> InBlueprintEditor)
{
	const TArray<UObject*>* Objects = (InBlueprintEditor.IsValid() ? InBlueprintEditor->GetObjectsCurrentlyBeingEdited() : nullptr);
	if (Objects && Objects->Num() == 1)
	{
		if (UBlueprint* Blueprint = Cast<UBlueprint>((*Objects)[0]))
		{
			return MakeShareable(new NeedExportVariableDetailCustomization(InBlueprintEditor, Blueprint));
		}
	}
	return nullptr;
}

UK2Node_Variable* NeedExportVariableDetailCustomization::EdGraphSelectionAsVar() const
{
	TWeakPtr<FBlueprintEditor> BlueprintEditor = MyBlueprint.Pin()->GetBlueprintEditor();

	if( BlueprintEditor.IsValid() )
	{
		/** Get the currently selected set of nodes */
		FGraphPanelSelectionSet Objects = BlueprintEditor.Pin()->GetSelectedNodes();

		if (Objects.Num() == 1)
		{
			FGraphPanelSelectionSet::TIterator Iter(Objects);
			UObject* Object = *Iter;

			if (Object && Object->IsA<UK2Node_Variable>())
			{
				return Cast<UK2Node_Variable>(Object);
			}
		}
	}
	return nullptr;
}

bool NeedExportVariableDetailCustomization::IsALocalVariable(FProperty* VariableProperty) const
{
	return VariableProperty && (VariableProperty->GetOwner<UFunction>() != NULL);
}

bool NeedExportVariableDetailCustomization::IsAUserVariable(FProperty* VariableProperty) const
{
	FObjectProperty* VariableObjProp = VariableProperty ? CastField<FObjectProperty>(VariableProperty) : NULL;

	if (VariableObjProp != NULL && VariableObjProp->PropertyClass != NULL)
	{
		return FBlueprintEditorUtils::IsVariableCreatedByBlueprint(GetBlueprintObj(), VariableObjProp);
	}
	return true;
}

bool NeedExportVariableDetailCustomization::IsABlueprintVariable(FProperty* VariableProperty) const
{
	UClass* VarSourceClass = VariableProperty ? VariableProperty->GetOwner<UClass>() : NULL;
	if(VarSourceClass)
	{
		return (VarSourceClass->ClassGeneratedBy != NULL);
	}
	return false;
}

FProperty* NeedExportVariableDetailCustomization::SelectionAsProperty() const
{
	if (FEdGraphSchemaAction_BlueprintVariableBase* BPVar = MyBlueprint.Pin()->SelectionAsBlueprintVariable())
	{
		return BPVar->GetProperty();
	}
	else if (UK2Node_Variable* GraphVar = EdGraphSelectionAsVar())
	{
		return GraphVar->GetPropertyForVariable();
	}

	return nullptr;
}

FName NeedExportVariableDetailCustomization::GetVariableName() const
{
	if (FEdGraphSchemaAction_BlueprintVariableBase* BPVar = MyBlueprint.Pin()->SelectionAsBlueprintVariable())
	{
		return BPVar->GetVariableName();
	}
	else if (UK2Node_Variable* GraphVar = EdGraphSelectionAsVar())
	{
		return GraphVar->GetVarName();
	}

	return NAME_None;
}

void NeedExportVariableDetailCustomization::OnPostEditorRefresh()
{
	CachedVariableProperty = SelectionAsProperty();
	CachedVariableName = GetVariableName();
}

UBlueprint* NeedExportVariableDetailCustomization::GetPropertyOwnerBlueprint() const
{
	FProperty* VariableProperty = CachedVariableProperty.Get();

	// Cache the Blueprint which owns this VariableProperty
	if (UBlueprintGeneratedClass* GeneratedClass = Cast<UBlueprintGeneratedClass>(VariableProperty->GetOwnerClass()))
	{
		return Cast<UBlueprint>(GeneratedClass->ClassGeneratedBy);
	}
	return nullptr;
}

EVisibility NeedExportVariableDetailCustomization::NeedExportVisibility() const
{
	FProperty* VariableProperty = CachedVariableProperty.Get();

	// Cache the Blueprint which owns this VariableProperty
	if (VariableProperty && GetPropertyOwnerBlueprint())
	{
		const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();
		FEdGraphPinType VariablePinType;
		K2Schema->ConvertPropertyToPinType(VariableProperty, VariablePinType);

		const bool bShowPrivacySetting = IsABlueprintVariable(VariableProperty) && IsAUserVariable(VariableProperty) && !IsALocalVariable(VariableProperty);
		FObjectPropertyBase* ObjectProperty  = CastField<FObjectPropertyBase>(VariableProperty);
		if (bShowPrivacySetting && (K2Schema->FindSetVariableByNameFunction(VariablePinType) != NULL) && ObjectProperty == nullptr)
		{
			return EVisibility::Visible;
		}
	}
	return EVisibility::Collapsed;
}

ECheckBoxState NeedExportVariableDetailCustomization::OnGetNeedExportCheckboxState() const
{
	FProperty* Property = CachedVariableProperty.Get();
	if (Property)
	{
		return (Property && Property->GetBoolMetaData(FEasyVariablesExporterMetadata::MD_VariableNeedExport) != false) ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
	}
	return ECheckBoxState::Unchecked;
}

void NeedExportVariableDetailCustomization::OnNeedExportChanged(ECheckBoxState InNewState)
{
	const FName VarName = CachedVariableName;
	if (VarName != NAME_None)
	{
		const bool bNeedExport = (InNewState == ECheckBoxState::Checked);
		if(bNeedExport)
		{
			FBlueprintEditorUtils::SetBlueprintVariableMetaData(GetBlueprintObj(), VarName, NULL, FEasyVariablesExporterMetadata::MD_VariableNeedExport, TEXT("true"));
		}
		else
		{
			FBlueprintEditorUtils::RemoveBlueprintVariableMetaData(GetBlueprintObj(), VarName, NULL, FEasyVariablesExporterMetadata::MD_VariableNeedExport);
		} 
	}
}

void NeedExportVariableDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	CachedVariableProperty = SelectionAsProperty();

	if(!CachedVariableProperty.IsValid())
	{
		return;
	}

	CachedVariableName = GetVariableName();

	TWeakPtr<FBlueprintEditor> BlueprintEditor = MyBlueprint.Pin()->GetBlueprintEditor();
	if( BlueprintEditor.IsValid() )
	{
		BlueprintEditor.Pin()->OnRefresh().AddSP(this, &NeedExportVariableDetailCustomization::OnPostEditorRefresh);
	}
	
	IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("Easy Variables Exporter Config",FText::GetEmpty(),ECategoryPriority::Variable);
	const FSlateFontInfo DetailFontInfo = IDetailLayoutBuilder::GetDetailFont();

	Category.AddCustomRow( LOCTEXT("VariableNeedToExportLabel", "Need Export") )
	.Visibility(TAttribute<EVisibility>(this, &NeedExportVariableDetailCustomization::NeedExportVisibility))
	.NameContent()
	[
		SNew(STextBlock)
		.Text( LOCTEXT("VariableNeedToExportLabel", "Need Export") )
		.Font( DetailFontInfo )
	]
	.ValueContent()
	[
		SNew(SCheckBox)
		.IsChecked( this, &NeedExportVariableDetailCustomization::OnGetNeedExportCheckboxState )
		.OnCheckStateChanged( this, &NeedExportVariableDetailCustomization::OnNeedExportChanged )
	];
}

#undef LOCTEXT_NAMESPACE