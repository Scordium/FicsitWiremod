// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Oscilloscope.generated.h"

#define DF_GET_X GetConnection(0).GetFloat()
#define DF_GET_Y GetConnection(1).GetFloat()
#define DF_LINE_COLOR GetConnection(2).GetColor()
#define DF_BACKGROUND_COLOR GetConnection(3).GetColor()
#define DF_MAX_POINTS GetConnection(4).GetFloat()
#define DF_NO_FADING GetConnection(5).GetBool()

UCLASS()
class FICSITWIREMOD_API AOscilloscope : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:

	AOscilloscope(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
	{
		ProcessLocally = true;
	}


	virtual void Process_Implementation(float DeltaTime) override
	{
		SetupWidgetVariables(DF_BACKGROUND_COLOR, DF_MAX_POINTS, DF_NO_FADING);
		DrawPoint(DF_GET_X, DF_GET_Y, DF_LINE_COLOR);
	}


	UFUNCTION(BlueprintImplementableEvent)
	void DrawPoint(float X, float Y, FLinearColor Color);

	UFUNCTION(BlueprintImplementableEvent)
	void SetupWidgetVariables(FLinearColor BackgroundColor, int MaxPoints, bool NoFading);
};
