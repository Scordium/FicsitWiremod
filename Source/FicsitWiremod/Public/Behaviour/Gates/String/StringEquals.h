// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "StringEquals.generated.h"

UCLASS()
class FICSITWIREMOD_API AStringEquals : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:

	virtual void Process_Implementation(float DeltaTime) override
	{
		auto A = GetConnection(0).GetString();
		auto B = GetConnection(1).GetString();
		auto CaseSensitive = GetConnection(2).GetBool() ? ESearchCase::CaseSensitive : ESearchCase::IgnoreCase;

		Out = A.Equals(B, CaseSensitive);
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AStringEquals, Out)
	}


	UPROPERTY(Replicated, SaveGame)
	bool Out;
};
