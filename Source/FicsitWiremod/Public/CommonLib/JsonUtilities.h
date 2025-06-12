// 

#pragma once

#include "CoreMinimal.h"
#include "FGTrainStationIdentifier.h"
#include "ReflectionUtilities.h"
#include "Runtime/Engine/Classes/Kismet/BlueprintFunctionLibrary.h"
#include "JsonUtilities.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UJsonUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static TSharedRef<FJsonValue> TimeTableStopToJson(const FTimeTableStopData& Value)
	{
		const TSharedRef<FJsonObject> Object = MakeShareable(new FJsonObject());

		Object->SetStringField("StationName", Value.Station ? Value.Station->GetStationName().ToString() : "N/A");

		const TSharedRef<FJsonObject> DockingRules = MakeShareable(new FJsonObject());
		DockingRules->SetNumberField("DockingDefinition", (int)Value.DockingRuleSet.DockingDefinition);
		DockingRules->SetNumberField("DockingDuration", Value.DockingRuleSet.DockForDuration);
		DockingRules->SetBoolField("IsDurationAndRule", Value.DockingRuleSet.IsDurationAndRule);
		DockingRules->SetBoolField("IgnoreFullLoadUnloadIfTransferBlockedByFilters", Value.DockingRuleSet.IgnoreFullLoadUnloadIfTransferBlockedByFilters);

		TArray<TSharedPtr<FJsonValue>> LoadDescriptors;
		for (auto& LoadFilterDescriptor : Value.DockingRuleSet.LoadFilterDescriptors)
		{
			LoadDescriptors.Add(MakeShareable(new FJsonValueString(FSoftClassPath(LoadFilterDescriptor).ToString())));
		}
		DockingRules->SetArrayField("LoadDescriptors", LoadDescriptors);

		TArray<TSharedPtr<FJsonValue>> UnloadDescriptors;
		for (auto& UnloadFilterDescriptor : Value.DockingRuleSet.UnloadFilterDescriptors)
		{
			UnloadDescriptors.Add(MakeShareable(new FJsonValueString(FSoftClassPath(UnloadFilterDescriptor).ToString())));
		}
		DockingRules->SetArrayField("UnloadDescriptors", UnloadDescriptors);
		
		Object->SetObjectField("DockingRules", DockingRules);

		return MakeShareable(new FJsonValueObject(Object));
	}

	template<typename OutStructType>
	static bool JsonObjectToUStruct(const TSharedRef<FJsonObject>& JsonObject, OutStructType* OutStruct, int64 CheckFlags = 0, int64 SkipFlags = 0, const bool bStrictMode = false)
	{
		return JsonObjectToUStruct(JsonObject, OutStructType::StaticStruct(), OutStruct, CheckFlags, SkipFlags, bStrictMode);
	}

	static bool JsonObjectToUStruct(const TSharedRef<FJsonObject>& JsonObject, const UStruct* StructDefinition, void* OutStruct, int64 CheckFlags = 0, int64 SkipFlags = 0, const bool bStrictMode = false)
	{
		return JsonAttributesToUStruct(JsonObject->Values, StructDefinition, OutStruct, CheckFlags, SkipFlags, bStrictMode);
	}

	static bool JsonAttributesToUStruct(const TMap< FString, TSharedPtr<FJsonValue> >& JsonAttributes, const UStruct* StructDefinition, void* OutStruct, int64 CheckFlags, int64 SkipFlags, const bool bStrictMode)
	{
		return JsonAttributesToUStructWithContainer(JsonAttributes, StructDefinition, OutStruct, StructDefinition, OutStruct, CheckFlags, SkipFlags, bStrictMode);
	}
	
	static bool JsonAttributesToUStructWithContainer(const TMap< FString, TSharedPtr<FJsonValue> >& JsonAttributes, const UStruct* StructDefinition, void* OutStruct, const UStruct* ContainerStruct, void* Container, int64 CheckFlags, int64 SkipFlags, const bool bStrictMode);

	static bool JsonValueToFPropertyWithContainer(const TSharedPtr<FJsonValue>& JsonValue, FProperty* Property, void* OutValue, const UStruct* ContainerStruct, void* Container, int64 CheckFlags, int64 SkipFlags, const bool bStrictMode);

	static bool ConvertScalarJsonValueToFPropertyWithContainer(const TSharedPtr<FJsonValue>& JsonValue, FProperty* Property, void* OutValue, const UStruct* ContainerStruct, void* Container, int64 CheckFlags, int64 SkipFlags, const bool bStrictMode);
};
