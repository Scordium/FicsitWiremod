// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/DynamicValues/CCDynamicValueUtils.h"
#include "CustomStructProcessor.generated.h"


UCLASS()
class UCustomStructClipboard : public UFGFactoryClipboardSettings
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FCustomStruct Value;
};

UCLASS()
class FICSITWIREMOD_API ACustomStructProcessor : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		for(int i = 0; i < Out.Values.Num(); i++)
		{
			if(!IsConnected(i)) continue;

			auto Value = Out.Values[i].Value;
			//Since all inputs will be disconnected when we change the structure, we don't have to check if the connection types match
			//The only case where that could happen is if someone changed their save data through some 3P editor, so it's their own fault if something crashes
			Out.Values[i].Value = UCCDynamicValueUtils::FromValue(GetConnection(i), Value ? (UObject*) Value : (UObject*) this);
		}
	}

	virtual bool CanUseFactoryClipboard_Implementation() override { return true; }
	virtual TSubclassOf<UObject> GetClipboardMappingClass_Implementation() override { return StaticClass(); }
	virtual TSubclassOf<UFGFactoryClipboardSettings> GetClipboardSettingsClass_Implementation() override { return UCustomStructClipboard::StaticClass(); }
	virtual UFGFactoryClipboardSettings* CopySettings_Implementation() override
	{
		auto Val = NewObject<UCustomStructClipboard>();
		Val->Value = Out;
		return Val;
	}

	virtual bool PasteSettings_Implementation(UFGFactoryClipboardSettings* factoryClipboard) override
	{
		auto Val = Cast<UCustomStructClipboard>(factoryClipboard);
		auto NewStruct = FCustomStruct();
		NewStruct.Name = Val->Value.Name;
		
		for(auto& Property : Val->Value.Values)
		{
			auto NewPropertyValue = UCCDynamicValueUtils::FromType(Property.Value ? Property.Value->ConnectionType.GetValue() : Unknown, this);
			NewStruct.Values.Add(FNamedDynamicValue(Property.Name, NewPropertyValue));
		}
		
		SetCustomStruct_Internal(NewStruct);
		return true;
	}

	UFUNCTION(BlueprintCallable)
	void SetCustomStruct(const FCustomStruct& NewStruct, UObject* Setter)
	{
		if(GetCanConfigure(Setter))
			SetCustomStruct_Internal(NewStruct);
	}

	void SetCustomStruct_Internal(const FCustomStruct& NewStruct)
	{
		//Disconnect all changed inputs
		for(int i = 0; i < NewStruct.Values.Num(); i++)
		{
			if(i >= Out.Values.Num()) break;

			auto& OldValue = Out.Values[i];
			auto& NewValue = NewStruct.Values[i];
			
			if(!NewValue.Value || OldValue.Value->ConnectionType != NewValue.Value->ConnectionType || OldValue.Name != NewValue.Name)
				OnInputDisconnected_Internal(i);
		}

		//Disconnect all stale inputs that remain in case the new struct is smaller.
		if(Out.Values.Num() > NewStruct.Values.Num())
		{
			for(int i = Out.Values.Num() - 1; i >= NewStruct.Values.Num(); i--)
			{
				OnInputDisconnected_Internal(i);
			}
		}
		
		Out = NewStruct;
		Recompile();
	}

	UFUNCTION(BlueprintPure)
	bool RequiresRecompile(const FCustomStruct& CompareTo){ return Out != CompareTo; }

	void Recompile()
	{
		ConnectionsInfo.Inputs.Empty();

		for(const auto& Field : Out.Values)
		{
			if(!Field.Value)
			{
				ACircuitryLogger::DispatchWarningEvent("Detected null value on struct recompile: " + Field.Name);
				continue;
			}
			auto Input = FBuildingConnection(Field.Name, "", Field.Value->ConnectionType);
			ConnectionsInfo.Inputs.Add(Input);
		}
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(ACustomStructProcessor, Out)
	}
	
	virtual void GatherReplicatedObjects_Implementation(TArray<UObject*>& OutObjects) override
	{
		Super::GatherReplicatedObjects_Implementation(OutObjects);
		for(const auto& Val : Out.Values)
			OutObjects.Add(Val.Value);
	}

	UPROPERTY(SaveGame, BlueprintReadWrite, Replicated)
	FCustomStruct Out;
};
