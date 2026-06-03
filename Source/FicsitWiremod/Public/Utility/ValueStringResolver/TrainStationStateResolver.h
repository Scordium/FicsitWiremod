// 

#pragma once

#include "CoreMinimal.h"
#include "Utility/ValueStringResolver/ValueStringResolverBase.h"

#include "TrainStationStateResolver.generated.h"
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FICSITWIREMOD_API UTrainStationStateResolver : public UValueStringResolverBase
{
	GENERATED_BODY()

public:
	virtual FString ResolveString_Implementation(const FConnectionPointer& Pointer, EConnectionType Type) const override
	{
		if(Type != EConnectionType::Integer) return Super::ResolveString_Implementation(Pointer, Type);
		
		auto Value = (ETrainPlatformDockingStatus) UReflectionUtilities::GetFloat(Pointer);

		FString Result = FString::FromInt((int)Value);
		
		
		switch (Value)
		{
		case ETrainPlatformDockingStatus::ETPDS_None: Result += " (None)"; break;
		case ETrainPlatformDockingStatus::ETPDS_Complete: Result += " (Complete)"; break;
		case ETrainPlatformDockingStatus::ETPDS_Loading: Result += " (Cargo Loading)"; break;
		case ETrainPlatformDockingStatus::ETPDS_Unloading: Result += " (Cargo Unloading)"; break;
		case ETrainPlatformDockingStatus::ETPDS_WaitingForTransfer: Result += " (Waiting For Transfer)"; break;
		case ETrainPlatformDockingStatus::ETPDS_WaitingToStart: Result += " (Waiting To Start)"; break;
		case ETrainPlatformDockingStatus::ETPDS_IdleWaitForTime: Result += " (Waiting for Timer)"; break;
		case ETrainPlatformDockingStatus::ETPDS_WaitForTransferCondition: Result += " (Waiting for Condition)"; break;
		default: Result += " (Unknown?!)"; break;
		}

		return Result;
	}
};
