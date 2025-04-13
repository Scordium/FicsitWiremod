// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CCDynamicValueUtils.h"
#include "FGWiremodBuildable.h"
#include "CircuitryToJson.generated.h"

UCLASS()
class FICSITWIREMOD_API ACircuitryToJson : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		ValueCache = UCCDynamicValueUtils::FromValue(GetConnection(0), ValueCache);

		if (!ValueCache) Out = "";
		else
		{
			auto SerializedValue = ValueCache->ToJson();

			if (SerializedValue->Type == EJson::Object)
			{
				TSharedPtr<FJsonObject>* Object;
				SerializedValue->TryGetObject(Object);
				FJsonSerializer::Serialize(Object->ToSharedRef(), TJsonWriterFactory<>::Create(&Out, 0));
			}
			else
			{
				TSharedRef<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
				JsonObject->SetField("Value", SerializedValue);
				FJsonSerializer::Serialize(JsonObject, TJsonWriterFactory<>::Create(&Out, 0));
			}
		}
	}

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ThisClass, Out)
	}


	UPROPERTY(Replicated, SaveGame)
	FString Out;

	UPROPERTY()
	UCCDynamicValueBase* ValueCache;
};
