// 

#pragma once

#include "CoreMinimal.h"
#include "MultistateWiremodBuildable.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "HexToColor.generated.h"

UCLASS()
class FICSITWIREMOD_API AHexToColor : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		if (CurrentStateIndex == 0)
		{
			Out = UFGBlueprintFunctionLibrary::HexToLinearColor(GetConnection(0).GetString());	
		}
		else if (CurrentStateIndex == 1)
		{
			TArray<FLinearColor> Array;
			auto Strings = GetConnection(0).GetStringArray();

			for (const auto& String : Strings)
			{
				Array.Add(UFGBlueprintFunctionLibrary::HexToLinearColor(String));
			}

			OutArray = Array;
		}
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AHexToColor, Out)
		DOREPLIFETIME(AHexToColor, OutArray)
	}

	virtual void OnStateSelected_Internal(int Index) override
	{
		Super::OnStateSelected_Internal(Index);
		Out = FLinearColor();
		OutArray.Empty();
	}


	UPROPERTY(Replicated, SaveGame)
	FLinearColor Out;

	UPROPERTY(Replicated, SaveGame)
	TArray<FLinearColor> OutArray;
};
