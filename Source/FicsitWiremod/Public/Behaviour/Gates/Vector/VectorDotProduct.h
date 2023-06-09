// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "VectorDotProduct.generated.h"

UCLASS()
class FICSITWIREMOD_API AVectorDotProduct : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		auto const V1 = GetConnection(0).GetVector();
		auto const V2 = GetConnection(1).GetVector();
		Out = FVector::DotProduct(V1, V2);
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AVectorDotProduct, Out)
	}


	UPROPERTY(Replicated, SaveGame)
	float Out;
};
