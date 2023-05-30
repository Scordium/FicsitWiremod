// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "LerpColor.generated.h"

UCLASS()
class FICSITWIREMOD_API ALerpColor : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		//Default mode - lerp from A to B
		if(CurrentStateIndex == 0)
		{
			auto A = WM_GetColor(0);
			auto B = WM_GetColor(1);
			auto Alpha = WM_GetFloat(2);

			Out = UKismetMathLibrary::LinearColorLerp(A, B, Alpha);
		}
		//Alt mode - lerp in color array
		else if (CurrentStateIndex == 1)
		{
			auto Colors = WM::GetColorArray(GetConnection(0));
			auto NumColors = Colors.Num();
			
			if(NumColors == 0)
			{
				Out = FLinearColor::Black;
			}
			else if(NumColors == 1)
			{
				Out = Colors[0];
			}
			else
			{
				auto Alpha = WM_GetFloat(1);
				auto AlphaPerIndex = 1.f / Colors.Num();

				//This is so fucking stupid
				auto AIndex = FMath::Clamp((int)(Alpha / AlphaPerIndex - 1), 0, NumColors - 2);
				auto BIndex = FMath::Clamp((int)(Alpha / AlphaPerIndex + 1), 1, NumColors - 1);
				
				auto A = Colors[AIndex];
				auto B = Colors[BIndex];

				Out = UKismetMathLibrary::LinearColorLerp(A, B, Alpha);
			}
			
			
		}
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ALerpColor, Out)
	}

	UPROPERTY(Replicated)
	FLinearColor Out;
};
