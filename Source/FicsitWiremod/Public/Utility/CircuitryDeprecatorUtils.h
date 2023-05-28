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
		NewBuildable->InputConnections = Old->InputConnections; //Transfer connections
		
		//Calling this *before* setting owner data to avoid problems with object ownership getting in the way
		NewBuildable->OnStateSelected(StateIndex, NULL); //Set the state index
		
		NewBuildable->OwnerData = Old->OwnerData; // Transfer owner data

		// "Do not directly call Event functions in Interfaces" my ass, UE.
		IFGDismantleInterface::Execute_Dismantle(Old);
	}
};
