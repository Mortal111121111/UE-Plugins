// Fill out your copyright notice in the Description page of Project Settings.


#include "EnhanceTextCustomization.h"

#include "DetailWidgetRow.h"
#include "UIExtension/UIExtensionSetting.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Text/SMultiLineEditableText.h"

TSharedRef<IPropertyTypeCustomization> EnhanceTextCustomization::MakeInstance()
{
	return MakeShareable(new EnhanceTextCustomization());
}

void EnhanceTextCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
                                               IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	ValuePropertyHandle = PropertyHandle->GetChildHandle(0);
	// 在使用句柄之前，检查句柄是否为空
	check(ValuePropertyHandle.IsValid());

	const UUIExtensionSetting* Settings = GetDefault<UUIExtensionSetting>();
	const float Height = Settings->EnhanceTextHeight;
	FSlateFontInfo Font = FCoreStyle::GetDefaultFontStyle("Bold", Settings->EnhanceFontSize);
	FTextBlockStyle TextBlockStyle = FTextBlockStyle::GetDefault();
	TextBlockStyle.SetColorAndOpacity(FLinearColor::White);
	Style.OutlineSettings = FSlateBrushOutlineSettings(5.0,FLinearColor::Gray,2);
	
	HeaderRow.NameContent()[PropertyHandle->CreatePropertyNameWidget()]
	.ValueContent()
	.HAlign(HAlign_Fill)
	[
		SNew(SBorder)
		.BorderImage( &Style )
		[
			SNew(SBox)
			.Padding(5.0,5.0)
			.MinDesiredHeight(Height)  
			.HAlign(HAlign_Fill)  
			.VAlign(VAlign_Fill)  
			[
				SNew(SMultiLineEditableText)
				.OnTextCommitted(this, &EnhanceTextCustomization::OnEditableTextCommitted)
				.AutoWrapText(true)
				.WrappingPolicy(ETextWrappingPolicy::AllowPerCharacterWrapping)
				.Font(Font)
				.TextStyle(&TextBlockStyle)
				.Text(this, &EnhanceTextCustomization::GetSelectedPropertyName)
			]
		]
	];
}

void EnhanceTextCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
	IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	
}

FText EnhanceTextCustomization::GetSelectedPropertyName() const
{
	FText CurrentSelectedValue;
	ValuePropertyHandle->GetValue(CurrentSelectedValue);

	return CurrentSelectedValue;
}

void EnhanceTextCustomization::OnEditableTextCommitted(const FText& InText, ETextCommit::Type InCommitType) const
{
	ValuePropertyHandle->SetValue(InText);
}
