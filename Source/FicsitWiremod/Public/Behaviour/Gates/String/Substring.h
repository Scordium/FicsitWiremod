// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Substring.generated.h"

UCLASS()
class FICSITWIREMOD_API ASubstring : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:

	virtual void Process_Implementation(float DeltaTime) override
	{
		auto Source = GetConnection(0).GetString();
		int StartIndex = GetConnection(1).GetFloat();
		int Length = GetConnection(2).GetFloat();

		Out = Source.Mid(StartIndex, Length);
	}
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ASubstring, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	FString Out;
};
