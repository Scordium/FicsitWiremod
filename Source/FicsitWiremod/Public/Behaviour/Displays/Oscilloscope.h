// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Oscilloscope.generated.h"

#define DF_GET_X GetConnection(0).GetFloat()
#define DF_GET_Y GetConnection(1).GetFloat()
#define DF_LINE_COLOR GetConnection(2).GetColor(DefaultLineColor)
#define DF_BACKGROUND_COLOR GetConnection(3).GetColor(DefaultBGColor)
#define DF_MAX_POINTS GetConnection(4).GetFloat(200)
#define DF_NO_FADING GetConnection(5).GetBool()

UCLASS()
class FICSITWIREMOD_API AOscilloscope : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ClientProcess_Implementation(double DeltaTime) override
	{
		SetupWidgetVariables(DF_BACKGROUND_COLOR, DF_MAX_POINTS, DF_NO_FADING);
		DrawPoint(DF_GET_X, DF_GET_Y, DF_LINE_COLOR);
	}


	UFUNCTION(BlueprintImplementableEvent)
	void DrawPoint(double X, double Y, FLinearColor Color);

	UFUNCTION(BlueprintImplementableEvent)
	void SetupWidgetVariables(FLinearColor BackgroundColor, int MaxPoints, bool NoFading);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor DefaultLineColor = FLinearColor(0.783538, 0.291771, 0.059511); //Ficsit orange color

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor DefaultBGColor = FLinearColor::Black;
};
