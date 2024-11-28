// Fill out your copyright notice in the Description page of Project Settings.


#include "DTRowHandleCustomization.h"

#include "DataTableEditorUtils.h"
#include "DetailWidgetRow.h"
#include "PropertyCustomizationHelpers.h"
#include "EnhanceDataTableRowHandle/EnhanceDTRowHandleSetting.h"
#include "PropertyHandle.h"
#include "Widgets/Input/SSearchBox.h"

TSharedRef<IPropertyTypeCustomization> DTRowHandleCustomization::MakeInstance()
{
	return MakeShareable(new DTRowHandleCustomization());
}

void DTRowHandleCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
	IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	NamePropertyHandle = PropertyHandle->GetChildHandle(0);
	StructPropertyHandle = PropertyHandle;
	// 在使用句柄之前，检查句柄是否为空
	check(NamePropertyHandle.IsValid());
	
	FString StructName = *PropertyHandle->GetProperty()->GetCPPType();
	AllRowNames = GetAllRowNames(StructName);

	SAssignNew(ComboButton, SComboButton)
		.OnComboBoxOpened(this, &DTRowHandleCustomization::OnComboBoxOpened)
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
		SNew(SHorizontalBox)
		.Visibility(EVisibility::SelfHitTestInvisible)
		+ SHorizontalBox::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Fill)
		.VAlign(EVerticalAlignment::VAlign_Fill)
		.FillWidth(1.0)
		.Padding(5.0)
		[
			ComboButton.ToSharedRef()
		]
		+ SHorizontalBox::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Fill)
		.VAlign(EVerticalAlignment::VAlign_Fill)
		.Padding(5.0)
		.AutoWidth()
		[
			PropertyCustomizationHelpers::MakeBrowseButton(FSimpleDelegate::CreateSP(this, &DTRowHandleCustomization::OnBrowseTo))
		]
		
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
			.Visibility(EVisibility::SelfHitTestInvisible)
			+ SVerticalBox::Slot()
			.HAlign(EHorizontalAlignment::HAlign_Fill)
			.VAlign(EVerticalAlignment::VAlign_Fill)
			.AutoHeight()
			.Padding(5.0)
			[
				SNew(SSearchBox)
				.OnTextChanged(this,&DTRowHandleCustomization::OnTextChanged)
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.MaxHeight(500)
			[
				SAssignNew(ComboContainer,SListView<TSharedPtr<FName>>)
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

void DTRowHandleCustomization::OnBrowseTo()
{
	const UEnhanceDTRowHandleSetting* Settings = GetDefault<UEnhanceDTRowHandleSetting>();
	FString StructName = *StructPropertyHandle->GetProperty()->GetCPPType();
	TArray<TSharedPtr<FName>> OutNames;
	for (const FEnhanceDTRowHandleInfo& EnhanceStructPath : Settings->EnhanceStructPaths)
	{
		if(EnhanceStructPath.StructName == StructName)
		{
			const TObjectPtr<UDataTable> DataTable = Cast<UDataTable>(EnhanceStructPath.DataTable.TryLoad());
			if(IsValid(DataTable))
			{
				GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(DataTable);
				FName CurrentSelectedValue;
				NamePropertyHandle->GetValue(CurrentSelectedValue);
				if(DataTable->GetRowMap().Contains(CurrentSelectedValue))
				{
					FDataTableEditorUtils::SelectRow(DataTable,CurrentSelectedValue);
				}
			}
		}
	}
}

void DTRowHandleCustomization::OnTextChanged(const FText& Text)
{
	if(!ComboContainer.IsValid())
	{
		return;
	}
	FString SearchString = Text.ToString();
	if(SearchString.Len() > 0)
	{
		FString StructName = *StructPropertyHandle->GetProperty()->GetCPPType();
		AllRowNames = GetAllRowNames(StructName);
		AllRowNames = AllRowNames.FilterByPredicate([&SearchString](TSharedPtr<FName> Item)
		{
			return Item.Get()->ToString().Find(SearchString) != INDEX_NONE;
		});
	}
	else
	{
		FString StructName = *StructPropertyHandle->GetProperty()->GetCPPType();
		AllRowNames = GetAllRowNames(StructName);
	}
	ComboContainer->RebuildList();
}

void DTRowHandleCustomization::OnComboBoxOpened()
{
	FString StructName = *StructPropertyHandle->GetProperty()->GetCPPType();
	AllRowNames = GetAllRowNames(StructName);
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
