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
		float X = WM_GetFloat(0);
		float Y = WM_GetFloat(1);
		float Z = WM_GetFloat(2);
		float MinX = WM_GetFloat(3);
		float MaxX = WM_GetFloat(4);
		float MinY = WM_GetFloat(5);
		float MaxY = WM_GetFloat(6);
		float MinZ = WM_GetFloat(7);
		float MaxZ = WM_GetFloat(8);
		
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
