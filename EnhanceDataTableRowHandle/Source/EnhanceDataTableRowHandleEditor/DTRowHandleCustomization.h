// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"

class ENHANCEDATATABLEROWHANDLEEDITOR_API DTRowHandleCustomization : public IPropertyTypeCustomization
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
	void OnBrowseTo();

protected:
	void OnTextChanged(const FText& Text);
	void OnComboBoxOpened();
private:

	TArray<TSharedPtr<FName>> AllRowNames;
	TSharedPtr<SListView<TSharedPtr<FName>>> ComboContainer;
	FName SelectedPropertyName = FName("None");
	
	TSharedPtr<SComboButton> ComboButton;
	TSharedPtr<IPropertyHandle> NamePropertyHandle;
	TSharedPtr<IPropertyHandle> StructPropertyHandle;
	FOnClicked OnClicked;
};



