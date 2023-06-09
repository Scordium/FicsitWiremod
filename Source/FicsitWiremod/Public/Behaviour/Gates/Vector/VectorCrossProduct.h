// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "VectorCrossProduct.generated.h"

UCLASS()
class FICSITWIREMOD_API AVectorCrossProduct : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		auto const V1 = GetConnection(0).GetVector();
		auto const V2 = GetConnection(1).GetVector();
		Out = FVector::CrossProduct(V1, V2);
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AVectorCrossProduct, Out)
	}


	UPROPERTY(Replicated, SaveGame)
	FVector Out;
};
