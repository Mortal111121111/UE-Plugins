// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"

class ENHANCEDATATABLEROWHANDLE_API DTRowHandleCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();
	virtual void CustomizeHeader( TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils ) override;
	virtual void CustomizeChildren( TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils ) override;

private:
	TArray<TSharedPtr<FName>> GetAllRowNames(const FString& StructName);
	
	FText GetSelectedPropertyName() const;
	TSharedRef<SWidget> GeneratePropertyList();
	void OnChangeProperty(TSharedPtr<FName> ItemSelected, ESelectInfo::Type SelectInfo);
	void OnPropertyNameSelectionChanged(TSharedPtr<FName> Item,ESelectInfo::Type SelectInfo);
	TSharedRef<ITableRow> OnGenerateRowForPropertyName(TSharedPtr<FName> Item,const TSharedRef<STableViewBase>& OwnerTable);

private:
	TArray<TSharedPtr<FName>> AllRowNames;
	FName SelectedPropertyName = FName("None");
	
	TSharedPtr<SComboButton> ComboButton;
	TSharedPtr<IPropertyHandle> NamePropertyHandle;
};



