// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "StringByte.generated.h"

UCLASS()
class FICSITWIREMOD_API AStringByte : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(double DeltaTime) override
	{
		auto SourceString = GetConnection(0).GetString();
		int Index = GetConnection(1).GetFloat();
		
		if (Index >= 0 && Index < SourceString.Len())
			Out = SourceString.GetCharArray()[Index];
		else
			Out = 0;
	}
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AStringByte, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	int Out;
};
