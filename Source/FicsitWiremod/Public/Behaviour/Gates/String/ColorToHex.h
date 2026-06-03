// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ColorToHex.generated.h"

UCLASS()
class FICSITWIREMOD_API AColorToHex : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		if (CurrentStateIndex == 0)
		{
			Out = UFGBlueprintFunctionLibrary::LinearColorToHex(GetConnection(0).GetColor());
		}
		else if (CurrentStateIndex == 1)
		{
			auto const ColorArray = GetConnection(0).GetColorArray();

			OutArray.SetNum(ColorArray.Num());

			for (int i = 0; i < ColorArray.Num(); i++)
			{
				OutArray[i] = UFGBlueprintFunctionLibrary::LinearColorToHex(ColorArray[i]);
			}
		}
		
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AColorToHex, Out)
		DOREPLIFETIME(AColorToHex, OutArray)
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
