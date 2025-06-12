#include "JsonUtilities.h"

#include "JsonObjectConverter.h"

bool UJsonUtilities::JsonAttributesToUStructWithContainer(const TMap< FString, TSharedPtr<FJsonValue> >& JsonAttributes, const UStruct* StructDefinition, void* OutStruct, const UStruct* ContainerStruct, void* Container, int64 CheckFlags, int64 SkipFlags, const bool bStrictMode)
{
	if (StructDefinition == FJsonObjectWrapper::StaticStruct())
	{
		// Just copy it into the object
		FJsonObjectWrapper* ProxyObject = (FJsonObjectWrapper*)OutStruct;
		ProxyObject->JsonObject = MakeShared<FJsonObject>();
		ProxyObject->JsonObject->Values = JsonAttributes;
		return true;
	}

	int32 NumUnclaimedProperties = JsonAttributes.Num();
	if (NumUnclaimedProperties <= 0)
	{
		return true;
	}

	// iterate over the struct properties
	for (TFieldIterator<FProperty> PropIt(StructDefinition); PropIt; ++PropIt)
	{
		FProperty* Property = *PropIt;

		// Check to see if we should ignore this property
		if (CheckFlags != 0 && !Property->HasAnyPropertyFlags(CheckFlags))
		{
			continue;
		}
		if (Property->HasAnyPropertyFlags(SkipFlags))
		{
			continue;
		}

		// find a JSON value matching this property name
		FString PropertyName = StructDefinition->GetAuthoredNameForField(Property);
		const TSharedPtr<FJsonValue>* JsonValue = JsonAttributes.Find(PropertyName);
			
		if (!JsonValue)
		{
			if (bStrictMode)
			{
				return false;
			}
				
			// we allow values to not be found since this mirrors the typical UObject mantra that all the fields are optional when deserializing
			continue;
		}

		if (JsonValue->IsValid() && !(*JsonValue)->IsNull())
		{
			void* Value = Property->ContainerPtrToValuePtr<uint8>(OutStruct);
			if (!JsonValueToFPropertyWithContainer(*JsonValue, Property, Value, ContainerStruct, Container, CheckFlags, SkipFlags, bStrictMode))
			{
				return false;
			}
		}

		if (--NumUnclaimedProperties <= 0)
		{
			// Should we log a warning/error if we still have properties in the JSON data that aren't in the struct definition in strict mode?
				
			// If we found all properties that were in the JsonAttributes map, there is no reason to keep looking for more.
			break;
		}
	}

	return true;
}

bool UJsonUtilities::JsonValueToFPropertyWithContainer(const TSharedPtr<FJsonValue>& JsonValue, FProperty* Property, void* OutValue, const UStruct* ContainerStruct, void* Container, int64 CheckFlags, int64 SkipFlags, const bool bStrictMode)
{
	if (!JsonValue.IsValid())
	{
		UE_LOG(LogJson, Error, TEXT("JsonValueToUProperty - Invalid JSON value"));
		return false;
	}

	const bool bArrayOrSetProperty = Property->IsA<FArrayProperty>() || Property->IsA<FSetProperty>();
	const bool bJsonArray = JsonValue->Type == EJson::Array;

	if (!bJsonArray)
	{
		if (bArrayOrSetProperty)
		{
			UE_LOG(LogJson, Error, TEXT("JsonValueToUProperty - Expecting JSON array"));
			return false;
		}

		if (Property->ArrayDim != 1)
		{
			if (bStrictMode)
			{
				UE_LOG(LogJson, Error, TEXT("JsonValueToUProperty - Property %s is not an array but has %d elements"), *Property->GetAuthoredName(), Property->ArrayDim);
				return false;
			}
				
			UE_LOG(LogJson, Warning, TEXT("Ignoring excess properties when deserializing %s"), *Property->GetAuthoredName());
		}

		return ConvertScalarJsonValueToFPropertyWithContainer(JsonValue, Property, OutValue, ContainerStruct, Container, CheckFlags, SkipFlags, bStrictMode);
	}

	// In practice, the ArrayDim == 1 check ought to be redundant, since nested arrays of FProperties are not supported
	if (bArrayOrSetProperty && Property->ArrayDim == 1)
	{
		// Read into TArray
		return ConvertScalarJsonValueToFPropertyWithContainer(JsonValue, Property, OutValue, ContainerStruct, Container, CheckFlags, SkipFlags, bStrictMode);
	}

	// We're deserializing a JSON array
	const auto& ArrayValue = JsonValue->AsArray();

	if (bStrictMode && (Property->ArrayDim != ArrayValue.Num()))
	{
		UE_LOG(LogJson, Error, TEXT("JsonValueToUProperty - JSON array size is incorrect (has %d elements, but needs %d)"), ArrayValue.Num(), Property->ArrayDim);
		return false;
	}
		
	if (Property->ArrayDim < ArrayValue.Num())
	{
		UE_LOG(LogJson, Warning, TEXT("Ignoring excess properties when deserializing %s"), *Property->GetAuthoredName());
	}

	// Read into native array
	const int32 ItemsToRead = FMath::Clamp(ArrayValue.Num(), 0, Property->ArrayDim);
	for (int Index = 0; Index != ItemsToRead; ++Index)
	{
		if (!ConvertScalarJsonValueToFPropertyWithContainer(ArrayValue[Index], Property, static_cast<char*>(OutValue) + Index * Property->ElementSize, ContainerStruct, Container, CheckFlags, SkipFlags, bStrictMode))
		{
			return false;
		}
	}
	return true;
}

bool UJsonUtilities::ConvertScalarJsonValueToFPropertyWithContainer(const TSharedPtr<FJsonValue>& JsonValue, FProperty* Property, void* OutValue, const UStruct* ContainerStruct, void* Container, int64 CheckFlags, int64 SkipFlags, const bool bStrictMode)
{
	if (FEnumProperty* EnumProperty = CastField<FEnumProperty>(Property))
	{
		if (JsonValue->Type == EJson::String)
		{
			// see if we were passed a string for the enum
			const UEnum* Enum = EnumProperty->GetEnum();
			check(Enum);
			FString StrValue = JsonValue->AsString();
			int64 IntValue = Enum->GetValueByName(FName(*StrValue), EGetByNameFlags::CheckAuthoredName);
			if (IntValue == INDEX_NONE)
			{
				UE_LOG(LogJson, Error, TEXT("JsonValueToUProperty - Unable to import enum %s from string value %s for property %s"), *Enum->CppType, *StrValue, *Property->GetAuthoredName());
				
				return false;
			}
			EnumProperty->GetUnderlyingProperty()->SetIntPropertyValue(OutValue, IntValue);
		}
		else
		{
			// AsNumber will log an error for completely inappropriate types (then give us a default)
			EnumProperty->GetUnderlyingProperty()->SetIntPropertyValue(OutValue, (int64)JsonValue->AsNumber());
		}
	}
	else if (FNumericProperty *NumericProperty = CastField<FNumericProperty>(Property))
	{
		if (NumericProperty->IsEnum() && JsonValue->Type == EJson::String)
		{
			// see if we were passed a string for the enum
			const UEnum* Enum = NumericProperty->GetIntPropertyEnum();
			check(Enum); // should be assured by IsEnum()
			FString StrValue = JsonValue->AsString();
			int64 IntValue = Enum->GetValueByName(FName(*StrValue), EGetByNameFlags::CheckAuthoredName);
			if (IntValue == INDEX_NONE)
			{
				UE_LOG(LogJson, Error, TEXT("JsonValueToUProperty - Unable to import enum %s from numeric value %s for property %s"), *Enum->CppType, *StrValue, *Property->GetAuthoredName());
				
				return false;
			}
			NumericProperty->SetIntPropertyValue(OutValue, IntValue);
		}
		else if (NumericProperty->IsFloatingPoint())
		{
			// AsNumber will log an error for completely inappropriate types (then give us a default)
			NumericProperty->SetFloatingPointPropertyValue(OutValue, JsonValue->AsNumber());
		}
		else if (NumericProperty->IsInteger())
		{
			if (JsonValue->Type == EJson::String)
			{
				// parse string -> int64 ourselves so we don't lose any precision going through AsNumber (aka double)
				NumericProperty->SetIntPropertyValue(OutValue, FCString::Atoi64(*JsonValue->AsString()));
			}
			else
			{
				// AsNumber will log an error for completely inappropriate types (then give us a default)
				NumericProperty->SetIntPropertyValue(OutValue, (int64)JsonValue->AsNumber());
			}
		}
		else
		{
			UE_LOG(LogJson, Error, TEXT("JsonValueToUProperty - Unable to import json value into %s numeric property %s"), *Property->GetClass()->GetName(), *Property->GetAuthoredName());
			
			return false;
		}
	}
	else if (FBoolProperty *BoolProperty = CastField<FBoolProperty>(Property))
	{
		// AsBool will log an error for completely inappropriate types (then give us a default)
		BoolProperty->SetPropertyValue(OutValue, JsonValue->AsBool());
	}
	else if (FStrProperty *StringProperty = CastField<FStrProperty>(Property))
	{
		// AsString will log an error for completely inappropriate types (then give us a default)
		StringProperty->SetPropertyValue(OutValue, JsonValue->AsString());
	}
	else if (FArrayProperty *ArrayProperty = CastField<FArrayProperty>(Property))
	{
		if (JsonValue->Type == EJson::Array)
		{
			TArray< TSharedPtr<FJsonValue> > ArrayValue = JsonValue->AsArray();
			int32 ArrLen = ArrayValue.Num();

			// make the output array size match
			FScriptArrayHelper Helper(ArrayProperty, OutValue);
			Helper.Resize(ArrLen);

			// set the property values
			for (int32 i = 0; i < ArrLen; ++i)
			{
				const TSharedPtr<FJsonValue>& ArrayValueItem = ArrayValue[i];
				if (ArrayValueItem.IsValid() && !ArrayValueItem->IsNull())
				{
					if (!JsonValueToFPropertyWithContainer(ArrayValueItem, ArrayProperty->Inner, Helper.GetRawPtr(i), ContainerStruct, Container, CheckFlags & (~CPF_ParmFlags), SkipFlags, bStrictMode))
					{
						UE_LOG(LogJson, Error, TEXT("JsonValueToUProperty - Unable to import Array element %d for property %s"), i, *Property->GetAuthoredName());
						
						return false;
					}
				}
			}
		}
		else
		{
			UE_LOG(LogJson, Error, TEXT("JsonValueToUProperty - Unable to import non-array JSON value into Array property %s"), *Property->GetAuthoredName());
			
			return false;
		}
	}
	else if (FMapProperty* MapProperty = CastField<FMapProperty>(Property))
	{
		if (JsonValue->Type == EJson::Object)
		{
			TSharedPtr<FJsonObject> ObjectValue = JsonValue->AsObject();

			FScriptMapHelper Helper(MapProperty, OutValue);

			check(ObjectValue);

			int32 MapSize = ObjectValue->Values.Num();
			Helper.EmptyValues(MapSize);

			// set the property values
			for (const auto& Entry : ObjectValue->Values)
			{
				if (Entry.Value.IsValid() && !Entry.Value->IsNull())
				{
					int32 NewIndex = Helper.AddDefaultValue_Invalid_NeedsRehash();

					TSharedPtr<FJsonValueString> TempKeyValue = MakeShared<FJsonValueString>(Entry.Key);

					if (!JsonValueToFPropertyWithContainer(TempKeyValue, MapProperty->KeyProp, Helper.GetKeyPtr(NewIndex), ContainerStruct, Container, CheckFlags & (~CPF_ParmFlags), SkipFlags, bStrictMode))
					{
						UE_LOG(LogJson, Error, TEXT("JsonValueToUProperty - Unable to import Map element %s key for property %s"), *Entry.Key, *Property->GetAuthoredName());
						
						return false;
					}

					if (!JsonValueToFPropertyWithContainer(Entry.Value, MapProperty->ValueProp, Helper.GetValuePtr(NewIndex), ContainerStruct, Container, CheckFlags & (~CPF_ParmFlags), SkipFlags, bStrictMode))
					{
						UE_LOG(LogJson, Error, TEXT("JsonValueToUProperty - Unable to import Map element %s value for property %s"), *Entry.Key, *Property->GetAuthoredName());
						
						return false;
					}
				}
			}

			Helper.Rehash();
		}
		else
		{
			UE_LOG(LogJson, Error, TEXT("JsonValueToUProperty - Unable to import non-object JSON value into Map property %s"), *Property->GetAuthoredName());
			return false;
		}
	}
	else if (FSetProperty* SetProperty = CastField<FSetProperty>(Property))
	{
		if (JsonValue->Type == EJson::Array)
		{
			TArray< TSharedPtr<FJsonValue> > ArrayValue = JsonValue->AsArray();
			int32 ArrLen = ArrayValue.Num();

			FScriptSetHelper Helper(SetProperty, OutValue);
			Helper.EmptyElements(ArrLen);

			// set the property values
			for (int32 i = 0; i < ArrLen; ++i)
			{
				const TSharedPtr<FJsonValue>& ArrayValueItem = ArrayValue[i];
				if (ArrayValueItem.IsValid() && !ArrayValueItem->IsNull())
				{
					int32 NewIndex = Helper.AddDefaultValue_Invalid_NeedsRehash();
					if (!JsonValueToFPropertyWithContainer(ArrayValueItem, SetProperty->ElementProp, Helper.GetElementPtr(NewIndex), ContainerStruct, Container, CheckFlags & (~CPF_ParmFlags), SkipFlags, bStrictMode))
					{
						UE_LOG(LogJson, Error, TEXT("JsonValueToUProperty - Unable to import Set element %d for property %s"), i, *Property->GetAuthoredName());
						return false;
					}
				}
			}

			Helper.Rehash();
		}
		else
		{
			UE_LOG(LogJson, Error, TEXT("JsonValueToUProperty - Unable to import non-array JSON value into Set property %s"), *Property->GetAuthoredName());
			return false;
		}
	}
	else if (FTextProperty* TextProperty = CastField<FTextProperty>(Property))
	{
		if (JsonValue->Type == EJson::String)
		{
			// assume this string is already localized, so import as invariant
			TextProperty->SetPropertyValue(OutValue, FText::FromString(JsonValue->AsString()));
		}
		else if (JsonValue->Type == EJson::Object)
		{
			TSharedPtr<FJsonObject> Obj = JsonValue->AsObject();
			check(Obj.IsValid()); // should not fail if Type == EJson::Object

			// import the subvalue as a culture invariant string
			FText Text;
			if (!FJsonObjectConverter::GetTextFromObject(Obj.ToSharedRef(), Text))
			{
				UE_LOG(LogJson, Error, TEXT("JsonValueToUProperty - Unable to import JSON object with invalid keys into Text property %s"), *Property->GetAuthoredName());
				return false;
			}
			TextProperty->SetPropertyValue(OutValue, Text);
		}
		else
		{
			UE_LOG(LogJson, Error, TEXT("JsonValueToUProperty - Unable to import JSON value that is neither string nor object into Text property %s"), *Property->GetAuthoredName());
			return false;
		}
	}
	else if (FStructProperty *StructProperty = CastField<FStructProperty>(Property))
	{
		if (JsonValue->Type == EJson::Object)
		{
			TSharedPtr<FJsonObject> Obj = JsonValue->AsObject();
			check(Obj.IsValid()); // should not fail if Type == EJson::Object
			if (!JsonAttributesToUStructWithContainer(Obj->Values, StructProperty->Struct, OutValue, ContainerStruct, Container, CheckFlags & (~CPF_ParmFlags), SkipFlags, bStrictMode))
			{
				UE_LOG(LogJson, Error, TEXT("JsonValueToUProperty - Unable to import JSON object into %s property %s"), *StructProperty->Struct->GetAuthoredName(), *Property->GetAuthoredName());
				return false;
			}
		}
		else if (JsonValue->Type == EJson::String && StructProperty->Struct->GetFName() == NAME_LinearColor)
		{
			FLinearColor& ColorOut = *(FLinearColor*)OutValue;
			FString ColorString = JsonValue->AsString();

			FColor IntermediateColor;
			IntermediateColor = FColor::FromHex(ColorString);

			ColorOut = IntermediateColor;
		}
		else if (JsonValue->Type == EJson::String && StructProperty->Struct->GetFName() == NAME_Color)
		{
			FColor& ColorOut = *(FColor*)OutValue;
			FString ColorString = JsonValue->AsString();

			ColorOut = FColor::FromHex(ColorString);
		}
		else if (JsonValue->Type == EJson::String && StructProperty->Struct->GetFName() == FName("DateTime"))
		{
			FString DateString = JsonValue->AsString();
			FDateTime& DateTimeOut = *(FDateTime*)OutValue;
			if (DateString == TEXT("min"))
			{
				// min representable value for our date struct. Actual date may vary by platform (this is used for sorting)
				DateTimeOut = FDateTime::MinValue();
			}
			else if (DateString == TEXT("max"))
			{
				// max representable value for our date struct. Actual date may vary by platform (this is used for sorting)
				DateTimeOut = FDateTime::MaxValue();
			}
			else if (DateString == TEXT("now"))
			{
				// this value's not really meaningful from JSON serialization (since we don't know timezone) but handle it anyway since we're handling the other keywords
				DateTimeOut = FDateTime::UtcNow();
			}
			else if (FDateTime::ParseIso8601(*DateString, DateTimeOut))
			{
				// ok
			}
			else if (FDateTime::Parse(DateString, DateTimeOut))
			{
				// ok
			}
			else
			{
				UE_LOG(LogJson, Error, TEXT("JsonValueToUProperty - Unable to import JSON string into DateTime property %s"), *Property->GetAuthoredName());
				return false;
			}
		}
		else if (JsonValue->Type == EJson::String && StructProperty->Struct->GetCppStructOps() && StructProperty->Struct->GetCppStructOps()->HasImportTextItem())
		{
			UScriptStruct::ICppStructOps* TheCppStructOps = StructProperty->Struct->GetCppStructOps();

			FString ImportTextString = JsonValue->AsString();
			const TCHAR* ImportTextPtr = *ImportTextString;
			if (!TheCppStructOps->ImportTextItem(ImportTextPtr, OutValue, PPF_None, nullptr, (FOutputDevice*)GWarn))
			{
				// Fall back to trying the tagged property approach if custom ImportTextItem couldn't get it done
				if (Property->ImportText_Direct(ImportTextPtr, OutValue, nullptr, PPF_None) == nullptr)
				{
					UE_LOG(LogJson, Error, TEXT("JsonValueToUProperty - Unable to import JSON string into %s property %s"), *StructProperty->Struct->GetAuthoredName(), *Property->GetAuthoredName());
					return false;
				}
			}
		}
		else if (JsonValue->Type == EJson::String)
		{
			FString ImportTextString = JsonValue->AsString();
			const TCHAR* ImportTextPtr = *ImportTextString;
			if (Property->ImportText_Direct(ImportTextPtr, OutValue, nullptr, PPF_None) == nullptr)
			{
				UE_LOG(LogJson, Error, TEXT("JsonValueToUProperty - Unable to import JSON string into %s property %s"), *StructProperty->Struct->GetAuthoredName(), *Property->GetAuthoredName());
				
				return false;
			}
		}
		else
		{
			UE_LOG(LogJson, Error, TEXT("JsonValueToUProperty - Unable to import JSON value that is neither string nor object into %s property %s"), *StructProperty->Struct->GetAuthoredName(), *Property->GetAuthoredName());
			
			return false;
		}
	}
	else if (FObjectProperty *ObjectProperty = CastField<FObjectProperty>(Property))
	{
		if (JsonValue->Type == EJson::Object)
		{
			UObject* Outer = GetTransientPackage();
			if (ContainerStruct->IsChildOf(UObject::StaticClass()))
			{
				Outer = (UObject*)Container;
			}

			TSharedPtr<FJsonObject> Obj = JsonValue->AsObject();
			UClass* PropertyClass = ObjectProperty->PropertyClass;

			// If a specific subclass was stored in the JSON, use that instead of the PropertyClass
			const FString ObjectClassNameKey = "_ClassName";
			FString ClassString = Obj->GetStringField(ObjectClassNameKey);
			Obj->RemoveField(ObjectClassNameKey);
			if (!ClassString.IsEmpty())
			{
				UClass* FoundClass = FPackageName::IsShortPackageName(ClassString) ? FindFirstObject<UClass>(*ClassString) : UClass::TryFindTypeSlow<UClass>(ClassString);
				if (FoundClass)
				{
					PropertyClass = FoundClass;
				}
			}

			UObject* createdObj = StaticAllocateObject(PropertyClass, Outer, NAME_None, EObjectFlags::RF_NoFlags, EInternalObjectFlags::None, false);
			(*PropertyClass->ClassConstructor)(FObjectInitializer(createdObj, PropertyClass->ClassDefaultObject, EObjectInitializerOptions::None));

			ObjectProperty->SetObjectPropertyValue(OutValue, createdObj);

			check(Obj.IsValid()); // should not fail if Type == EJson::Object
			if (!JsonAttributesToUStructWithContainer(Obj->Values, PropertyClass, createdObj, PropertyClass, createdObj, CheckFlags & (~CPF_ParmFlags), SkipFlags, bStrictMode))
			{
				UE_LOG(LogJson, Error, TEXT("JsonValueToUProperty - Unable to import JSON object into %s property %s"), *PropertyClass->GetAuthoredName(), *Property->GetAuthoredName());
				
				return false;
			}
		}
		else if (JsonValue->Type == EJson::String)
		{
			// Default to expect a string for everything else
			if (Property->ImportText_Direct(*JsonValue->AsString(), OutValue, nullptr, 0) == nullptr)
			{
				UE_LOG(LogJson, Error, TEXT("JsonValueToUProperty - Unable to import JSON string into %s property %s"), *ObjectProperty->PropertyClass->GetAuthoredName(), *Property->GetAuthoredName());
				
				return false;
			}
		}
	}
	else
	{
		// Default to expect a string for everything else
		if (Property->ImportText_Direct(*JsonValue->AsString(), OutValue, nullptr, 0) == nullptr)
		{
			UE_LOG(LogJson, Error, TEXT("JsonValueToUProperty - Unable to import JSON string into property %s"), *Property->GetAuthoredName());
			
			return false;
		}
	}

	return true;
}