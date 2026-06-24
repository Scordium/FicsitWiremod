// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/DynamicValues/CCArrayValueBase.h"
#include "CommonLib/DynamicValues/CCDynamicValueUtils.h"
#include "ArrayFilter.generated.h"

UCLASS()
class FICSITWIREMOD_API AArrayFilter : public AFGWiremodBuildable, public IDynamicValuePasser
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		const auto& Connection = GetConnection(0);
		if(!Out || Connection.ConnectionType != Out->ConnectionType)
		{
			SetOutputType(0, Unknown);
			Out = Cast<UCCArrayValueBase>(UCCDynamicValueUtils::FromType(Connection.ConnectionType, this));
			if (Out) Out->SetFilter(FilterData);
		}

		if(Out)
		{
			Out->FromConnectionValue(Connection.ToPointer());
			SetOutputType(0, Out->ConnectionType);
		}
	}

	virtual UObject* GetValue_Implementation(const FString& ValueName) override { return Out; }

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ThisClass, Out);
	}

	virtual void GatherReplicatedObjects_Implementation(TArray<UObject*>& Output) override
	{
		Super::GatherReplicatedObjects_Implementation(Output);
		Output.Add(Out);
	}

	UFUNCTION(BlueprintCallable)
	void SetFilter(const FCircuitryArrayFilterData& NewFilterData, UObject* Setter)
	{
		PERMISSION_CHECK(Setter);
		
		if(FilterData != NewFilterData)
		{
			FilterData = NewFilterData;
			if (Out) Out->SetFilter(FilterData);
		}
	}
	
	UPROPERTY(Replicated, BlueprintReadWrite, SaveGame)
	UCCArrayValueBase* Out;
	
	UPROPERTY(BlueprintReadWrite, SaveGame)
	FCircuitryArrayFilterData FilterData;
};
