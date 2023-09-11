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
	virtual void Process_Implementation(double DeltaTime) override
	{
		//Default mode - lerp from A to B
		if(CurrentStateIndex == 0)
		{
			auto A = GetConnection(0).GetColor();
			auto B = GetConnection(1).GetColor();
			auto Alpha = GetConnection(2).GetFloat();

			Out = UKismetMathLibrary::LinearColorLerp(A, B, Alpha);
		}
		//Alt mode - lerp in color array
		else if (CurrentStateIndex == 1)
		{
			auto Colors = GetConnection(0).GetColorArray();
			auto NumColors = Colors.Num();

			switch (NumColors)
			{
			case 0: Out = FLinearColor::Black; break;
			case 1: Out = Colors[0]; break;
			default:
				auto Alpha = GetConnection(1).GetFloat();
				auto MagicNumber = Alpha * (NumColors - 1);
				
				auto A = Colors[FMath::Max(MagicNumber, 0)];
				auto B = Colors[FMath::Min(MagicNumber + 1, NumColors - 1)];
				
				// //This is so fucking stupid
				// auto AIndex = FMath::Clamp((int)(Alpha / AlphaPerColor), 0, NumColors - 1);
				// auto BIndex = FMath::Clamp((int)(Alpha / AlphaPerColor + 1), 1, NumColors - 1);
				//
				// auto A = Colors[AIndex];
				// auto B = Colors[BIndex];
				
				Out = UKismetMathLibrary::LinearColorLerp(A, B, FMath::Fractional(MagicNumber));
				break;
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
