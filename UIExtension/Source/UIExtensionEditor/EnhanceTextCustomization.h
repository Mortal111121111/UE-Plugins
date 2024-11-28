// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

/**
 * 
 */
class UIEXTENSIONEDITOR_API EnhanceTextCustomization : public IPropertyTypeCustomization
{

public:
    static TSharedRef<IPropertyTypeCustomization> MakeInstance();
    virtual void CustomizeHeader( TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils ) override;
	virtual void CustomizeChildren( TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils ) override;

private:
	FText GetSelectedPropertyName() const;
	void OnEditableTextCommitted(const FText& InText, ETextCommit::Type InCommitType) const;

private:
	TSharedPtr<IPropertyHandle> ValuePropertyHandle;
	FSlateRoundedBoxBrush Style = FSlateRoundedBoxBrush(FLinearColor::Black,10.0);
	
};
