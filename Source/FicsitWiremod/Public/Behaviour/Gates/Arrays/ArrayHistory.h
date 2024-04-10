// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/DynamicValues/CCDynamicValueUtils.h"
#include "ArrayHistory.generated.h"

UCLASS()
class FICSITWIREMOD_API AArrayHistory : public AFGWiremodBuildable, public IDynamicValuePasser
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		if(!Cache) return;

		//Clear array if needed
		if(GetConnection(3).GetBool()) Cache->Clear();

		//Insert element
		if(GetConnection(2).GetBool())
		{
			if(HasTickedLastFrame) return;

			const auto& Value = GetConnection(0);
			Cache->InsertElement(Value, 0);

			//Set array size
			const int MaxSize = GetConnection(1).GetFloat(60);
			Cache->Resize(MaxSize);
			
			HasTickedLastFrame = true;
		}
		else HasTickedLastFrame = false;
		
	}

	virtual void OnInputConnected_Implementation(const FConnectionData& Data, int Index, UObject* Setter) override
	{
		Super::OnInputConnected_Implementation(Data, Index, Setter);

		if(Index == 0)
		{
			auto ArrayType = UConnectionTypeFunctions::BaseToArray(Data.ConnectionType);
			if(!Cache || !Cache->IsOfType(ArrayType)) Cache = Cast<UCCArrayValueBase>(UCCDynamicValueUtils::FromType(ArrayType, this));

			SetOutputType(0, ArrayType);
		}
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AArrayHistory, Cache)
	}

	virtual UObject* GetValue_Implementation(const FString& ValueName) override { return Cache; }


	UPROPERTY(Replicated, SaveGame)
	UCCArrayValueBase* Cache;

	UPROPERTY(SaveGame)
	bool HasTickedLastFrame;
};
