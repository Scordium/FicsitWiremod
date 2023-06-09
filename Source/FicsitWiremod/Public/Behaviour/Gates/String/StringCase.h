// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "StringCase.generated.h"

UCLASS()
class FICSITWIREMOD_API AStringCase : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		//To lowercase
		if(CurrentStateIndex == 0) Out = GetConnection(0).GetString().ToLower();
		//To UPPERCASE
		else if(CurrentStateIndex == 1) Out = GetConnection(0).GetString().ToUpper();
		//To Titlecase
		else if(CurrentStateIndex == 2)
		{
			Out = GetConnection(0).GetString();
			if(Out.Len() == 0) return;
			
			//What
			Out[0] = FChar::ToUpper(Out[0]);
		}
		//To SaRcAsMcAsE
		else if(CurrentStateIndex == 3)
		{
			Out = "";
			auto Source = GetConnection(0).GetString();

			for(int i = 0; i < Source.Len(); i++)
			{
				Out += i%2 ? FChar::ToLower(Source[0]) : FChar::ToUpper(Source[0]);;
			}
		}
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AStringCase, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	FString Out;
};
