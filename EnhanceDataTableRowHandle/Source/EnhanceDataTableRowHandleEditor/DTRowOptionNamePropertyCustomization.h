// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"

class FDTRowOptionNamePropertyTypeIdentifier : public IPropertyTypeIdentifier
{
public:
	virtual bool IsPropertyTypeCustomized( const IPropertyHandle& PropertyHandle ) const override;

private:
	FName GetPropertyOptionsMetaDataKey(const IPropertyHandle& PropertyHandle) const;
	
};

class ENHANCEDATATABLEROWHANDLEEDITOR_API DTRowOptionNamePropertyCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();
	virtual void CustomizeHeader( TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils ) override;
	virtual void CustomizeChildren( TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils ) override;

private:
	void InitCustomPropertyHandle(const TSharedRef<IPropertyHandle>& PropertyHandle);
	FSoftObjectPath GetDTPathsByMate(const TSharedRef<IPropertyHandle>& PropertyHandle);
	TArray<TSharedPtr<FName>> GetAllRowNames();
	
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
	FSoftObjectPath DTPath;
	FName MetaDataName;
	
	TArray<TSharedPtr<FName>> AllRowNames;
	TSharedPtr<SListView<TSharedPtr<FName>>> ComboContainer;
	FName SelectedPropertyName = FName("None");
	
	TSharedPtr<SComboButton> ComboButton;
	TSharedPtr<IPropertyHandle> NamePropertyHandle;
	FOnClicked OnClicked;
};



