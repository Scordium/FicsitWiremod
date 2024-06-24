#pragma once

#include "CoreMinimal.h"
#include "CircuitryArrayFilterBase.h"
#include "Behaviour/Gates/Arrays/Filter/Rules/CircuitryBoolFilterRule.h"
#include "CircuitryBoolArrayFilter.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UCircuitryBoolArrayFilter : public UCircuitryArrayFilterBase
{
	GENERATED_BODY()

public:
	UCircuitryBoolArrayFilter() : Super(Boolean){}
	
	virtual bool FromJson(const FCircuitryArrayFilterData& FilterData) override { return DeserializeArrayFilter(FilterData, Data.StaticStruct(), &Data); }
	
	virtual FCircuitryArrayFilterData ToJson() override { return SerializeArrayFilter(Data.StaticStruct(), &Data); }

	UPROPERTY(BlueprintReadWrite, SaveGame, Replicated)
	FCircuitryBoolFilterRule Data;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(ThisClass, Data);
	}

	TArray<bool> FilterValues(const TArray<bool>& In) const { return In.FilterByPredicate([this](const bool& Val) { return Data.CheckFilterMatch(Val, true); }); }
};
