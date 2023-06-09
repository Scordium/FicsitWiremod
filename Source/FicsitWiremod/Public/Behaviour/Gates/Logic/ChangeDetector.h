// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ChangeDetector.generated.h"

UCLASS()
class FICSITWIREMOD_API AChangeDetector : public AFGWiremodBuildable
{
	GENERATED_BODY()
	
public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		bool Value = GetConnection(0).GetBool();

		if(Value != LastValue)
		{
			Out = true;
			LastValue = Value;
		}
		else Out = false;
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AChangeDetector, Out);
	};


	UPROPERTY(Replicated, VisibleInstanceOnly)
	bool Out;
	
	UPROPERTY(VisibleInstanceOnly)
	bool LastValue;
};
