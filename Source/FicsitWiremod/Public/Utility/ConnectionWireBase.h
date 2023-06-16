// 

#pragma once

#include "CoreMinimal.h"
#include "ConnectionWireSplineMesh.h"
#include "WiremodReflection.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "ConnectionWireBase.generated.h"


UCLASS()
class FICSITWIREMOD_API AConnectionWireBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AConnectionWireBase();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AConnectionWireBase, AssignedConnection);
		DOREPLIFETIME(AConnectionWireBase, PreviewMode);
	};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override
	{
		Super::BeginPlay();
		
		if(!HasAuthority())
			DrawWireFromData();
	}

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override
	{
		if(!PreviewMode)
			if(HasAuthority())
				if(!AssignedConnection.IsValidForWire())
					Invalidate();
	}

	UFUNCTION(BlueprintCallable)
	void DrawWireFromData();

	UFUNCTION(BlueprintCallable)
	void UpdateWireVisuals();

	UFUNCTION(BlueprintCallable)
	void Invalidate()
	{
		DestroyAllSplineMeshes();
		Destroy();
	}
	
	UFUNCTION(BlueprintCallable)
	void DestroyAllSplineMeshes()
	{
		TInlineComponentArray<UConnectionWireSplineMesh*> SplineMeshes;
		GetComponents<UConnectionWireSplineMesh>(SplineMeshes);
		for (auto SplineMesh : SplineMeshes)
			if(SplineMesh) SplineMesh->DestroyComponent();
	}

	UFUNCTION(BlueprintCallable)
	void DrawWireFromPoints(const TArray<FVector>& Points, ESplineCoordinateSpace::Type CoordinateSpace = ESplineCoordinateSpace::World);

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated)
	FDynamicConnectionData AssignedConnection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool PreviewMode;
	
	UPROPERTY()
	class USplineComponent* Spline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* WireMesh = LoadObject<UStaticMesh>(NULL, *FString("/FicsitWiremod/Buildings/SpecialCube.SpecialCube"));
};
