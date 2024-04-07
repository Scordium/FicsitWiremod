// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/DynamicValues/CCDynamicValueUtils.h"
#include "ArrayAppend.generated.h"

UCLASS()
class FICSITWIREMOD_API AArrayAppend : public AFGWiremodBuildable, public IDynamicValuePasser
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		ArrayCache1 = Cast<UCCArrayValueBase>(UCCDynamicValueUtils::FromValue(GetConnection(0), ArrayCache1));
		ArrayCache2 = Cast<UCCArrayValueBase>(UCCDynamicValueUtils::FromValue(GetConnection(1), ArrayCache2));
		
		auto OutType = ArrayCache1 ? ArrayCache1->ConnectionType.GetValue() : (ArrayCache2 ? ArrayCache2->ConnectionType.GetValue() : Unknown); 
		
		OutArray = Cast<UCCArrayValueBase>(UCCDynamicValueUtils::FromType(OutType, OutArray ? (UObject*)OutArray : (UObject*)this));
		if(OutArray)
		{
			OutArray->Clear();
			OutArray->AppendArray(ArrayCache1);
			OutArray->AppendArray(ArrayCache2);
		}

		SetOutputType(0, OutType);
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AArrayAppend, OutArray)
	}

	virtual void GatherReplicatedObjects_Implementation(TArray<UObject*>& Out) override
	{
		Super::GatherReplicatedObjects_Implementation(Out);
		Out.Add(OutArray);
	}

	virtual UObject* GetValue_Implementation(const FString& ValueName) override { return OutArray; }


	UPROPERTY() UCCArrayValueBase* ArrayCache1;
	UPROPERTY() UCCArrayValueBase* ArrayCache2;
	UPROPERTY(Replicated, SaveGame) UCCArrayValueBase* OutArray;
};
