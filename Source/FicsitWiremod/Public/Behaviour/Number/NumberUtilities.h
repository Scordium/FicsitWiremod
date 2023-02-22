// 

#pragma once

#include "CoreMinimal.h"
#include "WiremodReflection.h"
#include "NumberUtilities.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UNumberUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure)
	static float CalculateAverage(TArray<float> In)
	{
		if(In.Num() == 0) return 0;
		
		float a = 0;
		for (float Element : In){ a += Element; }
		
		return a / In.Num();
	}
	
};
