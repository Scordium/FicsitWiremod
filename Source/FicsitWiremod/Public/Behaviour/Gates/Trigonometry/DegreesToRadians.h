// 

#pragma once

#include "CoreMinimal.h"
#include "MultistateWiremodBuildable.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "DegreesToRadians.generated.h"

UCLASS()
class FICSITWIREMOD_API ADegreesToRadians : public AMultistateWiremodBuildable
{
	GENERATED_BODY()
        
public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		if (CurrentStateIndex == 0)
		{
			Out = UKismetMathLibrary::DegreesToRadians(GetConnection(0).GetFloat());
		}
		else if (CurrentStateIndex == 1)
		{
			auto const DegreesArray = GetConnection(0).GetFloatArray();

			OutArray.SetNum(DegreesArray.Num());
			for (int i = 0; i < OutArray.Num(); i++)
			{
				OutArray[i] = UKismetMathLibrary::DegreesToRadians(DegreesArray[i]);
			}
		}
	}
        
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
        
		DOREPLIFETIME(ADegreesToRadians, Out);
		DOREPLIFETIME(ADegreesToRadians, OutArray);
	}

	virtual void OnStateSelected_Internal(int Index) override
	{
		Super::OnStateSelected_Internal(Index);

		Out = 0;
		OutArray.Empty();
	}
        
        
	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	double Out;

	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	TArray<double> OutArray;
};
