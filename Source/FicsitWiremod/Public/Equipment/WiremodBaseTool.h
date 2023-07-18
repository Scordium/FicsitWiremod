// 

#pragma once

#include "WiremodUtils.h"
#include "CoreMinimal.h"
#include "Equipment/FGEquipment.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/CircuitryInputMappings.h"
#include "EnhancedInputSubsystems.h"
#include "FGPlayerController.h"
#include "WiremodBaseTool.generated.h"

UCLASS()
class FICSITWIREMOD_API AWiremodBaseTool : public AFGEquipment
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	AActor* GetTargetLookAt(double TraceDistance, TEnumAsByte<ETraceTypeQuery> Channel, FVector& Location, bool& Success)
	{
		auto camera = UGameplayStatics::GetPlayerCameraManager(this, 0);

		FVector Start = camera->GetCameraLocation();
		FVector End = camera->GetCameraLocation() + camera->GetActorForwardVector() * TraceDistance;
		FHitResult Result;

		Success = UKismetSystemLibrary::LineTraceSingle(this, Start, End, Channel, false, TArray<AActor*>(), EDrawDebugTrace::None, Result, true);
		return UWiremodUtils::GetActualHitTarget(Result, Location);
	}

	
	virtual void WasEquipped_Implementation() override
	{
		Super::WasEquipped_Implementation();
		if(IsLocallyControlled()) InjectMappings();
	}

	virtual void WasUnEquipped_Implementation() override
	{
		Super::WasUnEquipped_Implementation();
		if(IsLocallyControlled()) EjectMappings();
	}

	virtual void OnInteractWidgetAddedOrRemoved(UFGInteractWidget* widget, bool added) override
	{
		Super::OnInteractWidgetAddedOrRemoved(widget, added);
		if(added) EjectMappings();
		else InjectMappings();
	}

#if !WITH_EDITOR
	virtual void Destroyed() override
	{
		if(IsLocallyControlled()) EjectMappings();
		Super::Destroyed();
	}
#endif
	

	template<class T>
	T* GetFirstContextOfType()
	{
		for(auto InputsContext : InputsContexts)
		{
			if(InputsContext.Value->IsA<T>()) return Cast<T>(InputsContext.Value);
		}

		return nullptr;
	}
		

	void InjectMappings()
	{
		auto Player = Cast<AFGPlayerController>(GetInstigatorController());
		auto EnhancedInputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Player->GetLocalPlayer());
		for(auto InputsContext : InputsContexts) EnhancedInputSystem->AddMappingContext(InputsContext.Value, MappingContextPriority);
	}

	void EjectMappings()
	{
		auto Player = Cast<AFGPlayerController>(GetInstigatorController());
		auto EnhancedInputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Player->GetLocalPlayer());
		for(auto InputsContext : InputsContexts) EnhancedInputSystem->RemoveMappingContext(InputsContext.Value);
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MappingContextPriority = 10000;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, UFGInputMappingContext*> InputsContexts;

protected:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ShowNotification(const FText& Text);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HideHudHints();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DisplayHudHints();


	FORCEINLINE bool IsLocallyControlled() const { return GetInstigator() ? GetInstigator()->IsLocallyControlled() : false; }
};
