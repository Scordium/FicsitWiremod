// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ReplaceString.generated.h"

UCLASS()
class FICSITWIREMOD_API AReplaceString : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(double DeltaTime) override
	{
		auto Source = GetConnection(0).GetString();
		auto Target = GetConnection(1).GetString();
		auto Replacement = GetConnection(2).GetString();
		auto CaseSensitive = GetConnection(3).GetBool() ? ESearchCase::CaseSensitive : ESearchCase::IgnoreCase;

		Out = Source.Replace(*Target, *Replacement, CaseSensitive);
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AReplaceString, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	FString Out;
};
