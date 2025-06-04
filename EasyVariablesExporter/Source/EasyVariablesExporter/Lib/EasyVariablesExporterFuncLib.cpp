// Fill out your copyright notice in the Description page of Project Settings.


#include "EasyVariablesExporterFuncLib.h"

#include "EasyVariablesExporter/EasyVariablesExporter.h"

bool UEasyVariablesExporterFuncLib::ExportParamFromObject(UObject* Object, FEasyExporterParamsMap& OutParamsMap)
{
#if WITH_EDITOR
	if(Object == nullptr || !Object->GetClass()->HasAnyFlags(RF_WasLoaded) || Object->GetClass()->HasAnyFlags(RF_Transient))
	{
		return false;
	}
	UClass* ObjectClass = Object->GetClass();
	if(ObjectClass->GetName().StartsWith("SKEL_"))
	{
		UE_LOG(LogTemp,Warning,TEXT("Export Params of SKEL_ Object : %s"),*(ObjectClass->GetName()))
		return false;
	}
	if(ObjectClass->GetName().StartsWith("REINST_"))
	{
		UE_LOG(LogTemp,Warning,TEXT("Export Params of REINST_ Object : %s"),*(ObjectClass->GetName()))
		return false;
	}
	
	OutParamsMap.ObjectClass = ObjectClass;
	FString ClassDesc = ObjectClass->GetMetaData("ToolTip");
	OutParamsMap.Comment = ClassDesc;
	for(TFieldIterator<FProperty> It(ObjectClass,EFieldIteratorFlags::IncludeSuper); It ; ++It)
	{
		bool bNeedExport = ShouldPropertyExport(Object, *It);
		if(!bNeedExport)
		{
			continue;
		}
		
		FString PropertyName = It->GetName();
		FString Desc = It->GetMetaData("ToolTip");
		FEasyExporterParam Param;
		
#define TRY_EXPORT_PARAM(ParamType) \
		if(Export##ParamType##Property(Object,*It,Param))\
		{\
			Param.Comment = Desc;\
			OutParamsMap.ParamsMap.Add(PropertyName,Param);\
			continue;\
		}
		
		TRY_EXPORT_PARAM(Numeric)
		TRY_EXPORT_PARAM(String)
		TRY_EXPORT_PARAM(Enumerate)
		TRY_EXPORT_PARAM(Boolean)
		TRY_EXPORT_PARAM(Vector)
		TRY_EXPORT_PARAM(Name)
		TRY_EXPORT_PARAM(Text)
		TRY_EXPORT_PARAM(Structure)
	}
	
	return true;
#endif
	
	return false;
}

bool UEasyVariablesExporterFuncLib::ImportParamToObject(const FEasyExporterParamsMap& ParamsMap)
{
#if WITH_EDITOR
	if(!IsValid(ParamsMap.ObjectClass))
	{
		return false;
	}
	UObject* Object = ParamsMap.ObjectClass.GetDefaultObject();
	if(Object == nullptr || !Object->HasAnyFlags(RF_ClassDefaultObject) || !Object->GetClass()->HasAnyFlags(RF_WasLoaded) || Object->GetClass()->HasAnyFlags(RF_Transient))
	{
		return false;
	}
	
	if(ParamsMap.ParamsMap.IsEmpty())
	{
		return true;
	}
	
	int32 ParamHasImport = 0;
	for(TFieldIterator<FProperty> It(Object->GetClass()); It ; ++It)
	{
		if(ParamHasImport >= ParamsMap.ParamsMap.Num())
		{
			break;
		}
		FString PropertyName = It->GetName();
		if(!ParamsMap.ParamsMap.Contains(PropertyName))
		{
			continue;
		}
		
		++ParamHasImport;
		const FEasyExporterParam& Param = ParamsMap.ParamsMap.FindRef(PropertyName);
		
#define TRY_IMPORT_PARAM(ParamType) \
	if(Import##ParamType##Property(Object,*It,Param))\
	{\
		continue;\
	}
		TRY_IMPORT_PARAM(Numeric)
		TRY_IMPORT_PARAM(String)
		TRY_IMPORT_PARAM(Enumerate)
		TRY_IMPORT_PARAM(Boolean)
		TRY_IMPORT_PARAM(Vector)
		TRY_IMPORT_PARAM(Name)
		TRY_IMPORT_PARAM(Text)
		TRY_IMPORT_PARAM(Structure)
	}
	return true;
#endif

	return false;
}

UObject* UEasyVariablesExporterFuncLib::GetClassDefaultObject(TSubclassOf<UObject> InClass)
{
	if(IsValid(InClass))
	{
		return InClass.GetDefaultObject();
	}
	return nullptr;
}

bool UEasyVariablesExporterFuncLib::ShouldPropertyExport(const UObject* Owner, const FProperty* Property)
{
	
#if WITH_EDITOR
	if(Property == nullptr)
	{
		return false;
	}
	bool bShouldExport = Property->HasMetaData(FEasyVariablesExporterMetadata::MD_VariableNeedExport);
	FEasyVariablesExporterModule& EasyVariablesExporterModule = FModuleManager::LoadModuleChecked<FEasyVariablesExporterModule>("EasyVariablesExporter");
	const TMap<FString, FCustomNeedExportFunc>& CustomNeedExportMap = EasyVariablesExporterModule.GetCustomNeedExportMap();
	for(const auto& It : CustomNeedExportMap)
	{
		if(bShouldExport)
		{
			break;
		}
		bShouldExport |= It.Value(Owner, Property);
	}
	return bShouldExport;
#endif
	
	return false;
}

bool UEasyVariablesExporterFuncLib::ExportEnumerateProperty(UObject* Object, const FProperty* Property, FEasyExporterParam& OutParam)
{
	if (const FEnumProperty* EnumProperty = CastField<FEnumProperty>(Property))
	{
		FNumericProperty* EnumNumericProperty = EnumProperty->GetUnderlyingProperty();
		const void* ValuePtr = EnumProperty->ContainerPtrToValuePtr<void>(Object);
		int64 NumericEnumValue = EnumNumericProperty->GetSignedIntPropertyValue(ValuePtr);
		FString EnumType = EnumProperty->GetEnum()->GetFullName();
		FString EnumValue = EnumProperty->GetEnum()->GetNameByValue(NumericEnumValue).ToString();
		OutParam = FEasyExporterParam(EnumType,EnumValue);
		return true;
	}

	if(const FByteProperty* ByteProperty = CastField<FByteProperty>(Property))
	{
		if(IsValid(ByteProperty->GetIntPropertyEnum()))
		{
			const void* ValuePtr = ByteProperty->ContainerPtrToValuePtr<void>(Object);
			int64 NumericEnumValue = ByteProperty->GetSignedIntPropertyValue(ValuePtr);
			FString EnumType = ByteProperty->GetIntPropertyEnum()->GetFullName();
			FString EnumValue = ByteProperty->GetIntPropertyEnum()->GetNameByValue(NumericEnumValue).ToString();
			OutParam = FEasyExporterParam(EnumType,EnumValue);
			return true;
		}
	}
	return false;
}

bool UEasyVariablesExporterFuncLib::ExportBooleanProperty(UObject* Object, const FProperty* Property, FEasyExporterParam& OutParam)
{
	if (const FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property))
	{
		const void* ValuePtr = BoolProperty->ContainerPtrToValuePtr<void>(Object);
		bool BoolValue = BoolProperty->GetPropertyValue(ValuePtr);
		OutParam = FEasyExporterParam(BoolValue);
		return true;
	}
	return false;
}

bool UEasyVariablesExporterFuncLib::ExportNumericProperty(UObject* Object, const FProperty* Property, FEasyExporterParam& OutParam)
{
	if (const FNumericProperty* NumericProperty = CastField<FNumericProperty>(Property))
	{
		if(IsValid(NumericProperty->GetIntPropertyEnum()))
		{
			return false;
		}
		const void* ValuePtr = NumericProperty->ContainerPtrToValuePtr<void>(Object);
		float FloatValue = 0;
		if(NumericProperty->IsFloatingPoint())
		{
			FloatValue = NumericProperty->GetFloatingPointPropertyValue(ValuePtr);
		}
		else if(NumericProperty->IsInteger())
		{
			FloatValue = (float)NumericProperty->GetSignedIntPropertyValue(ValuePtr);
		}
		OutParam = FEasyExporterParam(FloatValue);
		return true;
	}
	return false;
}

bool UEasyVariablesExporterFuncLib::ExportStructureProperty(UObject* Object, const FProperty* Property, FEasyExporterParam& OutParam)
{
	if(const FStructProperty* StructProperty = CastField<FStructProperty>(Property))
	{
		uint8* ValuePtr = StructProperty->ContainerPtrToValuePtr<uint8>(Object);
		FInstancedStruct TempStructure;
		TempStructure.InitializeAs(StructProperty->Struct,ValuePtr);
		OutParam = FEasyExporterParam(TempStructure);
		return true;
	}
	return false;
}

bool UEasyVariablesExporterFuncLib::ExportVectorProperty(UObject* Object, const FProperty* Property, FEasyExporterParam& OutParam)
{
	if(const FStructProperty* StructProperty = CastField<FStructProperty>(Property))
	{
		if(StructProperty->Struct->GetName() == "Vector")
		{
			FVector* VectorParam = StructProperty->ContainerPtrToValuePtr<FVector>(Object);
			OutParam = FEasyExporterParam(*VectorParam);
			return true;
		}
	}
	return false;
}

bool UEasyVariablesExporterFuncLib::ExportStringProperty(UObject* Object, const FProperty* Property, FEasyExporterParam& OutParam)
{
	if (const FStrProperty* StrProperty = CastField<FStrProperty>(Property))
	{
		const void* ValuePtr = StrProperty->ContainerPtrToValuePtr<void>(Object);
		FString StringValue = StrProperty->GetPropertyValue(ValuePtr);
		OutParam = FEasyExporterParam(StringValue);
		return true;
	}
	return false;
}

bool UEasyVariablesExporterFuncLib::ExportNameProperty(UObject* Object, const FProperty* Property,
	FEasyExporterParam& OutParam)
{
	if (const FNameProperty* NameProperty = CastField<FNameProperty>(Property))
	{
		const FName* ValuePtr = NameProperty->ContainerPtrToValuePtr<FName>(Object);
		FName NameValue = NameProperty->GetPropertyValue(ValuePtr);
		OutParam = FEasyExporterParam(NameValue);
		return true;
	}
	return false;
}

bool UEasyVariablesExporterFuncLib::ExportTextProperty(UObject* Object, const FProperty* Property,
	FEasyExporterParam& OutParam)
{
	if (const FTextProperty* TextProperty = CastField<FTextProperty>(Property))
	{
		const FText* ValuePtr = TextProperty->ContainerPtrToValuePtr<FText>(Object);
		FText NameValue = TextProperty->GetPropertyValue(ValuePtr);
		OutParam = FEasyExporterParam(NameValue);
		return true;
	}
	return false;
}

bool UEasyVariablesExporterFuncLib::ImportEnumerateProperty(UObject* Object, const FProperty* Property,
                                                            const FEasyExporterParam& Param)
{
	if (const FEnumProperty* EnumProperty = CastField<FEnumProperty>(Property))
	{
		FNumericProperty* EnumNumericProperty = EnumProperty->GetUnderlyingProperty();
		const void* ValuePtr = EnumProperty->ContainerPtrToValuePtr<void>(Object);
		const int64 NumericEnumValue = EnumNumericProperty->GetSignedIntPropertyValue(ValuePtr);
		FString EnumType = EnumProperty->GetEnum()->GetFullName();
		FString EnumValue = EnumProperty->GetEnum()->GetNameByValue(NumericEnumValue).ToString();
		if(Param.ParamType != EParamType::Enumerate || Param.EnumValue.ENumType != EnumType)
		{
			ensureAlwaysMsgf(0,TEXT("Param Type Not Match!"));
			return false;
		}
		if(EnumValue != Param.EnumValue.ENumValue)
		{
			Object->Modify();
			int64 ParamEnumValue = EnumProperty->GetEnum()->GetValueByNameString(Param.EnumValue.ENumValue);
			EnumProperty->SetValue_InContainer(Object,&ParamEnumValue);
		}
		return true;
	}
	if(const FByteProperty* ByteProperty = CastField<FByteProperty>(Property))
	{
		if(IsValid(ByteProperty->GetIntPropertyEnum()))
		{
			void* ValuePtr = ByteProperty->ContainerPtrToValuePtr<void>(Object);
			int64 NumericEnumValue = ByteProperty->GetSignedIntPropertyValue(ValuePtr);
			FString EnumType = ByteProperty->GetIntPropertyEnum()->GetFullName();
			FString EnumValue = ByteProperty->GetIntPropertyEnum()->GetNameByValue(NumericEnumValue).ToString();
			if(Param.ParamType != EParamType::Enumerate || Param.EnumValue.ENumType != EnumType)
			{
				ensureAlwaysMsgf(0,TEXT("Param Type Not Match!"));
				return false;
			}
			if(EnumValue != Param.EnumValue.ENumValue)
			{
				Object->Modify();
				int64 ParamEnumValue = ByteProperty->GetIntPropertyEnum()->GetValueByNameString(Param.EnumValue.ENumValue);
				ByteProperty->SetIntPropertyValue(ValuePtr,ParamEnumValue);
			}
			return true;
		}
	}
	return false;
}

bool UEasyVariablesExporterFuncLib::ImportBooleanProperty(UObject* Object, const FProperty* Property,
	const FEasyExporterParam& Param)
{
	if (const FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property))
	{
		if(Param.ParamType != EParamType::Boolean)
		{
			ensureAlwaysMsgf(0,TEXT("Param Type Not Match!"));
			return false;
		}
		bool* ValuePtr = BoolProperty->ContainerPtrToValuePtr<bool>(Object);
		if(*ValuePtr != Param.BooleanValue)
		{
			Object->Modify();
			*ValuePtr = Param.BooleanValue;
		}
		return true;
	}
	return false;
}

bool UEasyVariablesExporterFuncLib::ImportNumericProperty(UObject* Object, const FProperty* Property,
	const FEasyExporterParam& Param)
{
	if (const FNumericProperty* NumericProperty = CastField<FNumericProperty>(Property))
	{
		if(IsValid(NumericProperty->GetIntPropertyEnum()))
		{
			return false;
		}
		if(Param.ParamType != EParamType::Numeric)
		{
			ensureAlwaysMsgf(0,TEXT("Param Type Not Match!"));
			return false;
		}
		float ParamValue = Param.NumericValue.Value;
		void* DataResult = NumericProperty->ContainerPtrToValuePtr<void>(Object);
		float FloatValue = 0;
		if(NumericProperty->IsFloatingPoint())
		{
			FloatValue = NumericProperty->GetFloatingPointPropertyValue(DataResult);
		}
		else if(NumericProperty->IsInteger())
		{
			FloatValue = (float)NumericProperty->GetSignedIntPropertyValue(DataResult);
		}
		if(FloatValue != ParamValue)
		{
			Object->Modify();
			if(NumericProperty->IsInteger())
			{
				NumericProperty->SetIntPropertyValue(DataResult,FMath::TruncToInt64(ParamValue));
			}
			else if(NumericProperty->IsFloatingPoint())
			{
				NumericProperty->SetFloatingPointPropertyValue(DataResult,ParamValue);
			}
		}
		return true;
	}
	return false;
}

bool UEasyVariablesExporterFuncLib::ImportStructureProperty(UObject* Object, const FProperty* Property,
	const FEasyExporterParam& Param)
{
	if(const FStructProperty* StructProperty = CastField<FStructProperty>(Property))
	{
		if(Param.ParamType != EParamType::Structure || StructProperty->Struct != Param.StructureValue.GetScriptStruct())
		{
			ensureAlwaysMsgf(0,TEXT("Param Type Not Match!"));
			return false;
		}
		uint8* ValuePtr = Property->ContainerPtrToValuePtr<uint8>(Object);
		if(Param.StructureValue.GetScriptStruct()->CompareScriptStruct(ValuePtr,Param.StructureValue.GetMemory(),0) == false)
		{
			Object->Modify();
			Param.StructureValue.GetScriptStruct()->CopyScriptStruct(ValuePtr,Param.StructureValue.GetMemory());
		}
		return true;
	}
	return false;
}

bool UEasyVariablesExporterFuncLib::ImportVectorProperty(UObject* Object, const FProperty* Property,
	const FEasyExporterParam& Param)
{
	if(const FStructProperty* StructProperty = CastField<FStructProperty>(Property))
	{
		if(StructProperty->Struct->GetName() == "Vector")
		{
			if(Param.ParamType != EParamType::Vector)
			{
				ensureAlwaysMsgf(0,TEXT("Param Type Not Match!"));
				return false;
			}
			FVector* ValuePtr = Property->ContainerPtrToValuePtr<FVector>(Object);
			if(*ValuePtr != Param.VectorValue)
			{
				Object->Modify();
				*ValuePtr = Param.VectorValue;
			}
			return true;
		}
	}

	return false;
}

bool UEasyVariablesExporterFuncLib::ImportStringProperty(UObject* Object, const FProperty* Property,
	const FEasyExporterParam& Param)
{
	if (const FStrProperty* StrProperty = CastField<FStrProperty>(Property))
	{
		if(Param.ParamType != EParamType::String)
		{
			ensureAlwaysMsgf(0,TEXT("Param Type Not Match!"));
			return false;
		}
		FString* ValuePtr = StrProperty->ContainerPtrToValuePtr<FString>(Object);
		if(*ValuePtr != Param.StringValue)
		{
			Object->Modify();
			*ValuePtr = Param.StringValue;
		}
		return true;
	}
	return false;
}

bool UEasyVariablesExporterFuncLib::ImportNameProperty(UObject* Object, const FProperty* Property,
	const FEasyExporterParam& Param)
{
	if (const FNameProperty* NameProperty = CastField<FNameProperty>(Property))
	{
		if(Param.ParamType != EParamType::Name)
		{
			ensureAlwaysMsgf(0,TEXT("Param Type Not Match!"));
			return false;
		}
		FName* ValuePtr = NameProperty->ContainerPtrToValuePtr<FName>(Object);
		if(*ValuePtr != Param.NameValue)
		{
			Object->Modify();
			*ValuePtr = Param.NameValue;
		}
		return true;
	}
	return false;
}

bool UEasyVariablesExporterFuncLib::ImportTextProperty(UObject* Object, const FProperty* Property,
	const FEasyExporterParam& Param)
{
	if (const FTextProperty* TextProperty = CastField<FTextProperty>(Property))
	{
		if(Param.ParamType != EParamType::Text)
		{
			ensureAlwaysMsgf(0,TEXT("Param Type Not Match!"));
			return false;
		}
		FText* ValuePtr = TextProperty->ContainerPtrToValuePtr<FText>(Object);
		if(!ValuePtr->EqualTo(Param.TextValue))
		{
			Object->Modify();
			*ValuePtr = Param.TextValue;
		}
		return true;
	}
	return false;
}
