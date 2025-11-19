// 

#pragma once

#include "CoreMinimal.h"
#include "MultistateWiremodBuildable.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "StringToNumber.generated.h"

UCLASS()
class FICSITWIREMOD_API AStringToNumber : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		if (CurrentStateIndex == 0)
		{
			Out = FCString::Atod(*GetConnection(0).GetString());
		}
		else if (CurrentStateIndex == 1)
		{
			auto const StringArray = GetConnection(0).GetStringArray();

			OutArray.SetNum(StringArray.Num());

			for (int i = 0; i < StringArray.Num(); i++)
			{
				OutArray[i] = FCString::Atod(*StringArray[i]);
			}
		}
		
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AStringToNumber, Out);
		DOREPLIFETIME(AStringToNumber, OutArray);
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
