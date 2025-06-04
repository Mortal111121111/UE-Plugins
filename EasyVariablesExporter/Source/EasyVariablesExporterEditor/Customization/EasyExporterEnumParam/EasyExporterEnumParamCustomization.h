// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"


class EasyExporterEnumParamCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();
	virtual void CustomizeHeader( TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils ) override;
	virtual void CustomizeChildren( TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils ) override;

private:
	TArray<TSharedPtr<FString>> GetAllRowNames();
	
	FText GetSelectedPropertyName() const;
	FText GetSelectedPropertyToolTip() const;
	TSharedRef<SWidget> GeneratePropertyList();
	void OnChangeProperty(TSharedPtr<FString> ItemSelected, ESelectInfo::Type SelectInfo);
	void OnPropertyNameSelectionChanged(TSharedPtr<FString> Item,ESelectInfo::Type SelectInfo);
	TSharedRef<ITableRow> OnGenerateRowForPropertyName(TSharedPtr<FString> Item,const TSharedRef<STableViewBase>& OwnerTable);
	
	

protected:
	void OnComboBoxOpened();
	FText GetEnumTypeText()const;
	
private:

	TArray<TSharedPtr<FString>> AllRowNames;
	TSharedPtr<SListView<TSharedPtr<FString>>> ComboContainer;
	FString SelectedPropertyName;
	
	TSharedPtr<SComboButton> ComboButton;
	TSharedPtr<IPropertyHandle> EnumTypePropertyHandle;
	TSharedPtr<IPropertyHandle> ENumValuePropertyHandle;
	TSharedPtr<IPropertyHandle> StructPropertyHandle;
	FOnClicked OnClicked;
};