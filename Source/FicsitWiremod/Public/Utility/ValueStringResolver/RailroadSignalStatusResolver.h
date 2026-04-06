// 

#pragma once

#include "CoreMinimal.h"
#include "ValueStringResolverBase.h"
#include "RailroadSignalStatusResolver.generated.h" 

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FICSITWIREMOD_API URailroadSignalStatusResolver : public UValueStringResolverBase
{
	GENERATED_BODY()

public:
	virtual FString ResolveString_Implementation(const FConnectionPointer& Pointer, EConnectionType Type) const override
	{
		if (Type != Integer) return Super::ResolveString_Implementation(Pointer, Type);

		auto Value = (ERailroadBlockValidation) UReflectionUtilities::GetFloat(Pointer);

		FString Result = FString::FromInt((int) Value);

		switch (Value)
		{
		case ERailroadBlockValidation::RBV_Unvalidated: Result += " (Unvalidated)"; break;
		case ERailroadBlockValidation::RBV_Valid: Result += " (Valid)"; break;
		case ERailroadBlockValidation::RBV_ContainsLoop: Result += " (Contains a Loop)"; break;
		case ERailroadBlockValidation::RBV_ContainsStation: Result += " (Contains a Station)"; break;
		case ERailroadBlockValidation::RBV_NoExitSignals: Result += " (No Exit Signals)"; break;
		case ERailroadBlockValidation::RBV_ContainsMixedEntrySignals: Result += " (Mixed Entry Signals)"; break;
		}

		return Result;
	}
};
