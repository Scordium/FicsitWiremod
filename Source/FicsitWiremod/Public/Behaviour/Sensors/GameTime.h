// 

#pragma once

#include "CoreMinimal.h"
#include "FGTimeSubsystem.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "GameTime.generated.h"

UCLASS()
class FICSITWIREMOD_API AGameTime : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		//If no time inputs are connected, return
		if(!IsConnected(0) && !IsConnected(1) && !IsConnected(2)) return;
		
		auto System = AFGTimeOfDaySubsystem::Get(this);

		auto Hour = GetConnection(0).GetFloat();
		auto Minute = GetConnection(1).GetFloat();
		auto Second = GetConnection(2).GetFloat();

		auto FinalTime = Hour * 3600 + Minute * 60 + Second;
		System->SetDaySeconds(FinalTime);
	}

	UFUNCTION()
	int GetDays() { return AFGTimeOfDaySubsystem::Get(this)->GetPassedDays(); }

	UFUNCTION()
	int GetHours() { return AFGTimeOfDaySubsystem::Get(this)->GetHours(); }

	UFUNCTION()
	int GetMinutes() { return AFGTimeOfDaySubsystem::Get(this)->GetDayMinutes(); }

	UFUNCTION()
	double GetSeconds() { return AFGTimeOfDaySubsystem::Get(this)->GetDaySeconds(); }

	UFUNCTION()
	double GetDayPct() { return AFGTimeOfDaySubsystem::Get(this)->GetDayPct(); }

	UFUNCTION()
	double GetNightPct() { return AFGTimeOfDaySubsystem::Get(this)->GetNightPct(); }

	UFUNCTION()
	int GetCurrentMinute() { return AFGTimeOfDaySubsystem::Get(this)->GetMinutes(); }

	UFUNCTION()
	double GetCurrentSecond() { return AFGTimeOfDaySubsystem::Get(this)->GetSeconds(); }

	UFUNCTION()
	double GetDayLength() { return AFGTimeOfDaySubsystem::Get(this)->GetDaytimeSeconds(); }

	UFUNCTION()
	double GetNightLength() { return AFGTimeOfDaySubsystem::Get(this)->GetNighttimeSeconds(); }

	UFUNCTION()
	bool GetIsDay() { return AFGTimeOfDaySubsystem::Get(this)->IsDay(); }

	UFUNCTION()
	bool GetIsNight() { return AFGTimeOfDaySubsystem::Get(this)->IsNight(); }
};
