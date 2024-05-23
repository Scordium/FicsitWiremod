#include "Utility/WiremodDecalMesh.h"
#include "Behaviour/CircuitryStatics.h"


UWiremodDecalMesh::UWiremodDecalMesh() : Super()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	SetStaticMesh(UCircuitryStatics::GetGateDecalMesh());
	SetMaterial(0, UCircuitryStatics::GetGateDecalMaterial());
}

void UWiremodDecalMesh::BeginPlay()
{
	Super::BeginPlay();
	UpdateTexture();
}

