// 

#pragma once

#include "CoreMinimal.h"
#include "ValueStringResolverBase.h"
#include "PercentageValueResolver.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FICSITWIREMOD_API UPercentageValueResolver : public UValueStringResolverBase
{
	GENERATED_BODY()
public:
	virtual FString ResolveString_Implementation(const FConnectionPointer& Pointer, EConnectionType Type) const override
	{
		if (Type != EConnectionType::Number) return Super::ResolveString_Implementation(Pointer, Type);

		auto Value = UReflectionUtilities::GetFloat(Pointer);
		return FString::SanitizeFloat(Value) + FString::Printf(TEXT(" (%.2f%%)"), Value*100.0);
	}
};
