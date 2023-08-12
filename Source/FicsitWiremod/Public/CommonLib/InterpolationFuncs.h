// 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InterpolationFuncs.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UInterpolationFuncs : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	static float BounceIn(double A, double B, double Alpha);
	static float BounceInOut(double A, double B, double Alpha);
	static float BounceOut(double A, double B, double Alpha);



	UFUNCTION(BlueprintPure)
	static float TransformAlpha_BounceIn(double Alpha);

	UFUNCTION(BlueprintPure)
	static float TransformAlpha_BounceOut(double Alpha);

	UFUNCTION(BlueprintPure)
	static float TransformAlpha_BounceInOut(double Alpha);
};
