// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "StringToBool.generated.h"

UCLASS()
class FICSITWIREMOD_API AStringToBool : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(double DeltaTime) override
	{
		auto const String = GetConnection(0).GetString();
		Out = String == "1" || String.Equals("Yes", ESearchCase::IgnoreCase) || String.Equals("True", ESearchCase::IgnoreCase);
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AStringToBool, Out)
	}
	
	UPROPERTY(Replicated, SaveGame)
	bool Out;
};
