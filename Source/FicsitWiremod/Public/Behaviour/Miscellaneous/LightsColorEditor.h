// 

#pragma once

#include "CoreMinimal.h"
#include "FGGameState.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "LightsColorEditor.generated.h"

UCLASS()
class FICSITWIREMOD_API ALightsColorEditor : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		auto GameState = Cast<AFGGameState>(UGameplayStatics::GetGameState(this));
		
		for(int i = 0; i < InputConnections.Num(); i++)
		{
			if(IsConnected(i))
				GameState->Server_SetBuildableLightColorSlot(i, GetConnection(i).GetColor());
		}
	}

	UFUNCTION()
	TArray<FLinearColor> GetBuildableLightColors()
	{
		FConnectionData Data(UGameplayStatics::GetGameState(this), "mBuildableLightColorSlots");
		return Data.GetColorArray();
	}
};
