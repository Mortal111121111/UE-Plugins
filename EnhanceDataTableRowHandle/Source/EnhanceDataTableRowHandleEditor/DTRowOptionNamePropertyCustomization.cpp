// Fill out your copyright notice in the Description page of Project Settings.


#include "DTRowOptionNamePropertyCustomization.h"

#include "DataTableEditorUtils.h"
#include "DetailWidgetRow.h"
#include "EnhanceDataTableRowHandleDefine.h"
#include "PropertyCustomizationHelpers.h"
#include "EnhanceDataTableRowHandle/EnhanceDTRowHandleSetting.h"
#include "PropertyHandle.h"
#include "Widgets/Input/SSearchBox.h"


bool FDTRowOptionNamePropertyTypeIdentifier::IsPropertyTypeCustomized(const IPropertyHandle& PropertyHandle) const
{
	FName MetaDataName = GetPropertyOptionsMetaDataKey(PropertyHandle);
	if ( MetaDataName.IsNone() )
	{
		return false;
	}
	
	const FString& MetaDataValue = PropertyHandle.GetMetaDataProperty()->GetMetaData(MetaDataName);
	const UEnhanceDTRowHandleSetting* Setting = GetDefault<UEnhanceDTRowHandleSetting>();
	if (!Setting->DataTablePathLinks.Contains(MetaDataValue))
	{
		return false;
	}

	FSoftObjectPath DTPath = Setting->DataTablePathLinks.FindChecked(MetaDataValue);
	return DTPath.IsValid();
}

FName FDTRowOptionNamePropertyTypeIdentifier::GetPropertyOptionsMetaDataKey(const IPropertyHandle& PropertyHandle) const
{
	FProperty* Property = PropertyHandle.GetProperty();
	const FProperty* OwnerProperty = Property->GetOwnerProperty();
	if (!Property->IsA(FNameProperty::StaticClass()))
	{
		return NAME_None;
	}
	
	if (OwnerProperty->HasMetaData(CustomMacro::OptionDataTableRows))
	{
		return CustomMacro::OptionDataTableRows;
	}

	const FMapProperty* MapProperty = CastField<FMapProperty>(OwnerProperty);
	if (MapProperty)
	{
		if (MapProperty->HasMetaData(CustomMacro::OptionKeyDataTableRows) && MapProperty->GetKeyProperty() == Property)
		{
			return CustomMacro::OptionKeyDataTableRows;
		}

		if (MapProperty->HasMetaData(CustomMacro::OptionValueDataTableRows) && MapProperty->GetValueProperty() == Property)
		{
			return CustomMacro::OptionValueDataTableRows;
		}
	}
	
	return NAME_None;
}

TSharedRef<IPropertyTypeCustomization> DTRowOptionNamePropertyCustomization::MakeInstance()
{
	return MakeShareable(new DTRowOptionNamePropertyCustomization());
}

void DTRowOptionNamePropertyCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
	IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	InitCustomPropertyHandle(PropertyHandle);
	if ( NamePropertyHandle == nullptr )
	{
		return;
	}

	DTPath = GetDTPathsByMate(PropertyHandle);
	AllRowNames = GetAllRowNames();

	SAssignNew(ComboButton, SComboButton)
		.OnComboBoxOpened(this, &DTRowOptionNamePropertyCustomization::OnComboBoxOpened)
		.OnGetMenuContent(this, &DTRowOptionNamePropertyCustomization::GeneratePropertyList)
		.ContentPadding(FMargin(2.0f, 2.0f))
		.ButtonContent()
		[
		   SNew(STextBlock)
		   .Text(this, &DTRowOptionNamePropertyCustomization::GetSelectedPropertyName)
		];
	
	HeaderRow.NameContent()[NamePropertyHandle->CreatePropertyNameWidget()]
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
			PropertyCustomizationHelpers::MakeBrowseButton(FSimpleDelegate::CreateSP(this, &DTRowOptionNamePropertyCustomization::OnBrowseTo))
		]
		
	];
}

void DTRowOptionNamePropertyCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
	IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	
}

void DTRowOptionNamePropertyCustomization::InitCustomPropertyHandle(const TSharedRef<IPropertyHandle>& PropertyHandle)
{
	FProperty* Property = PropertyHandle->GetProperty();
	const FProperty* OwnerProperty = Property->GetOwnerProperty();
	if (!Property->IsA(FNameProperty::StaticClass()))
	{
		return;
	}

	if (OwnerProperty->HasMetaData(CustomMacro::OptionDataTableRows))
	{
		NamePropertyHandle = PropertyHandle;
		MetaDataName = CustomMacro::OptionDataTableRows;
	}

	const FMapProperty* MapProperty = CastField<FMapProperty>(OwnerProperty);
	if (MapProperty)
	{
		if (MapProperty->HasMetaData(CustomMacro::OptionKeyDataTableRows) && MapProperty->GetKeyProperty() == Property )
		{
			NamePropertyHandle = PropertyHandle->GetParentHandle();
			MetaDataName = CustomMacro::OptionKeyDataTableRows;
		}

		if (MapProperty->HasMetaData(CustomMacro::OptionValueDataTableRows) && MapProperty->GetValueProperty() == Property)
		{
			NamePropertyHandle = PropertyHandle;
			MetaDataName = CustomMacro::OptionValueDataTableRows;
		}
	}
}

FSoftObjectPath DTRowOptionNamePropertyCustomization::GetDTPathsByMate(const TSharedRef<IPropertyHandle>& PropertyHandle)
{
	const FProperty* MetaDataProperty = PropertyHandle->GetMetaDataProperty();
	const FString& MetaDataValue = MetaDataProperty->GetMetaData(MetaDataName);
	
	const UEnhanceDTRowHandleSetting* Settings = GetDefault<UEnhanceDTRowHandleSetting>();
	return Settings->DataTablePathLinks.FindChecked(MetaDataValue);
}


TArray<TSharedPtr<FName>> DTRowOptionNamePropertyCustomization::GetAllRowNames()
{
	TArray<TSharedPtr<FName>> OutNames;
	const TObjectPtr<UDataTable> DataTable = Cast<UDataTable>(DTPath.TryLoad());
	if( DataTable == nullptr )
	{
		return OutNames;
	}
	
	for (const FName& RowName : DataTable->GetRowNames())
	{
		OutNames.Add(MakeShared<FName>(RowName));
	}
	
	return OutNames;
}

void DTRowOptionNamePropertyCustomization::OnChangeProperty(TSharedPtr<FName> ItemSelected, ESelectInfo::Type SelectInfo)
{
}

TSharedRef<SWidget> DTRowOptionNamePropertyCustomization::GeneratePropertyList()
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
				.OnTextChanged(this,&DTRowOptionNamePropertyCustomization::OnTextChanged)
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.MaxHeight(500)
			[
				SAssignNew(ComboContainer,SListView<TSharedPtr<FName>>)
				.Visibility(EVisibility::Visible)
				.ListItemsSource(&AllRowNames)
				.OnGenerateRow(this, &DTRowOptionNamePropertyCustomization::OnGenerateRowForPropertyName)
				.OnSelectionChanged(this, &DTRowOptionNamePropertyCustomization::OnPropertyNameSelectionChanged)
			]
		];
}

TSharedRef<ITableRow> DTRowOptionNamePropertyCustomization::OnGenerateRowForPropertyName(TSharedPtr<FName> Item,const TSharedRef<STableViewBase>& OwnerTable)
{
	TSharedRef<ITableRow> ReturnRow = SNew(STableRow<TSharedPtr<FName>>, OwnerTable)
	   [
		  SNew(STextBlock)
		  .Text(FText::FromName(*Item.Get()))
	   ];

	return ReturnRow;
}

void DTRowOptionNamePropertyCustomization::OnBrowseTo()
{
	const TObjectPtr<UDataTable> DataTable = Cast<UDataTable>(DTPath.TryLoad());
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

void DTRowOptionNamePropertyCustomization::OnTextChanged(const FText& Text)
{
	if(!ComboContainer.IsValid())
	{
		return;
	}
	FString SearchString = Text.ToString();
	if(SearchString.Len() > 0)
	{
		AllRowNames = GetAllRowNames();
		AllRowNames = AllRowNames.FilterByPredicate([&SearchString](TSharedPtr<FName> Item)
		{
			return Item.Get()->ToString().Find(SearchString) != INDEX_NONE;
		});
	}
	else
	{
		AllRowNames = GetAllRowNames();
	}
	ComboContainer->RebuildList();
}

void DTRowOptionNamePropertyCustomization::OnComboBoxOpened()
{
	AllRowNames = GetAllRowNames();
}

void DTRowOptionNamePropertyCustomization::OnPropertyNameSelectionChanged(TSharedPtr<FName> Item,ESelectInfo::Type SelectInfo)
{
	SelectedPropertyName = *Item.Get();
	NamePropertyHandle->SetValue(SelectedPropertyName);
	ComboButton->SetIsOpen(false);
}

FText DTRowOptionNamePropertyCustomization::GetSelectedPropertyName() const
{
	FName CurrentSelectedValue;
	NamePropertyHandle->GetValue(CurrentSelectedValue);

	return FText::FromName(CurrentSelectedValue);
}
