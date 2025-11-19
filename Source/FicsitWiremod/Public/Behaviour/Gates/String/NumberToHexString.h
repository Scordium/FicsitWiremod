// 

#pragma once

#include "CoreMinimal.h"
#include "MultistateWiremodBuildable.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NumberToHexString.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberToHexString : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		if (CurrentStateIndex == 0)
		{
			Out = Convert(GetConnection(0).GetFloat());
		}
		else if (CurrentStateIndex == 1)
		{
			auto const NumberArray = GetConnection(0).GetFloatArray();

			OutArray.SetNum(NumberArray.Num());

			for (int i = 0; i < NumberArray.Num(); i++)
			{
				OutArray[i] = Convert(NumberArray[i]);
			}
		}
	
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANumberToHexString, Out);
		DOREPLIFETIME(ANumberToHexString, OutArray);
	}

	virtual void OnStateSelected_Internal(int Index) override
	{
		Super::OnStateSelected_Internal(Index);

		Out = "";
		OutArray.Empty();
	}


	UPROPERTY(EditInstanceOnly, Replicated, SaveGame)
	FString Out;

	UPROPERTY(EditInstanceOnly, Replicated, SaveGame)
	TArray<FString> OutArray;

private:

	TArray<FString> Chars
	{
		"0",
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",
		"A",
		"B",
		"C",
		"D",
		"E",
		"F",
	};

	FString Convert(int Number)
	{
		Number = FMath::Abs(Number);
		
		FString Output;

		while(Number > 0)
		{
			auto Value = Number%16;

			Output = Chars[Value] + Output;

			Number = (Number - Value)/16;
		}

		if(Output.IsEmpty()) Output = "0";
		return Output;
	}
};
