// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

/**
 * 将 OptionDataTableRow 宏暴露在蓝图中进行编辑
 */
class ENHANCEDATATABLEROWHANDLEEDITOR_API FDTRowOptionNamePropertyDetailCustomization : public IDetailCustomization
{

public:
	FDTRowOptionNamePropertyDetailCustomization(){};
	FDTRowOptionNamePropertyDetailCustomization(TSharedPtr<IBlueprintEditor> InBlueprintEditor, UBlueprint* Blueprint);
	
	static TSharedPtr<IDetailCustomization> MakeInstance(TSharedPtr<IBlueprintEditor> InBlueprintEditor);
	virtual void CustomizeDetails( IDetailLayoutBuilder& DetailBuilder ) override;

private:
	void OnEditableTextCommitted(const FText& InText, ETextCommit::Type InCommitType) const;
	FText GetNamePropertyMateDataValue() const;
	bool CheckMacroValueIsValid(const FString& InValue) const;
	
private:
	TSharedPtr<IPropertyHandle> NamePropertyHandle;
	TWeakFieldPtr<FProperty> NameProperty;
	TWeakObjectPtr<UBlueprint> MyBlueprint;
};
