// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "RadiansToDegrees.generated.h"

UCLASS()
class FICSITWIREMOD_API ARadiansToDegrees : public AMultistateWiremodBuildable
{
	GENERATED_BODY()
        
public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		if (CurrentStateIndex == 0)
		{
			Out = UKismetMathLibrary::RadiansToDegrees(GetConnection(0).GetFloat());
		}
		else if (CurrentStateIndex == 1)
		{
			auto const RadiansArray = GetConnection(0).GetFloatArray();

			OutArray.SetNum(RadiansArray.Num());

			for (int i = 0; i < RadiansArray.Num(); i++)
			{
				OutArray[i] = UKismetMathLibrary::RadiansToDegrees(RadiansArray[i]);
			}
		}
	}
        
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
        
		DOREPLIFETIME(ARadiansToDegrees, Out);
		DOREPLIFETIME(ARadiansToDegrees, OutArray);
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
