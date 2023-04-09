// 

#pragma once

#include "CoreMinimal.h"
#include "AbstractInstanceManager.h"
#include "Equipment/FGEquipment.h"
#include "WiremodBaseTool.generated.h"

UCLASS()
class FICSITWIREMOD_API AWiremodBaseTool : public AFGEquipment
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	AActor* GetTargetLookAt(float TraceDistance, TEnumAsByte<ETraceTypeQuery> Channel, FVector& Location, bool& Success)
	{
		auto camera = UGameplayStatics::GetPlayerCameraManager(this, 0);

		FVector Start = camera->GetCameraLocation();
		FVector End = camera->GetCameraLocation() + camera->GetActorForwardVector() * TraceDistance;
		FHitResult Result;

		Success = UKismetSystemLibrary::LineTraceSingle(this, Start, End, Channel, false, TArray<AActor*>(), EDrawDebugTrace::None, Result, true);
		return UWiremodUtils::GetActualHitTarget(Result, Location);
	}


protected:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ShowNotification(const FText& Text);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HideHudHints();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DisplayHudHints();


	FORCEINLINE bool IsLocallyControlled() const { return GetInstigator()->IsLocallyControlled(); }
};
