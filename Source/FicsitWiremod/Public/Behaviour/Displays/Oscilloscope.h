// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Oscilloscope.generated.h"

#define DF_GET_X WM_GetFloat(0)
#define DF_GET_Y WM_GetFloat(1)
#define DF_LINE_COLOR WM_GetColor(2)
#define DF_BACKGROUND_COLOR WM_GetColor(3)
#define DF_MAX_POINTS WM_GetInt(4)
#define DF_NO_FADING WM_GetBool(5)

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
