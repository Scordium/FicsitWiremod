// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "GetStringChar.generated.h"

UCLASS()
class FICSITWIREMOD_API AGetStringChar : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		auto String = GetConnection(0).GetString();
		int Index = GetConnection(1).GetFloat();

		if(String.IsValidIndex(Index)) Out = FString::Chr(String[Index]);
		else Out = "";
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AGetStringChar, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	FString Out;
};
