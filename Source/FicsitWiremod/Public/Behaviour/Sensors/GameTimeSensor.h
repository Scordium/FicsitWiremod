// 

#pragma once

#include "FGTimeSubsystem.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "GameTimeSensor.generated.h"

UCLASS(Blueprintable, BlueprintType)
class FICSITWIREMOD_API AGameTimeSensor : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		if(!SubsystemCache) SubsystemCache = AFGTimeOfDaySubsystem::Get(this->GetWorld());
		
		//If no time inputs are connected, return
		if(!IsConnected(0) && !IsConnected(1) && !IsConnected(2)) return;
		
		auto Hour = GetConnection(0).GetFloat();
		auto Minute = GetConnection(1).GetFloat();
		auto Second = GetConnection(2).GetFloat();

		auto FinalTime = Hour * 3600 + Minute * 60 + Second;
		if(SubsystemCache) SubsystemCache->SetDaySeconds(FinalTime);
	}

	UFUNCTION()
	int GetDays() const { return SubsystemCache ? SubsystemCache->GetPassedDays() : -1; }

	UFUNCTION()
	int GetHours() const { return SubsystemCache ? SubsystemCache->GetHours() : 0; }

	UFUNCTION()
	int GetMinutes() const { return SubsystemCache ? SubsystemCache->GetDayMinutes() : 0; }

	UFUNCTION()
	double GetSeconds() const { return SubsystemCache ? SubsystemCache->GetDaySeconds() : 0; }

	UFUNCTION()
	double GetDayPct() const { return SubsystemCache ? SubsystemCache->GetDayPct() : 0; }

	UFUNCTION()
	double GetNightPct() const { return SubsystemCache ? SubsystemCache->GetNightPct() : 0; }

	UFUNCTION()
	int GetCurrentMinute() const { return SubsystemCache ? SubsystemCache->GetMinutes() : 0; }

	UFUNCTION()
	double GetCurrentSecond() const { return SubsystemCache ? SubsystemCache->GetSeconds() : 0; }

	UFUNCTION()
	double GetDayLength() const { return SubsystemCache ? SubsystemCache->GetDaytimeSeconds() : 0; }

	UFUNCTION()
	double GetNightLength() const { return SubsystemCache ? SubsystemCache->GetNighttimeSeconds() : 0; }

	UFUNCTION()
	bool GetIsDay() const { return SubsystemCache ? SubsystemCache->IsDay() : false; }

	UFUNCTION()
	bool GetIsNight() const { return SubsystemCache ? SubsystemCache->IsNight() : false; }

	UPROPERTY()
	AFGTimeOfDaySubsystem* SubsystemCache;
};
