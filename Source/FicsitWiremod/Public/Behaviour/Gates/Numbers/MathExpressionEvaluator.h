// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "MathExpressionEvaluator.generated.h"

UCLASS()
class FICSITWIREMOD_API AMathExpressionEvaluator : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		FString Expression = WM_GetString(0);

		if(Expression == LastExpression) return;

		FText Out;
		Success = UFGBlueprintFunctionLibrary::EvaluateMathExpression(Expression, Out);
		Result = Out.ToString();
		LastExpression = Expression;
	}

	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AMathExpressionEvaluator, Result);
		DOREPLIFETIME(AMathExpressionEvaluator, Success);
	}


	//Expression cache to avoid reevaluating the input.
	UPROPERTY(VisibleInstanceOnly)
	FString LastExpression;
	
	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	FString Result;

	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	bool Success;
};
