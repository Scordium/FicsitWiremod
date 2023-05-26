// 

#pragma once

#include "CoreMinimal.h"
#include "IConstantsDistributor.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ConfigurableConstant.generated.h"

UCLASS()
class UConstantClipboardData : public UFGFactoryClipboardSettings
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TArray<FNamedValue> Values;
};



UCLASS()
class FICSITWIREMOD_API AConfigurableConstant : public AFGWiremodBuildable, public IIConstantsDistributor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override
	{
		Super::BeginPlay();
		ConvertDeprecatedConnections();
	}
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AConfigurableConstant, Values)
	}
	
	UFUNCTION(BlueprintCallable)
	FNamedValue FindValue(FString Name, bool AllowCached = true)
	{
		if(AllowCached && CachedValues.Contains(Name))
			return FNamedValue(Name, CachedValues[Name]);

		for (FNamedValue Value : Values)
		{
			if(Value.Name == Name)
			{
				CachedValues.Add(Name, Value.Value);
				return Value;
			}
		}

		return FNamedValue();
	}

	virtual bool CanUseFactoryClipboard_Implementation() override{ return true; }
	virtual TSubclassOf<UFGFactoryClipboardSettings> GetClipboardSettingsClass_Implementation() override { return UConstantClipboardData::StaticClass(); }
	virtual TSubclassOf<UObject> GetClipboardMappingClass_Implementation() override { return StaticClass(); }
	virtual UFGFactoryClipboardSettings* CopySettings_Implementation() override
	{
		auto Settings = NewObject<UConstantClipboardData>(this);
		Settings->Values = Values;

		return Settings;
	}
	virtual bool PasteSettings_Implementation(UFGFactoryClipboardSettings* factoryClipboard) override
	{
		auto Settings = Cast<UConstantClipboardData>(factoryClipboard);
		Values = Settings->Values;
		OnRep_ValuesUpdated();
		return true;
	}

	
	virtual FDynamicValue GetValue_Implementation(const FString& ValueName) override
	{
		return FindValue(ValueName);
	}

	virtual TArray<FBuildingConnection> GetAvailableConnections_Implementation(EConnectionDirection direction, int& Count, FBuildableNote& Note) override
	{
		if(direction == Input) return TArray<FBuildingConnection>();
		
		TArray<FBuildingConnection> Out;
		for (FNamedValue Value : Values)
		{
			auto Connection = FBuildingConnection(Value.Name, Value.Name, Value.Value.ConnectionType);
			Out.Add(Connection);
		}

		Count = Out.Num();
		return Out;
	}



	UFUNCTION()
	void OnRep_ValuesUpdated()
	{
		CachedValues.Empty();
	}

	UFUNCTION(BlueprintCallable)
	void RegisterValues(const TArray<FNamedValue>& NewValues)
	{
		this->Values = NewValues;
		OnRep_ValuesUpdated();
	}

	UFUNCTION(BlueprintImplementableEvent)
	void ConvertDeprecatedConnections();

	UFUNCTION()
	void netFunc_setBoolValue(FString Name, bool Value) { CreateNewOrUpdate(FNamedValue(Name, FDynamicValue(Value))); }

	UFUNCTION()
	void netFunc_setStringValue(FString Name, FString Value) { CreateNewOrUpdate(FNamedValue(Name, FDynamicValue(Value))); }

	UFUNCTION()
	void netFunc_setFloatValue(FString Name, float Value) { CreateNewOrUpdate(FNamedValue(Name, FDynamicValue(Value))); }

	UFUNCTION()
	void netFunc_setColorValue(FString Name, FLinearColor Value) { CreateNewOrUpdate(FNamedValue(Name, FDynamicValue(Value))); }

	UFUNCTION()
	void netFunc_setVectorValue(FString Name, FVector Value) { CreateNewOrUpdate(FNamedValue(Name, FDynamicValue(Value))); }



	UFUNCTION()
	void CreateNewOrUpdate(FNamedValue NamedValue)
	{
		//Try to find the existing value and change it
		for (auto& StoredValue : Values)
		{
			if(StoredValue.Name == NamedValue.Name)
			{
				StoredValue.Value = NamedValue.Value;
				OnRep_ValuesUpdated();
				return;
			}
		}

		//If value wasn't found, create a new one.
		Values.Add(NamedValue);
		OnRep_ValuesUpdated();
	}
	

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, SaveGame, ReplicatedUsing=OnRep_ValuesUpdated)
	TArray<FNamedValue> Values;

	UPROPERTY(EditInstanceOnly)
	TMap<FString, FDynamicValue> CachedValues;
};
