// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "NumberIsInRange.generated.h"

UCLASS()
class FICSITWIREMOD_API ANumberIsInRange : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		const auto Value = GetConnection(0).GetFloat();
		const auto Min = GetConnection(1).GetFloat();
		const auto Max = GetConnection(2).GetFloat();

		const bool InclusiveMin = GetConnection(3).GetBool();
		const bool InclusiveMax = GetConnection(4).GetBool();
		
		const bool IsGreaterThanMin = InclusiveMin ? (Value >= Min) : (Value > Min);
		const bool IsLesserThanMax = InclusiveMax ? (Value <= Max) : (Value < Max);

		if(!IsGreaterThanMin)
		{
			Below = true;
			Out = false;
			Above = false;
		}
		else if(!IsLesserThanMax)
		{
			Below = false;
			Out = false;
			Above = true;
		}
		else
		{
			Below = false;
			Out = IsGreaterThanMin && IsLesserThanMax;
			Above = false;
		}
	}
    
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
		DOREPLIFETIME(ANumberIsInRange, Out);
		DOREPLIFETIME(ANumberIsInRange, Below);
		DOREPLIFETIME(ANumberIsInRange, Above);
	}
    
    
	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	bool Out;

	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	bool Below;
	
	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	bool Above;
};
