// 

#pragma once

#include "CoreMinimal.h"
#include "ConnectionWireSplineMesh.h"
#include "WiremodReflection.h"
#include "GameFramework/Actor.h"
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
			DrawWire(AssignedConnection);
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
	void DrawWire(FDynamicConnectionData Data);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateWireVisuals();

	UFUNCTION(BlueprintCallable)
	void Invalidate()
	{
		AssignedConnection = FDynamicConnectionData();
		PreviewMode = false;

		TArray<UConnectionWireSplineMesh*> SplineMeshes;
		GetComponents<UConnectionWireSplineMesh>(SplineMeshes);
		for (auto SplineMesh : SplineMeshes) SplineMesh->DestroyComponent();
		
		Destroy();
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FDynamicConnectionData AssignedConnection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool PreviewMode;



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USplineComponent* Spline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* WireMesh;
};
