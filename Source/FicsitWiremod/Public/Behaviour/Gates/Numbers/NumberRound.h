// 

#pragma once

#include "CoreMinimal.h"
#include "MultistateWiremodBuildable.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NumberRound.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberRound : public AMultistateWiremodBuildable
{
	GENERATED_BODY()
    
public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		if (CurrentStateIndex == 0) Out = FMath::RoundToInt(GetConnection(0).GetFloat());
		else if (CurrentStateIndex == 1)
		{
			ArrayOut = GetConnection(0).GetFloatArray();

			for (int i = 0; i < ArrayOut.Num(); i++)
			{
				ArrayOut[i] = FMath::RoundToDouble(ArrayOut[i]);
			}
		}
	}

	virtual void OnStateSelected_Internal(int Index) override
	{
		Super::OnStateSelected_Internal(Index);

		Out = 0;
		ArrayOut.Empty();
	}
    
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
		DOREPLIFETIME(ANumberRound, Out);
		DOREPLIFETIME(ANumberRound, ArrayOut);
	}
    	
	UPROPERTY(Replicated, SaveGame)
	int Out;

	UPROPERTY(Replicated, SaveGame)
	TArray<double> ArrayOut;
};
