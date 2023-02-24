// 

#pragma once

#include "CoreMinimal.h"
#include "WiremodReflection.h"
#include "VectorUtilities.generated.h"

/**
 * Copies kismet math functions, but suppresses warnings.
 */
UCLASS()
class FICSITWIREMOD_API UVectorUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure)
	static FVector DivideVectorVector(const FNewConnectionData& Vector1, const FNewConnectionData& Vector2)
	{
		FVector v1 = UWiremodReflection::GetFunctionVectorResult(Vector1);
		FVector v2 = UWiremodReflection::GetFunctionVectorResult(Vector2);

		if(v2.X == 0 || v2.Y == 0 || v2.Z == 0) return FVector::ZeroVector;
		return v1/v2;
	}
	
	UFUNCTION(BlueprintPure)
	static FVector DivideVectorNumber(const FNewConnectionData& Vector, const FNewConnectionData& Number)
	{
		FVector v1 = UWiremodReflection::GetFunctionVectorResult(Vector);
		float n = UWiremodReflection::GetFunctionNumberResult(Number);

		if(n == 0) return FVector::ZeroVector;
		return v1/n;
	}
};
