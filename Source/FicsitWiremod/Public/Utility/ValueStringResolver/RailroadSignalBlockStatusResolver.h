#pragma once

#include "ValueStringResolverBase.h"
#include "RailroadSignalBlockStatusResolver.generated.h"

UCLASS(Blueprintable, BlueprintType)
class URailroadSignalBlockStatusResolver : public UValueStringResolverBase
{
	GENERATED_BODY()

public:
	virtual FString ResolveString_Implementation(UObject* Object, FName SourceName, EConnectionType Type, bool FromProperty) const override
	{
		if (Type != Integer) return Super::ResolveString_Implementation(Object, SourceName, Type, FromProperty);

		auto Value = (ERailroadSignalAspect) UReflectionUtilities::GetFloat(Object, SourceName, FromProperty);

		FString Result = FString::FromInt((int)Value);

		switch (Value)
		{
		case ERailroadSignalAspect::RSA_None: Result += " (None)"; break;
		case ERailroadSignalAspect::RSA_Clear: Result += " (Clear)"; break;
		case ERailroadSignalAspect::RSA_Dock: Result += " (Begin Docking)"; break;
		case ERailroadSignalAspect::RSA_Stop: Result += " (Stop)"; break;
		}
		
		return Result;
	}
};
