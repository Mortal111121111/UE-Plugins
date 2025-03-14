// Fill out your copyright notice in the Description page of Project Settings.


#include "DTRowOptionNamePropertyDetailCustomization.h"

#include "BlueprintEditorModule.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "EnhanceDataTableRowHandleDefine.h"
#include "EnhanceDataTableRowHandle/EnhanceDTRowHandleSetting.h"
#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "EnhanceDataTableRowHandle"

FDTRowOptionNamePropertyDetailCustomization::FDTRowOptionNamePropertyDetailCustomization(TSharedPtr<IBlueprintEditor> InBlueprintEditor, UBlueprint* Blueprint)
{
	MyBlueprint = Blueprint;
}

TSharedPtr<IDetailCustomization> FDTRowOptionNamePropertyDetailCustomization::MakeInstance(TSharedPtr<IBlueprintEditor> InBlueprintEditor)
{
	const TArray<UObject*>* Objects = (InBlueprintEditor.IsValid() ? InBlueprintEditor->GetObjectsCurrentlyBeingEdited() : nullptr);
	if (Objects && Objects->Num() == 1)
	{
		if (UBlueprint* Blueprint = Cast<UBlueprint>((*Objects)[0]))
		{
			return MakeShareable(new FDTRowOptionNamePropertyDetailCustomization(InBlueprintEditor, Blueprint));
		}
	}

	return nullptr;
}

void FDTRowOptionNamePropertyDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("OptionDataTableRow",FText::GetEmpty(),ECategoryPriority::Variable);
	const FSlateFontInfo DetailFontInfo = IDetailLayoutBuilder::GetDetailFont();

	TArray<TWeakObjectPtr<UObject>> CustomizedObjects;
	DetailBuilder.GetObjectsBeingCustomized(CustomizedObjects);

	UPropertyWrapper* PropertyWrapper = Cast<UPropertyWrapper>(CustomizedObjects[0].Get());
	NameProperty = PropertyWrapper ? PropertyWrapper->GetProperty() : nullptr;

	if ( NameProperty == nullptr )
	{
		return;
	}
	
	FTextBlockStyle TextBlockStyle = FTextBlockStyle::GetDefault();
	
	Category.AddCustomRow( LOCTEXT("EnhanceDataTableRowHandle", "OptionDataTableRow") )
	.NameContent()
	[
		SNew(STextBlock)
		.Text( LOCTEXT("EnhanceDataTableRowHandle", "OptionDataTableRow") )
		.Font( DetailFontInfo )
	]
	.ValueContent()
	[
		SNew(SEditableTextBox)
		.OnTextCommitted(this, &FDTRowOptionNamePropertyDetailCustomization::OnEditableTextCommitted)
		.Font(DetailFontInfo)
		.Text(this, &FDTRowOptionNamePropertyDetailCustomization::GetNamePropertyMateDataValue)
	];
}

void FDTRowOptionNamePropertyDetailCustomization::OnEditableTextCommitted(const FText& InText,ETextCommit::Type InCommitType) const
{
	// FString NewValue = InText.ToString();
	// if (NewValue.IsEmpty() || !CheckMacroValueIsValid(NewValue))
	// {
	// 	NameProperty->RemoveMetaData(CustomMacro::NAME_OptionDataTableRows);
	// 	return;
	// }
	//
	// NameProperty->SetMetaData(CustomMacro::NAME_OptionDataTableRows,*NewValue);

	FString VarName = NameProperty->GetName();
	if (VarName.IsEmpty() || InText.IsEmpty() || !CheckMacroValueIsValid(InText.ToString()))
	{
		FBlueprintEditorUtils::RemoveBlueprintVariableMetaData(MyBlueprint.Get(), FName(VarName), nullptr, CustomMacro::OptionDataTableRows);
		return;
	}
	
	FBlueprintEditorUtils::SetBlueprintVariableMetaData(MyBlueprint.Get(), FName(VarName), nullptr, CustomMacro::OptionDataTableRows, *InText.ToString());
	
}

FText FDTRowOptionNamePropertyDetailCustomization::GetNamePropertyMateDataValue() const
{
	if ( !NameProperty->HasMetaData(CustomMacro::OptionDataTableRows) )
	{
		return FText();
	}
	
	const FString& MetaDataValue = NameProperty->GetMetaData(CustomMacro::OptionDataTableRows);
	return FText::FromString( MetaDataValue );
}

bool FDTRowOptionNamePropertyDetailCustomization::CheckMacroValueIsValid(const FString& InValue) const
{
	const UEnhanceDTRowHandleSetting* Setting = GetDefault<UEnhanceDTRowHandleSetting>();
	return Setting->DataTablePathLinks.Contains(InValue);
}

#undef LOCTEXT_NAMESPACE
