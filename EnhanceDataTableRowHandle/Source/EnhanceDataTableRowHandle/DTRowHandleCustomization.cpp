// Fill out your copyright notice in the Description page of Project Settings.


#include "DTRowHandleCustomization.h"

#include "DetailWidgetRow.h"
#include "EnhanceDTRowHandleSetting.h"
#include "PropertyHandle.h"
#include "Engine/DataTable.h"

TSharedRef<IPropertyTypeCustomization> DTRowHandleCustomization::MakeInstance()
{
	return MakeShareable(new DTRowHandleCustomization());
}

void DTRowHandleCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
	IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	NamePropertyHandle = PropertyHandle->GetChildHandle(0);

	// 在使用句柄之前，检查句柄是否为空
	check(NamePropertyHandle.IsValid());
	
	
	AllRowNames = GetAllRowNames(*PropertyHandle->GetProperty()->GetCPPType());

	SAssignNew(ComboButton, SComboButton)
		.OnGetMenuContent(this, &DTRowHandleCustomization::GeneratePropertyList)
		.ContentPadding(FMargin(2.0f, 2.0f))
		.ButtonContent()
		[
		   SNew(STextBlock)
		   .Text(this, &DTRowHandleCustomization::GetSelectedPropertyName)
		];
	
	HeaderRow.NameContent()[PropertyHandle->CreatePropertyNameWidget()]
	.ValueContent()
	.HAlign(HAlign_Fill)
	[
		ComboButton.ToSharedRef()
	];
}

void DTRowHandleCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
	IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	
}


TArray<TSharedPtr<FName>> DTRowHandleCustomization::GetAllRowNames(const FString& StructName)
{
	const UEnhanceDTRowHandleSetting* Settings = GetDefault<UEnhanceDTRowHandleSetting>();

	TArray<TSharedPtr<FName>> OutNames;
	for (const FEnhanceDTRowHandleInfo& EnhanceStructPath : Settings->EnhanceStructPaths)
	{
		if(EnhanceStructPath.StructName == StructName)
		{
			const TObjectPtr<UDataTable> DataTable = Cast<UDataTable>(EnhanceStructPath.DataTable.TryLoad());
			if( DataTable != nullptr )
			{
				for (const FName& RowName : DataTable->GetRowNames())
				{
					OutNames.Add(MakeShared<FName>(RowName));
				}
			}
			return OutNames;
		}
	}

	return OutNames;
}

void DTRowHandleCustomization::OnChangeProperty(TSharedPtr<FName> ItemSelected, ESelectInfo::Type SelectInfo)
{
}

TSharedRef<SWidget> DTRowHandleCustomization::GeneratePropertyList()
{
	return SNew(SBox)
		.WidthOverride(280)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.MaxHeight(500)
			[
				SNew(SListView<TSharedPtr<FName>>)
				.Visibility(EVisibility::Visible)
				.ListItemsSource(&AllRowNames)
				.OnGenerateRow(this, &DTRowHandleCustomization::OnGenerateRowForPropertyName)
				.OnSelectionChanged(this, &DTRowHandleCustomization::OnPropertyNameSelectionChanged)
			]
		];
}

TSharedRef<ITableRow> DTRowHandleCustomization::OnGenerateRowForPropertyName(TSharedPtr<FName> Item,const TSharedRef<STableViewBase>& OwnerTable)
{
	TSharedRef<ITableRow> ReturnRow = SNew(STableRow<TSharedPtr<FName>>, OwnerTable)
	   [
		  SNew(STextBlock)
		  .Text(FText::FromName(*Item.Get()))
	   ];

	return ReturnRow;
}

void DTRowHandleCustomization::OnPropertyNameSelectionChanged(TSharedPtr<FName> Item,ESelectInfo::Type SelectInfo)
{
	SelectedPropertyName = *Item.Get();
	NamePropertyHandle->SetValue(SelectedPropertyName);
	ComboButton->SetIsOpen(false);
}

FText DTRowHandleCustomization::GetSelectedPropertyName() const
{
	FName CurrentSelectedValue;
	NamePropertyHandle->GetValue(CurrentSelectedValue);

	return FText::FromName(CurrentSelectedValue);
}
