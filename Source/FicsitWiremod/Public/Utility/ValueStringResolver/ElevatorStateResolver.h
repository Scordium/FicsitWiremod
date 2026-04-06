#pragma once
#include "ValueStringResolverBase.h"
#include "CommonLib/ConnectionType.h"

#include "ElevatorStateResolver.generated.h" 

UCLASS(Blueprintable, BlueprintType)
class UElevatorStateResolver : public UValueStringResolverBase
{
	GENERATED_BODY()

public:
	virtual FString ResolveString_Implementation(const FConnectionPointer& Pointer, EConnectionType Type) const override
	{
		if (Type != EConnectionType::Integer) return Super::ResolveString_Implementation(Pointer, Type);

		auto Value = (EElevatorState) UReflectionUtilities::GetFloat(Pointer);

		FString Result = FString::FromInt((int) Value);
		
		switch (Value)
		{
		case EElevatorState::EES_NONE: Result += " (None)"; break;
		case EElevatorState::EES_Moving: Result += " (Moving)"; break;
		case EElevatorState::EES_DoorsClosing: Result += " (Doors Closing)"; break;
		case EElevatorState::EES_DoorsOpening: Result += " (Doors Opening)"; break;
		case EElevatorState::EES_SafetyStopping: Result += " (Power Outage Stop)"; break;
		case EElevatorState::EES_ArrivedAtFloor: Result += " (Arrived at Floor)"; break;
		case EElevatorState::EES_IdleAtFloor: Result += " (Idle at Floor)"; break;
		case EElevatorState::EES_PauseBeforeMove: Result += " (Pause Before Moving)"; break;
		case EElevatorState::EES_PausePowerOut: Result += " (Power Outage Pause)"; break;
		case EElevatorState::EES_WaitingAtFloor: Result += " (Wait at Floor)"; break;
		case EElevatorState::EES_PauseBeforeDoorOpen: Result += " (Pause Before Doors Open)"; break;
		}

		return Result;
	}
};
