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

	virtual void ServerProcess_Implementation(double DeltaTime) override
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

	UFUNCTION()
	UTexture2D* GetProductIcon()
	{
		if (!IsValid(Recipe)) return nullptr;

		auto Products = GetProducts();
		if (Products.Num() == 0 || !IsValid(Products[0].ItemClass)) return nullptr;

		return UFGItemDescriptor::GetBigIcon(Products[0].ItemClass);
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ABreakRecipe, Recipe)
	}
	
	UPROPERTY(Replicated, SaveGame)
	TSubclassOf<UFGRecipe> Recipe;
};
