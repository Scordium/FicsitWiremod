// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "BreakRecipe.generated.h"

UCLASS()
class FICSITWIREMOD_API ABreakRecipe : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:

	virtual void Process_Implementation(double DeltaTime) override
	{
		Recipe = GetConnection(0).GetRecipe();
	}


	UFUNCTION()
	FString GetRecipeName() { return IsValid(Recipe) ? UFGRecipe::GetRecipeName(Recipe).ToString() : "N/A"; }

	UFUNCTION()
	TArray<FItemAmount> GetIngredients() { return IsValid(Recipe) ? UFGRecipe::GetIngredients(Recipe) : TArray<FItemAmount>(); }

	UFUNCTION()
	TArray<FItemAmount> GetProducts() { return IsValid(Recipe) ? UFGRecipe::GetProducts(Recipe) : TArray<FItemAmount>(); }

	UFUNCTION()
	double GetManufacturingTime() { return IsValid(Recipe) ? UFGRecipe::GetManufacturingDuration(Recipe) : 0; }


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ABreakRecipe, Recipe)
	}
	
	UPROPERTY(Replicated, SaveGame)
	TSubclassOf<UFGRecipe> Recipe;
};
