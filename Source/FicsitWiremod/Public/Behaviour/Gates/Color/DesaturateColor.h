// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "DesaturateColor.generated.h"

UCLASS()
class FICSITWIREMOD_API ADesaturateColor : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(double DeltaTime) override
	{
		auto Color = GetConnection(0).GetColor();
		auto Desaturation = GetConnection(1).GetFloat();

		Out = Color.Desaturate(Desaturation);
	}
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ADesaturateColor, Out)
	}
	
	UPROPERTY(Replicated, SaveGame)
	FLinearColor Out;
};
