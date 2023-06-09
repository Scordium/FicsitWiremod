// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "BreakVector.generated.h"

UCLASS()
class FICSITWIREMOD_API ABreakVector : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		auto Val = GetConnection(0).GetVector();

		Out_X = Val.X;
		Out_Y = Val.Y;
		Out_Z = Val.Z;
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ABreakVector, Out_X)
		DOREPLIFETIME(ABreakVector, Out_Y)
		DOREPLIFETIME(ABreakVector, Out_Z)
	}


	UPROPERTY(Replicated, SaveGame)
	float Out_X;
	
	UPROPERTY(Replicated, SaveGame)
	float Out_Y;
	
	UPROPERTY(Replicated, SaveGame)
	float Out_Z;
};
