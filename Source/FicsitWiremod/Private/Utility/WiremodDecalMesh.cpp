// 


#include "Utility/WiremodDecalMesh.h"

#include "WiremodUtils.h"
#include "Behaviour/CircuitryStatics.h"
#include "Utility/WiremodGameWorldModule.h"


// Sets default values for this component's properties
UWiremodDecalMesh::UWiremodDecalMesh() : Super()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
	SetRelativeLocation(FVector(0, 0, 8.5));
	SetRelativeScale3D(FVector(0.35, 0.35, 0.35));
	SetStaticMesh(UCircuitryStatics::GetGateDecalMesh());
	SetMaterial(0, UCircuitryStatics::GetGateDecalMaterial());
	
	// ...
}

void UWiremodDecalMesh::BeginPlay()
{
	Super::BeginPlay();
	UpdateTexture();
}

