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
};
