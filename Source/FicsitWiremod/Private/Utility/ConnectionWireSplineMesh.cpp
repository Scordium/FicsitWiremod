// 


#include "Utility/ConnectionWireSplineMesh.h"


// Sets default values for this component's properties
UConnectionWireSplineMesh::UConnectionWireSplineMesh()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Mobility = EComponentMobility::Movable;
	ForwardAxis = ESplineMeshAxis::Y;
	SetStartScale(FVector2D(0.03, 0.03));
	SetEndScale(FVector2D(0.03, 0.03));
	bHiddenInGame = false;
}


// Called when the game starts
void UConnectionWireSplineMesh::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UConnectionWireSplineMesh::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

