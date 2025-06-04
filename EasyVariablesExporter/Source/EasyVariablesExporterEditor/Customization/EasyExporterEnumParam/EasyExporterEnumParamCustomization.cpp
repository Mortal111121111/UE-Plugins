#include "EasyExporterEnumParamCustomization.h"

#include "DetailWidgetRow.h"
#include "PropertyHandle.h"

TSharedRef<IPropertyTypeCustomization> EasyExporterEnumParamCustomization::MakeInstance()
{
	return MakeShareable(new EasyExporterEnumParamCustomization());
}

void EasyExporterEnumParamCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle,
	FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	StructPropertyHandle = PropertyHandle;
	EnumTypePropertyHandle = PropertyHandle->GetChildHandle(0);
	ENumValuePropertyHandle = PropertyHandle->GetChildHandle(1);
	
	// 在使用句柄之前，检查句柄是否为空
	check(StructPropertyHandle.IsValid());
	
	AllRowNames = GetAllRowNames();

	SAssignNew(ComboButton, SComboButton)
		.OnComboBoxOpened(this, &EasyExporterEnumParamCustomization::OnComboBoxOpened)
		.OnGetMenuContent(this, &EasyExporterEnumParamCustomization::GeneratePropertyList)
		.ContentPadding(FMargin(1.0f, 1.0f))
		.ButtonContent()
		[
		   SNew(STextBlock)
		   .Text(this, &EasyExporterEnumParamCustomization::GetSelectedPropertyName)
		   .ToolTipText(this,&EasyExporterEnumParamCustomization::GetSelectedPropertyToolTip)
		];
	
	HeaderRow.NameContent()[PropertyHandle->CreatePropertyNameWidget()]
	.ValueContent()
	.HAlign(HAlign_Fill)
	[
		SNew(SVerticalBox)
		.Visibility(EVisibility::SelfHitTestInvisible)
		+ SVerticalBox::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Fill)
		.VAlign(EVerticalAlignment::VAlign_Fill)
		.Padding(5.0)
		[
			SNew(STextBlock)
			.Text_Raw(this,&EasyExporterEnumParamCustomization::GetEnumTypeText)
		]
		+ SVerticalBox::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Left)
		.VAlign(EVerticalAlignment::VAlign_Fill)
		.AutoHeight()
		.Padding(2.0)
		[
			ComboButton.ToSharedRef()
		]
	];
}

void EasyExporterEnumParamCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
	IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
}

TArray<TSharedPtr<FString>> EasyExporterEnumParamCustomization::GetAllRowNames()
{
	TArray<TSharedPtr<FString>> OutNames;
	FString EnumTypeValue;
	EnumTypePropertyHandle->GetValue(EnumTypeValue);
	EnumTypeValue = EnumTypeValue.Mid(5);
	UEnum* Enum = FindObject<UEnum>(nullptr, *EnumTypeValue);
	if(Enum != nullptr)
	{
		for(int32 Index = 0; Index < Enum->NumEnums() - 1; ++Index)
		{
			OutNames.Add(MakeShared<FString>(Enum->GetNameByIndex(Index).ToString()));
		}
	}
	return OutNames;
}

FText EasyExporterEnumParamCustomization::GetSelectedPropertyName() const
{
	FString CurrentSelectedValue;
	ENumValuePropertyHandle->GetValue(CurrentSelectedValue);

	return FText::FromString(CurrentSelectedValue);
}

FText EasyExporterEnumParamCustomization::GetSelectedPropertyToolTip() const
{
	FString CurrentSelectedValue;
	ENumValuePropertyHandle->GetValue(CurrentSelectedValue);
	FString EnumTypeValue;
	EnumTypePropertyHandle->GetValue(EnumTypeValue);
	// TODO: 因为导出的格式为 UEnum /script.xxxxx,所以这里需要手动把 'UEnum ' 这5个字符去掉
	EnumTypeValue = EnumTypeValue.Mid(5);
	UEnum* Enum = FindObject<UEnum>(nullptr, *EnumTypeValue);
	return Enum ? FText::FromString(Enum->GetMetaData(TEXT("ToolTip"),Enum->GetIndexByNameString(CurrentSelectedValue))) : FText();
}

TSharedRef<SWidget> EasyExporterEnumParamCustomization::GeneratePropertyList()
{
	return SNew(SBox)
		.WidthOverride(280)
		[
			SNew(SVerticalBox)
			.Visibility(EVisibility::SelfHitTestInvisible)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.MaxHeight(500)
			[
				SAssignNew(ComboContainer,SListView<TSharedPtr<FString>>)
				.Visibility(EVisibility::Visible)
				.ListItemsSource(&AllRowNames)
				.OnGenerateRow(this, &EasyExporterEnumParamCustomization::OnGenerateRowForPropertyName)
				.OnSelectionChanged(this, &EasyExporterEnumParamCustomization::OnPropertyNameSelectionChanged)
			]
		];
}

void EasyExporterEnumParamCustomization::OnChangeProperty(TSharedPtr<FString> ItemSelected, ESelectInfo::Type SelectInfo)
{
}

void EasyExporterEnumParamCustomization::OnPropertyNameSelectionChanged(TSharedPtr<FString> Item,
	ESelectInfo::Type SelectInfo)
{
	SelectedPropertyName = *Item.Get();
	ENumValuePropertyHandle->SetValue(SelectedPropertyName);
	ComboButton->SetIsOpen(false);
}

TSharedRef<ITableRow> EasyExporterEnumParamCustomization::OnGenerateRowForPropertyName(TSharedPtr<FString> Item,
	const TSharedRef<STableViewBase>& OwnerTable)
{
	FString EnumTypeValue;
	EnumTypePropertyHandle->GetValue(EnumTypeValue);
	EnumTypeValue = EnumTypeValue.Mid(5);
	UEnum* Enum = FindObject<UEnum>(nullptr, *EnumTypeValue);
	
	TSharedRef<ITableRow> ReturnRow = SNew(STableRow<TSharedPtr<FString>>, OwnerTable)
	   [
		  SNew(STextBlock)
		  .Text(FText::FromString(*Item.Get()))
		  .MinDesiredWidth(200)
		  .ToolTipText(Enum ? FText::FromString(Enum->GetMetaData(TEXT("ToolTip"),Enum->GetIndexByNameString(*Item))) : FText())
	   ];

	return ReturnRow;
}

void EasyExporterEnumParamCustomization::OnComboBoxOpened()
{
	AllRowNames = GetAllRowNames();
}

FText EasyExporterEnumParamCustomization::GetEnumTypeText() const
{
	FString TypeValue;
	EnumTypePropertyHandle->GetValue(TypeValue);

	return FText::FromString(TypeValue);
}
