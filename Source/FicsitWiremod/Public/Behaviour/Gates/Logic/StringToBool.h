// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "StringToBool.generated.h"

UCLASS()
class FICSITWIREMOD_API AStringToBool : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{

		if (CurrentStateIndex == 0)
		{
			auto const String = GetConnection(0).GetString();
			Out = String == "1" || String.Equals("Yes", ESearchCase::IgnoreCase) || String.Equals("True", ESearchCase::IgnoreCase);
		}
		else if (CurrentStateIndex == 1)
		{
			auto const StringArray = GetConnection(0).GetStringArray();

			TArray<bool> Array;
			for (const auto& String : StringArray)
			{
				auto Value = String == "1" || String.Equals("Yes", ESearchCase::IgnoreCase) || String.Equals("True", ESearchCase::IgnoreCase);
				
				Array.Add(Value);
			}

			OutArray = Array;
		}
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AStringToBool, Out)
		DOREPLIFETIME(AStringToBool, OutArray)
	}

	virtual void OnStateSelected_Internal(int Index) override
	{
		Super::OnStateSelected_Internal(Index);

		Out = false;
		OutArray.Empty();
	}
	
	UPROPERTY(Replicated, SaveGame)
	bool Out;

	UPROPERTY(Replicated, SaveGame)
	TArray<bool> OutArray;
};
