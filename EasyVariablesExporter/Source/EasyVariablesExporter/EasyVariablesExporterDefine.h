// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InstancedStruct.h"
#include "ScalableFloat.h"
#include "UObject/Object.h"
#include "EasyVariablesExporterDefine.generated.h"

/**
 * 
 */

struct EASYVARIABLESEXPORTER_API FEasyVariablesExporterMetadata
{
	static const FName MD_VariableNeedExport;
};

UENUM(BlueprintType)
enum class EParamType : uint8
{
	None = 0,
	Numeric,
	String,
	Enumerate,
	Boolean,
	Vector,
	Structure,
	Name,
	Text
};

USTRUCT(BlueprintType)
struct EASYVARIABLESEXPORTER_API FEasyExporterEnumParam
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, meta=(Tooltip = "类型"))
	FString ENumType;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta=(Tooltip = "值"))
	FString ENumValue;

	FEasyExporterEnumParam()
	{
		
	}

	FEasyExporterEnumParam(const FString& InType, const FString& InValue)
		: ENumType(InType)
		, ENumValue(InValue)
	{
		
	}

	bool operator==(const FEasyExporterEnumParam& Other) const
	{
		return ENumType == Other.ENumType && ENumValue == Other.ENumValue;
	}
};


USTRUCT(BlueprintType)
struct EASYVARIABLESEXPORTER_API FEasyExporterParam
{
	GENERATED_BODY()

	FEasyExporterParam()
	{
		
	}

	FEasyExporterParam(float InValue)
		:ParamType(EParamType::Numeric)
		,NumericValue(InValue)
	{
		
	}

	FEasyExporterParam(const FScalableFloat& InValue)
		:ParamType(EParamType::Numeric)
		,NumericValue(InValue)
	{
		
	}

	FEasyExporterParam(const FString& InValue)
		:ParamType(EParamType::String)
		,StringValue(InValue)
	{
		
	}

	FEasyExporterParam(const bool& InValue)
		:ParamType(EParamType::Boolean)
		,BooleanValue(InValue)
	{
		
	}

	FEasyExporterParam(const FString& InType, const FString& InValue)
		:ParamType(EParamType::Enumerate)
		,EnumValue(InType,InValue)
	{
		
	}

	FEasyExporterParam(const FVector& InValue)
		:ParamType(EParamType::Vector)
		,VectorValue(InValue)
	{
		
	}
	
	FEasyExporterParam(const FInstancedStruct& InValue)
		:ParamType(EParamType::Structure)
		,StructureValue(InValue)
	{
		
	}
	
	FEasyExporterParam(const FName& InValue)
		:ParamType(EParamType::Name)
		,NameValue(InValue)
	{
		
	}

	FEasyExporterParam(const FText& InValue)
		:ParamType(EParamType::Text)
		,TextValue(InValue)
	{
		
	}
	

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere,meta=(Tooltip = "备注"))
	FString Comment;
#endif

	UPROPERTY()
	EParamType ParamType = EParamType::None;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta=(EditCondition = "ParamType == EParamType::Numeric",EditConditionHides))
	FScalableFloat NumericValue;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta=(EditCondition = "ParamType == EParamType::Boolean",EditConditionHides))
	bool BooleanValue = false;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta=(EditCondition = "ParamType == EParamType::String",EditConditionHides))
	FString StringValue;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta=(EditCondition = "ParamType == EParamType::Enumerate",EditConditionHides))
	FEasyExporterEnumParam EnumValue;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta=(EditCondition = "ParamType == EParamType::Vector",EditConditionHides))
	FVector VectorValue = FVector::ZeroVector;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta=(EditCondition = "ParamType == EParamType::Structure",EditConditionHides,StructTypeConst))
	FInstancedStruct StructureValue;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta=(EditCondition = "ParamType == EParamType::Name",EditConditionHides))
	FName NameValue;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta=(EditCondition = "ParamType == EParamType::Text",EditConditionHides))
	FText TextValue;

	bool Equals(const FEasyExporterParam& Other) const
	{
		if(ParamType != Other.ParamType)
		{
			return false;
		}

		switch (ParamType)
		{
		case EParamType::None :
			return true;
		case EParamType::Boolean :
			return BooleanValue == Other.BooleanValue;
		case EParamType::Enumerate :
			return EnumValue == Other.EnumValue;
		case EParamType::Numeric :
			return NumericValue == Other.NumericValue;
		case EParamType::String :
			return StringValue == Other.StringValue;
		case EParamType::Vector :
			return VectorValue == Other.VectorValue;
		case EParamType::Structure :
			return StructureValue == Other.StructureValue;
		case EParamType::Name :
			return NameValue == Other.NameValue;
		case EParamType::Text :
			return TextValue.EqualTo(Other.TextValue);
		}

		check(0);
		return true;
	}
};

USTRUCT(BlueprintType)
struct EASYVARIABLESEXPORTER_API FEasyExporterParamsMap
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere,meta=(Tooltip = "备注"))
	FString Comment;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UObject> ObjectClass;
#endif

	/* Key: Param Name; Value: Param Value*/
	UPROPERTY(EditAnywhere,BlueprintReadOnly,EditFixedSize,meta=(ReadOnlyKeys,TitleProperty="Comment"))
	TMap<FString,FEasyExporterParam> ParamsMap;

	bool operator==(const FEasyExporterParamsMap& Other) const
	{
#if WITH_EDITORONLY_DATA
		if(Comment != Other.Comment)
		{
			return false;
		}

		if(ObjectClass != Other.ObjectClass)
		{
			return false;
		}
#endif
		TSet<FString> ParamKeys;
		TSet<FString> TempParamKeys;
		ParamsMap.GetKeys(TempParamKeys);
		ParamKeys.Append(TempParamKeys);
		Other.ParamsMap.GetKeys(TempParamKeys);
		ParamKeys.Append(TempParamKeys);
		for(const FString& It : ParamKeys)
		{
			const FEasyExporterParam* SelfValue = ParamsMap.Find(It);
			const FEasyExporterParam* OtherValue = Other.ParamsMap.Find(It);
			if(SelfValue == nullptr || OtherValue == nullptr)
			{
				return false;
			}
			if(!SelfValue->Equals(*OtherValue))
			{
				return false;
			}
		}
		return true;
	}
};
