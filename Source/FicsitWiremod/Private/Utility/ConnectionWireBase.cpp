// 


#include "Utility/ConnectionWireBase.h"

#include "Components/SplineComponent.h"
#include "Utility/ConnectionWireSplineMesh.h"


// Sets default values
AConnectionWireBase::AConnectionWireBase() : Super()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
	SetRootComponent(Spline);
	NetDormancy = DORM_Never;
	bReplicates = true;
}



void AConnectionWireBase::DrawWire(FDynamicConnectionData Data)
{
	if(!Spline)
	{
		Spline = Cast<USplineComponent>(AddComponentByClass(USplineComponent::StaticClass(), false, FTransform::Identity, false));
		Spline->Activate();
		Spline->SetIsReplicated(true);
		RootComponent = Spline;
	}
	
	AssignedConnection = Data;
	if(!Data.Receiver.Object || !Data.Transmitter.Object) return;
	
	TArray<FVector> Points = AssignedConnection.Transmitter.WirePositions;
	
	if(Points.Num() == 0)
	{
		Points.SetNum(2);

		if(auto transmitter = Cast<AActor>(Data.Transmitter.Object))
			Points[0] = transmitter->GetActorLocation();
		
		if(auto receiver = Cast<AActor>(Data.Receiver.Object))
			Points[1] = receiver->GetActorLocation();
	}

	Spline->SetSplinePoints(Points, ESplineCoordinateSpace::World);
	for(int i = 1; i < Points.Num(); i++)
	{
		auto SplineMesh = Cast<UConnectionWireSplineMesh>(AddComponentByClass(UConnectionWireSplineMesh::StaticClass(), false, FTransform::Identity, false));
		SplineMesh->SetStaticMesh(WireMesh);

		FVector StartPos = Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		FVector StartTan = Spline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
		FVector EndPos = Spline->GetLocationAtSplinePoint(i - 1, ESplineCoordinateSpace::Local);
		FVector EndTan = Spline->GetTangentAtSplinePoint(i - 1, ESplineCoordinateSpace::Local);
		SplineMesh->SetStartAndEnd(StartPos, StartTan, EndPos, EndTan, true);
	}

	UpdateWireVisuals();
}


