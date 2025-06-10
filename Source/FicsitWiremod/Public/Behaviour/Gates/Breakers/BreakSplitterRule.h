// 

#pragma once

#include "CoreMinimal.h"
#include "FGWiremodBuildable.h"
#include "BreakSplitterRule.generated.h"

UCLASS()
class FICSITWIREMOD_API ABreakSplitterRule : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		ValueCache = GetConnection(0).GetSplitterRule();
	}

	UFUNCTION()
	TSubclassOf<UFGItemDescriptor> GetDescriptor() { return ValueCache.ItemClass; }

	UFUNCTION()
	int GetOutputIndex() { return ValueCache.OutputIndex; }

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(ABreakSplitterRule, ValueCache);
	}


	UPROPERTY(SaveGame, Replicated)
	FSplitterSortRule ValueCache;
};
