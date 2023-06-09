// 

#pragma once

#include "CoreMinimal.h"
#include "KismetAnimationLibrary.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "PerlinNoise3D.generated.h"

UCLASS()
class FICSITWIREMOD_API APerlinNoise3D : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		float X = GetConnection(0).GetFloat();
		float Y = GetConnection(1).GetFloat();
		float Z = GetConnection(2).GetFloat();
		float MinX = GetConnection(3).GetFloat();
		float MaxX = GetConnection(4).GetFloat();
		float MinY = GetConnection(5).GetFloat();
		float MaxY = GetConnection(6).GetFloat();
		float MinZ = GetConnection(7).GetFloat();
		float MaxZ = GetConnection(8).GetFloat();
		
		Out = UKismetAnimationLibrary::K2_MakePerlinNoiseVectorAndRemap(X, Y, Z, MinX, MaxX, MinY, MaxY, MinZ, MaxZ);
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(APerlinNoise3D, Out);
	}

	UPROPERTY(VisibleInstanceOnly, Replicated)
	FVector Out;
};
