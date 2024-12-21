// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultistateWiremodBuildable.h"
#include "Kismet/KismetTextLibrary.h"

#include "FormatNumber.generated.h"

UCLASS()
class FICSITWIREMOD_API AFormatNumber : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		switch (CurrentStateIndex)
		{
			//Default mode - just convert the number to string
		default:
			{
				Out = FString::SanitizeFloat(GetConnection(0).GetFloat());
				break;
			}
			
			//Format number with configurable fractions
		case 1:
			{
				auto Value = GetConnection(0).GetFloat();
				int MinFractions = GetConnection(1).GetFloat();
				int MaxFractions = GetConnection(2).GetFloat(2);

				if (MaxFractions == 0)
				{
					Out = FString::FromInt(Value);
					break;
				}

				Out = UKismetTextLibrary::Conv_DoubleToText(
					Value,
					HalfToZero,
					false,
					false,
					1,
					324,
					MinFractions,
					MaxFractions).ToString();

				break;
			}

			//Format as percentage
		case 2:
			{
				auto Value = GetConnection(0).GetFloat();
				auto MinValue = GetConnection(1).GetFloat(0);
				auto MaxValue = GetConnection(2).GetFloat(1);

				auto RemappedValue = FMath::GetMappedRangeValueUnclamped(FVector2D(MinValue, MaxValue), FVector2D(0, 100), Value);

				int MinFractions = GetConnection(3).GetFloat();
				int MaxFractions = GetConnection(4).GetFloat(2);

				if (MaxFractions == 0)
				{
					Out = FString::FromInt(RemappedValue) + "%";
					break;
				}

				Out = UKismetTextLibrary::Conv_DoubleToText(
					RemappedValue,
					HalfToZero,
					false,
					false,
					1,
					324,
					MinFractions,
					MaxFractions).ToString() + "%";
				break;
			}

			//Format as currency
		case 3:
			{
				auto Value = GetConnection(0).GetFloat();
				auto CurrencyCode = GetConnection(1).GetString();
				
				Out = FText::AsCurrency(Value, CurrencyCode).ToString();
				break;
			}

			//Format to readable
		case 4:
			{
				Out = FMath::FormatIntToHumanReadable(GetConnection(0).GetFloat());
				break;
			}
		}
	}

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ThisClass, Out)
	}
	

	UPROPERTY(SaveGame,Replicated)
	FString Out;
};


