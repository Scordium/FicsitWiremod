// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Notificator.generated.h"

UCLASS()
class FICSITWIREMOD_API ANotificator : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		auto Fire = GetConnection(0).GetBool();

		if(Fire)
		{
			if(HasFiredLastTick) return;

			auto Title = GetConnection(1).GetString("Circuitry");
			auto Description = GetConnection(2).GetString("Notification fired!");
			auto Time = GetConnection(3).GetFloat(5);

			OnNotificationReceived(Title, Description, Time);
			HasFiredLastTick = true;
		}
		else HasFiredLastTick = false;
	}

	UFUNCTION(NetMulticast, Reliable)
	void OnNotificationReceived(const FString& Title, const FString& Description, double Time);
	void OnNotificationReceived_Implementation(const FString& Title, const FString& Description, double Time) { ShowNotification(Title, Description, Time); }

	UFUNCTION(BlueprintImplementableEvent)
	void ShowNotification(const FString& Title, const FString& Description, double Time);



	UPROPERTY()
	bool HasFiredLastTick;
};
