#pragma once

#include "CoreMinimal.h"
#include "CCBoolValue.h"
#include "CCDynamicValueUtils.h"
#include "FGWiremodBuildable.h"
#include "JsonToStruct.generated.h"

UCLASS()
class FICSITWIREMOD_API AJsonToStruct : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		auto Json = GetConnection(0).GetString();
		
		if (JsonCache == Json) return;
		
		JsonCache = Json;
		
		TSharedPtr<FJsonObject> Object;
		IsValidJson = FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Json), Object);
		
		Out = ParseJsonObject(Object);
	}

	FCustomStruct ParseJsonObject(const TSharedPtr<FJsonObject>& Object)
	{
		FCustomStruct Output;
		Output.Name = "JSON Object";
		
		if (!Object.IsValid()) return Output;
		
		for (auto& JsonValueTuple : Object.Get()->Values)
		{
			switch (JsonValueTuple.Value->Type)
			{
			case EJson::Boolean:
				{
					auto Value = UCCDynamicValueUtils::FromType<UCCBoolValue>(Boolean, this);
					JsonValueTuple.Value->TryGetBool(Value->Value);

					Output.Values.Add(FNamedDynamicValue(JsonValueTuple.Key, Value));
					break;
				}

			case EJson::String:
				{
					if (JsonValueTuple.Key == "__CIRCUITRY_STRUCT_NAME__")
					{
						JsonValueTuple.Value->TryGetString(Output.Name);
					}
					else
					{
						auto Value = UCCDynamicValueUtils::FromType<UCCStringValue>(String, this);
						JsonValueTuple.Value->TryGetString(Value->Value);

						Output.Values.Add(FNamedDynamicValue(JsonValueTuple.Key, Value));
						break;
					}
					
					break;
				}

			case EJson::Number:
				{
					auto Value = UCCDynamicValueUtils::FromType<UCCNumberValue>(Number, this);
					JsonValueTuple.Value->TryGetNumber(Value->Value);

					Output.Values.Add(FNamedDynamicValue(JsonValueTuple.Key, Value));
					break;
				}

			case EJson::Array:
				{
					Output.Values.Add(FNamedDynamicValue(JsonValueTuple.Key, ParseJsonArray(JsonValueTuple.Value)));
					break;
				}

			case EJson::Object:
				{
					auto Value = UCCDynamicValueUtils::FromType<UCCCustomStructValue>(CustomStruct, this);
					TSharedPtr<FJsonObject>* JsonObject;
					JsonValueTuple.Value->TryGetObject(JsonObject);
					Value->Value = ParseJsonObject(*JsonObject);
					
					Output.Values.Add(FNamedDynamicValue(JsonValueTuple.Key, Value));
					break;
				}

			case EJson::None:
			case EJson::Null:
				{
					Output.Values.Add(FNamedDynamicValue(JsonValueTuple.Key, nullptr));
					ACircuitryLogger::DispatchErrorEvent(TEXT("\"" + JsonValueTuple.Key + "\" field in json was of type none/null. Make sure this is intended."));
					break;
				}
			}
		}

		return Output;
	}

	UCCDynamicValueBase* ParseJsonArray(const TSharedPtr<FJsonValue>& JsonValue)
	{
		if (JsonValue->Type != EJson::Array) return nullptr;

		auto ArrayValues = JsonValue->AsArray();
		if (ArrayValues.Num() == 0) return nullptr;

		UCCDynamicValueBase* Output = nullptr;
		EJson ArrayType = EJson::None;
		
		//First find a valid array type
		for (int i = 0; i < ArrayValues.Num(); i++)
		{
			switch (ArrayValues[i]->Type)
			{
			case EJson::Array:
			case EJson::None:
			case EJson::Null:
			default:
				return nullptr;

			case EJson::Boolean:
				ArrayType = EJson::Boolean;
				Output = UCCDynamicValueUtils::FromType(ArrayOfBoolean, this);
				break;
				
			case EJson::Number:
				ArrayType = EJson::Number;
				Output = UCCDynamicValueUtils::FromType(ArrayOfNumber, this);
				break;

			case EJson::String:
				ArrayType = EJson::String;
				Output = UCCDynamicValueUtils::FromType(ArrayOfString, this);
				break;

			case EJson::Object:
				ArrayType = EJson::Object;
				Output = UCCDynamicValueUtils::FromType(ArrayOfCustomStruct, this);
				break;
			}
		}

		//Now parse all values according to that array type. This is a bit of a hindrance, but not much i can do here. Use fucking arrays properly. /shrug
		switch (ArrayType)
		{
			
		case EJson::Boolean:
			{
				auto OutputCasted = Cast<UCCBoolArrayValue>(Output);
				for (int i = 0; i < ArrayValues.Num(); i++) OutputCasted->Value.Add(ArrayValues[i]->AsBool());
				break;
			}

		case EJson::Number:
			{
				auto OutputCasted = Cast<UCCNumberArrayValue>(Output);
				for (int i = 0; i < ArrayValues.Num(); i++) OutputCasted->Value.Add(ArrayValues[i]->AsNumber());
				break;
			}

		case EJson::String:
			{
				auto OutputCasted = Cast<UCCStringArrayValue>(Output);
				for (int i = 0; i < ArrayValues.Num(); i++) OutputCasted->Value.Add(ArrayValues[i]->AsString());
				break;
			}
			
		case EJson::Object:
			{
				auto OutputCasted = Cast<UCCCustomStructArrayValue>(Output);
				for (int i = 0; i < ArrayValues.Num(); i++) OutputCasted->Value.Add(ParseJsonObject(ArrayValues[i]->AsObject()));
				break;
			}

		default: return nullptr;
			
		}

		return Output;
	}


	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ThisClass, Out)
		DOREPLIFETIME(ThisClass, IsValidJson)
	}

	UPROPERTY(Replicated, SaveGame)
	FCustomStruct Out;

	UPROPERTY(Replicated, SaveGame)
	bool IsValidJson;

	UPROPERTY()
	FString JsonCache;
};
