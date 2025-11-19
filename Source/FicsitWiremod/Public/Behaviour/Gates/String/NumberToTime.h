// 

#pragma once

#include "CoreMinimal.h"
#include "MultistateWiremodBuildable.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NumberToTime.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberToTime : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		if (CurrentStateIndex == 0)
		{
			Out = UFGBlueprintFunctionLibrary::SecondsToTimeString(GetConnection(0).GetFloat());
		}
		else if (CurrentStateIndex == 1)
		{
			auto const NumberArray = GetConnection(0).GetFloatArray();

			OutArray.SetNum(NumberArray.Num());

			for (int i = 0; i < NumberArray.Num(); i++)
			{
				OutArray[i] = UFGBlueprintFunctionLibrary::SecondsToTimeString(NumberArray[i]);
			}
		}
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANumberToTime, Out)
		DOREPLIFETIME(ANumberToTime, OutArray)
	}

	virtual void OnStateSelected_Internal(int Index) override
	{
		Super::OnStateSelected_Internal(Index);

		Out = "";
		OutArray.Empty();
	}

	UPROPERTY(Replicated, SaveGame)
	FString Out;

	UPROPERTY(Replicated, SaveGame)
	TArray<FString> OutArray;
};
