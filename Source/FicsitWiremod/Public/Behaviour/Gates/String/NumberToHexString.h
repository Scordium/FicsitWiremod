// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NumberToHexString.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberToHexString : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Out = Convert(GetConnection(0).GetFloat());
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ANumberToHexString, Out);
	}


	UPROPERTY(EditInstanceOnly, Replicated, SaveGame)
	FString Out;

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
