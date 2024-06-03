// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStructCustomization.h"

#include "DetailWidgetRow.h"
#include "MyTestStruct.h"

TSharedRef<IPropertyTypeCustomization> MyStructCustomization::MakeInstance()
{
	return MakeShareable(new MyStructCustomization());
}

void MyStructCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
	IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	const TSharedPtr<IPropertyHandle> NamePropertyHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FMyStruct, TestInt));
	
	// 在使用句柄之前，检查句柄是否为空
	check(NamePropertyHandle.IsValid());

	HeaderRow.NameContent()[PropertyHandle->CreatePropertyNameWidget()]
	.ValueContent()
	.HAlign(HAlign_Fill)
	[
		SNew(SBox)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.AutoWidth()
			.Padding(5.0f, 0.0f)
			[
				SNew(STextBlock)
				.Text(FText::FromString("123"))
			]
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				NamePropertyHandle->CreatePropertyValueWidget()
			]
		]
	];
}

void MyStructCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
	IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	
}
