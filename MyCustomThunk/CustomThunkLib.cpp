// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomThunkLib.h"


void UCustomThunkLib::GetObjectPropertyByName(UObject* Target, FString PropertyName, const int& OutValue)
{
	check(0);
} 

void UCustomThunkLib::Generic_GetObjectPropertyByName(UObject* Target, FString PropertyName, void* Value)
{
	// 类型信息
	FProperty* FoundProp = FindFieldChecked<FProperty>(Target->GetClass(),FName(PropertyName));
	// 拿到具体值
	void* PropertyValuePtr = FoundProp->ContainerPtrToValuePtr<void>(Target);
	// 根据类型信息进行赋值
	// FoundProp->CopyCompleteValue_InContainer(PropertyValuePtr,Value);
	FoundProp->CopySingleValue(Value,PropertyValuePtr);
	UE_LOG(LogTemp,Error,TEXT("111111111111111111111,%s %s"),*Target->GetName(),*PropertyName)
}

TArray<FSaveGamePropertyInfo> UCustomThunkLib::GetObjectSaveGamePropertyInfo(UObject* Object)
{
	TArray<FSaveGamePropertyInfo> Out;
	const TObjectPtr<UClass> Cls = Object->GetClass();
	for (TFieldIterator<FProperty> PropIt(Cls); PropIt; ++PropIt)
	{
		FProperty* Property = *PropIt;
		FString Category = Property->GetMetaData("Category");
		if (Category == "SaveGame" || Category == "Save Game")
		{
			FSaveGamePropertyInfo Info = FSaveGamePropertyInfo();
			Info.PropertyName = Property->GetName();
			if (const FIntProperty* IntProperty = CastField<FIntProperty>(Property))
			{
				Info.IntValue = IntProperty->GetPropertyValue_InContainer(Object);
			}
			else if (const FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property))
			{
				Info.BoolValue = BoolProperty->GetPropertyValue_InContainer(Object);
			}
			else if (const FFloatProperty* FloatProperty = CastField<FFloatProperty>(Property))
			{
				Info.FloatValue = FloatProperty->GetPropertyValue_InContainer(Object);
			}
			else if (const FDoubleProperty* DoubleProperty = CastField<FDoubleProperty>(Property))
			{
				Info.FloatValue = DoubleProperty->GetPropertyValue_InContainer(Object);
			}
			Out.Add(Info);
		}
	}
	return Out;
}

bool UCustomThunkLib::SetObjectPropertyByName(UObject* Object, const FSaveGamePropertyInfo& Info)
{
	const TObjectPtr<UClass> Cls = Object->GetClass();
	FProperty* Property = FindFieldChecked<FProperty>(Cls, FName(Info.PropertyName));
	if( Property == nullptr )
	{
		return false;
	}

	if (const FIntProperty* IntProperty = CastField<FIntProperty>(Property))
	{
		IntProperty->SetPropertyValue_InContainer(Object,Info.IntValue);
	}
	else if (const FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property))
	{
		BoolProperty->SetPropertyValue_InContainer(Object,Info.BoolValue);
	}
	else if (const FFloatProperty* FloatProperty = CastField<FFloatProperty>(Property))
	{
		FloatProperty->SetPropertyValue_InContainer(Object,Info.FloatValue);
	}
	else if (const FDoubleProperty* DoubleProperty = CastField<FDoubleProperty>(Property))
	{
		DoubleProperty->SetPropertyValue_InContainer(Object,Info.FloatValue);
	}
	else
	{
		return false;
	}

	return true;
}

