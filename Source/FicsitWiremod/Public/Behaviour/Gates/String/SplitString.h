// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "SplitString.generated.h"

UCLASS()
class FICSITWIREMOD_API ASplitString : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(double DeltaTime) override
	{
		auto Source = GetConnection(0).GetString();
		auto Separator = GetConnection(1).GetString();
		auto CullEmpty = GetConnection(2).GetBool();
		
		Source.ParseIntoArray(Out, *Separator, CullEmpty);
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ASplitString, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	TArray<FString> Out;
};
