// 

#pragma once

#include "CoreMinimal.h"
#include "WiremodUtils.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "RangerSensor.generated.h"


UCLASS()
class FICSITWIREMOD_API ARangerSensor : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:

	ARangerSensor()
	{
		Spline = CreateDefaultSubobject<USplineComponent>("Spline");
		SplineMesh = CreateDefaultSubobject<USplineMeshComponent>("SplineMesh");
	}

	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ARangerSensor, MaxDistance)
		DOREPLIFETIME(ARangerSensor, HasTarget)
		DOREPLIFETIME(ARangerSensor, HitInfo)
	}

	
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		MaxDistance = GetConnection(0).GetFloat(1000);
		
		auto ActorIgnoreList = TArray<AActor*>();
		TraceForHit(ActorIgnoreList);
	}

	virtual void ClientProcess_Implementation(double DeltaTime) override
	{
		RenderLaser();
	}

	
	void TraceForHit(TArray<AActor*>& IgnoredActors)
	{
		FVector StartTracePos = GetActorLocation() + GetActorUpVector() * 5;
		//Adding a small offset to fix the spline becoming invisible when it's completely vertically straight
		FVector EndTracePos = GetActorLocation() + GetActorUpVector() * MaxDistance + FVector(0.011);
			
		HasTarget = UKismetSystemLibrary::LineTraceSingle(
			this,
			StartTracePos,
			EndTracePos,
			TraceTypeQuery4,
			false,
			IgnoredActors,
			EDrawDebugTrace::None,
			HitInfo,
			true
			);

		if(!ConfirmHit())
		{
			//Adding the actor this way allows to ignore all buildable walls after one loop,
			//since HitResult will contain AbstractInstance actor rather than the buildable.
			IgnoredActors.Add(HitInfo.GetActor());

			//Keep looking for a target until a hit is confirmed
			TraceForHit(IgnoredActors);
		}
	}

	bool ConfirmHit()
	{
		//If there's nothing in our way then we confirm the hit to avoid an infinite loop of looking for a target.
		if(!HasTarget) return true;

		//Get the actor
		FVector HitLoc;
		auto Target = UWiremodUtils::GetActualHitTarget(HitInfo, HitLoc);

		//If the actor is not a buildable then we don't need to check if it should be ignored, so we just return true.
		if(!Target->GetClass()->IsChildOf(AFGBuildable::StaticClass())) return true;

		//Otherwise check if the ranger should ignore buildings (only react to players, vehicles, and wildlife)
		//If it should, then return false and continue looking for a new target
		return !GetConnection(2).GetBool();
	}
	
	void RenderLaser()
	{
		auto SplinePoints = TArray<FVector>
		{
			HitInfo.TraceStart,
			HasTarget ? HitInfo.Location : HitInfo.TraceEnd
		};

		Spline->SetSplinePoints(SplinePoints, ESplineCoordinateSpace::World);

		SplineMesh->SetHiddenInGame(GetConnection(1).GetBool());

		FVector StartLoc, StartTan, EndLoc, EndTan;
		Spline->GetLocationAndTangentAtSplinePoint(0, StartLoc, StartTan, ESplineCoordinateSpace::Local);
		Spline->GetLocationAndTangentAtSplinePoint(1, EndLoc, EndTan, ESplineCoordinateSpace::Local);

		SplineMesh->SetStartAndEnd(StartLoc, StartTan, EndLoc, EndTan);
	}


	UFUNCTION()
	double Distance() {return HitInfo.Distance;}

	UFUNCTION()
	FVector GetHitPoint(){ return HitInfo.ImpactPoint; }

	UFUNCTION()
	AActor* GetEntityHit() {FVector Loc; return UWiremodUtils::GetActualHitTarget(HitInfo, Loc); }
	
	UPROPERTY(Replicated, SaveGame)
	double MaxDistance;

	UPROPERTY(Replicated, SaveGame)
	bool HasTarget;

	UPROPERTY(Replicated)
	FHitResult HitInfo;


	UPROPERTY(Instanced, BlueprintReadWrite, EditAnywhere)
	USplineComponent* Spline;

	UPROPERTY(Instanced, BlueprintReadWrite, EditAnywhere)
	USplineMeshComponent* SplineMesh;
};
