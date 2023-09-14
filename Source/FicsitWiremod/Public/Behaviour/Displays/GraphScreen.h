// 

#pragma once

#include "CoreMinimal.h"
#include "ScreenDrawSegment.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "GraphScreen.generated.h"

UCLASS(meta=(DeprecationMessage="Graph screens are now part of managed signs"))
class FICSITWIREMOD_API AGraphScreen : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(double DeltaTime) override
	{
		auto Points = GetConnection(0).GetFloatArray();
		auto LineColor = GetConnection(1).GetColor(DefaultLineColor);
		auto BackgroundColor = GetConnection(2).GetColor(DefaultBGColor);

		auto ParsedPoints = ResolveData(Points, LineColor);
		SetPointsAndBackground(ParsedPoints, BackgroundColor);
	}

	TArray<FScreenDrawSegment> ResolveData(TArray<double> Points, FLinearColor LineColor)
	{
		TArray<FScreenDrawSegment> Out;
		auto ScreenSize = GetScreenDimensions() / 2;
		
		double Min = FMath::Min(FMath::Min(Points), -ScreenSize.Y);
		double Max = FMath::Max(FMath::Max(Points), ScreenSize.Y);
		
		auto InputRange = FVector2D(Min, Max);
		auto OutputRange = FVector2D(-ScreenSize.Y, ScreenSize.Y) * .95;

		int PointsCount = Points.Num() - 1;
		for(int i = PointsCount; i >= 1; i--)
		{
			FVector2D Start;
			FVector2D End;
			
			Start.X =  (ScreenSize.X - ScreenSize.X * ((double) (i - 1) / PointsCount)) * 2;
			End.X = (ScreenSize.X - ScreenSize.X * ((double) i / PointsCount)) * 2;
			
			Start.Y = FMath::GetMappedRangeValueClamped(InputRange, OutputRange, Points[i - 1]) + ScreenSize.Y;
			End.Y = FMath::GetMappedRangeValueClamped(InputRange, OutputRange, Points[i]) + ScreenSize.Y;
			
			auto Segment = FScreenDrawSegment(Start, End, LineColor);
			Out.Add(Segment);
		}

		return Out;
	}

	UFUNCTION(BlueprintImplementableEvent)
	FVector2D GetScreenDimensions();
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetPointsAndBackground(const TArray<FScreenDrawSegment>& Segments, FLinearColor BackgroundColor);


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor DefaultLineColor = FLinearColor(0.783538, 0.291771, 0.059511); //Ficsit orange color

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor DefaultBGColor = FLinearColor::Black;
};
