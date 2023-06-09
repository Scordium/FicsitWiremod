// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "AppendString.generated.h"

UCLASS()
class FICSITWIREMOD_API AAppendString : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		//Default mode - multiple inputs for values and one input for separator
		if(CurrentStateIndex == 0)
		{
			TArray<FString> Values;
			FString Separator = GetConnection(0).GetString();

			for(int i = 1; i < InputConnections.Num(); i++)
				Values.Add(GetConnection(i).GetString());

			Out = FString::Join(Values, *Separator);
		}
		//Alt mode - one input for array, one input for separator
		else if(CurrentStateIndex == 1)
		{
			TArray<FString> Values = GetConnection(0).GetStringArray();
			FString Separator = GetConnection(1).GetString();

			Out = FString::Join(Values, *Separator);
		}
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AAppendString, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	FString Out;
};
