#pragma once

#include "VectorUtils.generated.h"

UCLASS()
class FICSITWIREMOD_API UVectorUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	static FVector SetX(FVector In, double Val){ return FVector(Val, In.Y, In.Z); }
	
	UFUNCTION(BlueprintPure)
	static FVector SetY(FVector In, double Val){ return FVector(In.X, Val, In.Z); }

	UFUNCTION(BlueprintPure)
	static FVector SetZ(FVector In, double Val){ return FVector(In.X, In.Y, Val); }

	UFUNCTION(BlueprintPure)
	static FVector2D GridSnap2D(const FVector2D& InVec, const double GridSize){ return FVector2D(FMath::GridSnap(InVec.X, GridSize), FMath::GridSnap(InVec.Y, GridSize)); }

	UFUNCTION(BlueprintPure)
	static FVector ClampSize_Number(const FVector& InVec, const double Min, const double Max)
	{
		return FVector(
			FMath::Clamp(InVec.X, Min, Max),
			FMath::Clamp(InVec.Y, Min, Max),
			FMath::Clamp(InVec.Z, Min, Max)
			);
	}

	UFUNCTION(BlueprintPure)
	static FVector2D ClampSize2D_Number(const FVector2D& InVec, const double Min, const double Max)
	{
		return FVector2D(
			FMath::Clamp(InVec.X, Min, Max),
			FMath::Clamp(InVec.Y, Min, Max)
			);
	}

	UFUNCTION(BlueprintPure)
	static FVector ClampSize_Vector(const FVector& InVec, const FVector& Min, const FVector& Max)
	{
		return FVector(
			FMath::Clamp(InVec.X, Min.X, Max.X),
			FMath::Clamp(InVec.Y, Min.Y, Max.Y),
			FMath::Clamp(InVec.Z, Min.Z, Max.Z)
			);
	}

	UFUNCTION(BlueprintPure)
	static FVector2D ClampSize2D_Vector(const FVector2D& InVec, const FVector2D& Min, const FVector2D& Max)
	{
		return FVector2D(FMath::Clamp(InVec.X, Min.X, Max.X), FMath::Clamp(InVec.Y, Min.Y, Max.Y));
	}
};
