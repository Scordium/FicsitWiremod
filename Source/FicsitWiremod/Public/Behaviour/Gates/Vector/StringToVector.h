// 

#pragma once

#include "CoreMinimal.h"
#include "MultistateWiremodBuildable.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "StringToVector.generated.h"

UCLASS()
class FICSITWIREMOD_API AStringToVector : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		if (CurrentStateIndex == 0)
		{
			Success = Out.InitFromString(GetConnection(0).GetString());
		}
		else if (CurrentStateIndex == 1)
		{
			auto const StringArray = GetConnection(0).GetStringArray();

			OutArray.SetNum(StringArray.Num());

			Success = false;
			for (int i = 0; i < StringArray.Num(); i++)
			{
				auto Value = FVector();
				Success &= Value.InitFromString(StringArray[i]);
				OutArray[i] = Value;
			}
		}
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AStringToVector, Out)
		DOREPLIFETIME(AStringToVector, Success)
		DOREPLIFETIME(AStringToVector, OutArray)
	}

	virtual void OnStateSelected_Internal(int Index) override
	{
		Super::OnStateSelected_Internal(Index);

		Out = FVector();
		OutArray.Empty();
	}


	UPROPERTY(Replicated, SaveGame)
	FVector Out;

	UPROPERTY(Replicated, SaveGame)
	bool Success;

	UPROPERTY(Replicated, SaveGame)
	TArray<FVector> OutArray;
};
