// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "CommonLib/DynamicValues/CCDynamicValueUtils.h"
#include "ArrayFind.generated.h"

UCLASS()
class FICSITWIREMOD_API AArrayFind : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		ArrayCache = UCCDynamicValueUtils::FromValue(GetConnection(0), ArrayCache);
		if(ArrayCache) SetInputType(1, UConnectionTypeFunctions::ArrayToBase(ArrayCache->ConnectionType));
		
		//Default mode - find first match
		if(CurrentStateIndex == 0)
		{
			if(auto Array = Cast<UCCArrayValueBase>(ArrayCache))
				Out = Array->FindFirst(GetConnection(1));
			else Out = -1;
		}
		//Alt mode - find all matches
		else
		{
			if(auto Array = Cast<UCCArrayValueBase>(ArrayCache))
				OutArray = Array->FindAll(GetConnection(1));
			else OutArray = TArray<double>();
		}
	}

	virtual void OnStateSelected_Internal(int Index) override
	{
		Super::OnStateSelected_Internal(Index);
		Out = -1;
		OutArray = TArray<double>();
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AArrayFind, Out)
		DOREPLIFETIME(AArrayFind, OutArray)
	}


	UPROPERTY()
	UCCDynamicValueBase* ArrayCache;
	
	UPROPERTY(SaveGame, Replicated, BlueprintReadOnly)
	int Out;

	UPROPERTY(SaveGame, Replicated, BlueprintReadOnly)
	TArray<double> OutArray;
};
