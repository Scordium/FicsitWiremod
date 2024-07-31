#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ItemHologram.generated.h"

UCLASS()
class FICSITWIREMOD_API AItemHologram : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ClientProcess_Implementation(double DeltaTime) override
	{
		auto Descriptor = GetConnection(0).GetItemDescriptor();

		if(CurrentDescriptor != Descriptor)
		{
			CurrentDescriptor = Descriptor;
			UpdateMesh(Descriptor);
		}

		auto Position = GetConnection(1).GetVector() + FVector(0, 0, 50);
		auto Rotation = GetConnection(2).GetVector();
		auto Scale = GetConnection(3).GetVector(FVector(1));

		UpdateTransform(Position, FRotator::MakeFromEuler(Rotation), Scale);
	}


	UFUNCTION(BlueprintImplementableEvent)
	void UpdateTransform(const FVector& Position, const FRotator& Rotation, const FVector& Scale);
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateMesh(TSubclassOf<UFGItemDescriptor> Descriptor);

	UPROPERTY()
	TSubclassOf<UFGItemDescriptor> CurrentDescriptor;
};
