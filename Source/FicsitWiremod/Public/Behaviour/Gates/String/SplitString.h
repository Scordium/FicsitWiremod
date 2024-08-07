﻿// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "SplitString.generated.h"

UCLASS()
class FICSITWIREMOD_API ASplitString : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		auto Source = GetConnection(0).GetString();
		auto Separator = GetConnection(1).GetString();
		auto CullEmpty = GetConnection(2).GetBool();
		
		if(Separator.Len() > 0) Source.ParseIntoArray(Out, *Separator, CullEmpty);
		else
		{
			Out.Empty();
			for (int i = 0; i < Source.Len(); i++)
			{
				Out.Add(FString::Chr(Source[i]));
			} 
		}
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ASplitString, Out)
	}

	UPROPERTY(Replicated, SaveGame)
	TArray<FString> Out;
};
