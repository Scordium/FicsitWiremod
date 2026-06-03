// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "StringToColor.generated.h"

UCLASS()
class FICSITWIREMOD_API AStringToColor : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		if (CurrentStateIndex == 0)
		{
			Out.InitFromString(GetConnection(0).GetString());	
		}
		else if (CurrentStateIndex == 1)
		{
			auto Strings = GetConnection(0).GetStringArray();
			TArray<FLinearColor> Array;

			for (const auto& String : Strings)
			{
				FLinearColor Color = FLinearColor();
				Color.InitFromString(String);
				Array.Add(Color);
			}

			OutArray = Array;
		}
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AStringToColor, Out)
		DOREPLIFETIME(AStringToColor, OutArray)
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
