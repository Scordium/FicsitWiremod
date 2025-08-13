﻿#include "WiremodBaseTool.h"

#include "EnhancedInputSubsystems.h"


void AWiremodBaseTool::SetOutline(AActor* Object, EOutlineColor Color)
{
	auto Outline = GetInstigatorCharacter()->GetOutline();

	if (CurrentOutlineActor) Outline->HideOutline(CurrentOutlineActor);
		
	if(Object) Outline->ShowOutline(Object, Color);
	CurrentOutlineActor = Object;
}

AActor* AWiremodBaseTool::GetTargetLookAt(double TraceDistance, TEnumAsByte<ETraceTypeQuery> Channel, FVector& Location,
	bool& Success)
{
	auto camera = UGameplayStatics::GetPlayerCameraManager(this, 0);

	FVector Start = camera->GetCameraLocation();
	FVector End = camera->GetCameraLocation() + camera->GetActorForwardVector() * TraceDistance;
	FHitResult Result;

	TArray<AActor*> RaycastIgnoredActors =
	{
		GetInstigatorCharacter() //Ignoring player character holding the tool (self-collission)
	};

	Success = UKismetSystemLibrary::LineTraceSingle(this, Start, End, Channel, false, RaycastIgnoredActors, EDrawDebugTrace::None, Result, true);
	return UWiremodUtils::GetActualHitTarget(Result, Location);
}

void AWiremodBaseTool::InjectMappings()
{
	auto Player = Cast<AFGPlayerController>(GetInstigatorController());
	if(!Player)
	{
		ACircuitryLogger::DispatchErrorEvent("[CIRCUITRY TOOL KEYBIND HANDLER] Somehow, the player reference was null when injecting mappings.");
		return;
	}
		
	auto LocalPlayer = Player->GetLocalPlayer();
	if(!LocalPlayer)
	{
		ACircuitryLogger::DispatchErrorEvent("[CIRCUITRY TOOL KEYBIND HANDLER] Somehow, the local player reference was null when injecting mappings.");
		return;
	}
		
	auto EnhancedInputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if(!EnhancedInputSystem)
	{
		ACircuitryLogger::DispatchErrorEvent("[KEYBIND HANDLER] Input System is nullptr.");
		return;
	}
		
	for(auto& InputsContext : InputsContexts) EnhancedInputSystem->AddMappingContext(InputsContext.Value, MappingContextPriority);
	ApplyScrollWheelPatch();
}

void AWiremodBaseTool::EjectMappings()
{
	auto Player = Cast<AFGPlayerController>(GetInstigatorController());
	if(!Player)
	{
		ACircuitryLogger::DispatchErrorEvent("[CIRCUITRY TOOL KEYBIND HANDLER] Somehow, the player reference was null when ejecting mappings.");
		return;
	}
		
	auto LocalPlayer = Player->GetLocalPlayer();
	if(!LocalPlayer)
	{
		ACircuitryLogger::DispatchErrorEvent("[CIRCUITRY TOOL KEYBIND HANDLER] Somehow, the local player reference was null when ejecting mappings.");
		return;
	}
		
	auto EnhancedInputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if(!EnhancedInputSystem)
	{
		ACircuitryLogger::DispatchErrorEvent("[KEYBIND HANDLER] Input System is nullptr.");
		return;
	}
		
	for(auto& InputsContext : InputsContexts) EnhancedInputSystem->RemoveMappingContext(InputsContext.Value);
	RemoveScrollWheelPatch();
}

void AWiremodBaseTool::ShowNotification(const FText& Text)
{
	auto Player = Cast<AFGPlayerController>(GetInstigatorController());
	if(!Player) return;

	auto GameUI = Player->GetGameUI();
	if(!GameUI)
	{
		ACircuitryLogger::DispatchErrorEvent("Game UI was null when trying to display notification text");
		return;
	}

	GameUI->ShowTextNotification(Text);
}
