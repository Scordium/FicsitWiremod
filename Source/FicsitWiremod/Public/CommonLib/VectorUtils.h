#pragma once

#include "VectorUtils.generated.h"

UCLASS()
class FICSITWIREMOD_API UVectorUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure)
	static FVector SetX(FVector In, double Val){ return FVector(Val, In.Y, In.Z); }
	
	UFUNCTION(BlueprintPure)
	static FVector SetY(FVector In, double Val){ return FVector(In.X, Val, In.Z); }

	UFUNCTION(BlueprintPure)
	static FVector SetZ(FVector In, double Val){ return FVector(In.X, In.Y, Val); }
	
	
};
