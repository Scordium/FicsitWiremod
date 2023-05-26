// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CircuitryDeprecatorUtils.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UCircuitryDeprecatorUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable, meta=(DefaultToSelf="Old"))
	static void GenerateMultistateBuildable(AFGWiremodBuildable* Old, TSubclassOf<AMultistateWiremodBuildable> Class, int StateIndex)
	{
		auto const Transform = Old->GetActorTransform();
		auto NewBuildable = Cast<AMultistateWiremodBuildable>(Old->GetWorld()->SpawnActor(Class.GetDefaultObject()->GetClass(), &Transform));
		NewBuildable->CurrentStateIndex = StateIndex;
		NewBuildable->InputConnections = Old->InputConnections;
	}
};
