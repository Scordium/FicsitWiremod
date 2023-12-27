// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "MakeSplitterRule.generated.h"

UCLASS()
class FICSITWIREMOD_API AMakeSplitterRule : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		auto Class = GetConnection(0).GetItemDescriptor();
		int Output = GetConnection(1).GetFloat();

		Out.ItemClass = Class;
		Out.OutputIndex = Output;
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AMakeSplitterRule, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	FSplitterSortRule Out;
};
