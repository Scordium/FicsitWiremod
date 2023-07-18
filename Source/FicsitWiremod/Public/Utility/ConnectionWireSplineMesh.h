// 

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineMeshComponent.h"
#include "ConnectionWireSplineMesh.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FICSITWIREMOD_API UConnectionWireSplineMesh : public USplineMeshComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UConnectionWireSplineMesh()
	{
		PrimaryComponentTick.bCanEverTick = false;

		Mobility = EComponentMobility::Movable;
		ForwardAxis = ESplineMeshAxis::Y;
		SetStartScale(FVector2D(0.03, 0.03));
		SetEndScale(FVector2D(0.03, 0.03));
		bHiddenInGame = false;
	}
};
