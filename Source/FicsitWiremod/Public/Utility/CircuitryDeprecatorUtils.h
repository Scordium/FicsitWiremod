// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "Behaviour/VanillaInterface/WiremodVanillaConnections.h"
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
	static void GenerateMultistateBuildable(AFGWiremodBuildable* Old, TSubclassOf<AMultistateWiremodBuildable> Class, int StateIndex, TSubclassOf<UFGRecipe> WithRecipe)
	{
		auto const Transform = Old->GetActorTransform();
		auto NewBuildable = Cast<AMultistateWiremodBuildable>(Old->GetWorld()->SpawnActor(Class.GetDefaultObject()->GetClass(), &Transform));
		NewBuildable->SetBuiltWithRecipe(WithRecipe);

		NewBuildable->OnStateSelected_Internal(StateIndex); //Set the state index
		NewBuildable->InputConnections = Old->InputConnections; //Transfer connections
		NewBuildable->OwnerData = Old->OwnerData; // Transfer owner data
		
		TArray<AFGWiremodBuildable*> Gates;
		AFGBuildableSubsystem::Get(Old)->GetTypedBuildable<AFGWiremodBuildable>(Gates);
		
		for(auto Gate : Gates)
		{
			if(Gate->InputConnections.Num() == 0) continue;

			for(auto& Input : Gate->InputConnections)
				if(Input.Object == Old) Input.Object = NewBuildable;
			
			Gate->DrawWires_Implementation();
		}

		ACircuitryLogger::DispatchWarningEvent(Old->GetName() + " was converted into " + NewBuildable->GetName());
		// "Do not directly call Event functions in Interfaces" my ass, UE.
		IFGDismantleInterface::Execute_Dismantle(Old);
	}
};
