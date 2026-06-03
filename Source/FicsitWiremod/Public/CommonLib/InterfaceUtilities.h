#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InterfaceUtilities.generated.h"


UCLASS()
class UInterfaceUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void SetOpacityFromTexture(UWidgetComponent* WidgetComponent, float Opacity) { if (WidgetComponent) WidgetComponent->SetOpacityFromTexture(Opacity); }
};
