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
	virtual FString ResolveString_Implementation(UObject* Object, FName SourceName, EConnectionType Type, bool FromProperty) const override
	{
		if (Type != EConnectionType::Number) return Super::ResolveString_Implementation(Object, SourceName, Type, FromProperty);

		auto Value = UReflectionUtilities::GetFloat(Object, SourceName, FromProperty);
		return FString::SanitizeFloat(Value) + FString::Printf(TEXT(" (%.2f%%)"), Value*100.0);
	}
};
