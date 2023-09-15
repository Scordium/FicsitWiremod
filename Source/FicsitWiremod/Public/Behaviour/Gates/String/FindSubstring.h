// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "FindSubstring.generated.h"

UCLASS()
class FICSITWIREMOD_API AFindSubstring : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		auto Source = GetConnection(0).GetString();
		auto Substring = GetConnection(1).GetString();
		auto UseCase = GetConnection(2).GetBool();
		int StartIndex = GetConnection(3).GetFloat();

		Out = Source.Find(Substring, UseCase ? ESearchCase::CaseSensitive : ESearchCase::IgnoreCase, ESearchDir::FromStart, StartIndex);
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AFindSubstring, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	int Out;
};
