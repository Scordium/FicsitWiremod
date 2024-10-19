#pragma once

#include "CoreMinimal.h"
#include "WiremodUtils.h"
#include "Equipment/FGEquipment.h"
#include "Utility/CircuitryInputMappings.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "UI/FGGameUI.h"
#include "WiremodBaseTool.generated.h"

UCLASS()
class FICSITWIREMOD_API AWiremodBaseTool : public AFGEquipment
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SetOutline(AActor* Object, EOutlineColor Color = EOutlineColor::OC_USABLE);

	UPROPERTY()
	AActor* CurrentOutlineActor;

	UFUNCTION(BlueprintCallable)
	AActor* GetTargetLookAt(double TraceDistance, TEnumAsByte<ETraceTypeQuery> Channel, FVector& Location, bool& Success);


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
		for(auto& InputsContext : InputsContexts)
		{
			if(InputsContext.Value->IsA<T>()) return Cast<T>(InputsContext.Value);
		}

		return nullptr;
	}
	
	void InjectMappings();
	void EjectMappings();
	
	UFUNCTION(BlueprintImplementableEvent) void ApplyScrollWheelPatch();
	UFUNCTION(BlueprintImplementableEvent) void RemoveScrollWheelPatch();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MappingContextPriority = 10000;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, UFGInputMappingContext*> InputsContexts;

protected:

	UFUNCTION(BlueprintCallable)
	void ShowNotification(const FText& Text);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HideHudHints();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DisplayHudHints();


	FORCEINLINE bool IsLocallyControlled() const { return GetInstigator() ? GetInstigator()->IsLocallyControlled() : false; }
};
