// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "GameTime.generated.h"

UCLASS()
class FICSITWIREMOD_API AGameTime : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override{ SetTime(); }

	void SetTime()
	{
		//If no time inputs are connected, return
		if(!IsConnected(0) && !IsConnected(1) && !IsConnected(2)) return;
		
		auto System = AFGTimeOfDaySubsystem::Get(this);

		auto Hour = WM_GetFloat(0);
		auto Minute = WM_GetFloat(1);
		auto Second = WM_GetFloat(2);

		float FinalTime = Hour * 3600 + Minute * 60 + Second;
		System->SetDaySeconds(FinalTime);
	}

	UFUNCTION()
	int GetHours() { return AFGTimeOfDaySubsystem::Get(this)->GetHours(); }

	UFUNCTION()
	int GetMinutes() { return AFGTimeOfDaySubsystem::Get(this)->GetDayMinutes(); }

	UFUNCTION()
	float GetSeconds() { return AFGTimeOfDaySubsystem::Get(this)->GetDaySeconds(); }

	UFUNCTION()
	float GetDayPct() { return AFGTimeOfDaySubsystem::Get(this)->GetDayPct(); }

	UFUNCTION()
	float GetNightPct() { return AFGTimeOfDaySubsystem::Get(this)->GetNightPct(); }

	UFUNCTION()
	int GetCurrentMinute() { return AFGTimeOfDaySubsystem::Get(this)->GetMinutes(); }

	UFUNCTION()
	float GetCurrentSecond() { return AFGTimeOfDaySubsystem::Get(this)->GetSeconds(); }

	UFUNCTION()
	float GetDayLength() { return AFGTimeOfDaySubsystem::Get(this)->GetDaytimeSeconds(); }

	UFUNCTION()
	float GetNightLength() { return AFGTimeOfDaySubsystem::Get(this)->GetNighttimeSeconds(); }

	UFUNCTION()
	bool GetIsDay() { return AFGTimeOfDaySubsystem::Get(this)->IsDay(); }

	UFUNCTION()
	bool GetIsNight() { return AFGTimeOfDaySubsystem::Get(this)->IsNight(); }
};
