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
	virtual void Process_Implementation(float DeltaTime) override
	{
		auto Fire = GetConnection(0).GetBool();

		if(Fire)
		{
			if(HasFiredLastTick) return;

			auto Title = GetConnection(1).GetString("Circuitry");
			auto Description = GetConnection(2).GetString("Notification fired!");
			auto Time = GetConnection(3).GetFloat(5);

			ShowNotification(Title, Description, Time);
			HasFiredLastTick = true;
		}
		else HasFiredLastTick = false;
	}

	UFUNCTION(BlueprintImplementableEvent)
	void ShowNotification(const FString& Title, const FString& Description, float Time);



	UPROPERTY()
	bool HasFiredLastTick;
};
