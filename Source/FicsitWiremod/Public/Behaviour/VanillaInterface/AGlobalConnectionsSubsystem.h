// 

#pragma once

#include "CoreMinimal.h"
#include "WiremodVanillaConnections.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Buildables/FGBuildable.h"
#include "CommonLib/BackwardsCompatibilityHandler.h"
#include "AGlobalConnectionsSubsystem.generated.h"

UCLASS(Blueprintable)
class FICSITWIREMOD_API AAGlobalConnectionsSubsystem : public AModSubsystem
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void CreateGlobalConnection(AFGBuildable* Buildable, int Index, const FDynamicValue& Value, const FConnectionData& Input, UObject* Setter)
	{
		if(Input.Object && Input.Object->GetClass()->IsChildOf(UCCDynamicValueBase::StaticClass()))
		{
			UpdateGlobalConnection(Buildable, Index, Value, Input, Setter);
			return;
		}

		auto Data = FConnectionData(Value.Convert(Buildable), FName("Value"), Value.ConnectionType);
		Data.WireHidden = true;

		if(Buildable->GetClass()->ImplementsInterface(ICircuitryProcessableInterface::UClassType::StaticClass()))
			ICircuitryProcessableInterface::Execute_OnInputConnected(Buildable, Data, Index, Setter);
		else
		{
			auto Dynamic = FDynamicConnectionData(Data, Input);
			AWiremodVanillaConnections::Self->AddConnection(Dynamic, Index, Setter);
		}
	}

	UFUNCTION(BlueprintCallable)
	void UpdateGlobalConnection(AFGBuildable* Buildable, int Index,  const FDynamicValue& Value, const FConnectionData& Input, UObject* Setter)
	{
		auto Data = FConnectionData(Value.Convert(Input.Object), FName("Value"), Value.ConnectionType);
		Data.WireHidden = true;

		if(Buildable->GetClass()->ImplementsInterface(ICircuitryProcessableInterface::UClassType::StaticClass()))
			ICircuitryProcessableInterface::Execute_OnInputConnected(Buildable, Data, Index, Setter);
		else
		{
			auto Dynamic = FDynamicConnectionData(Data, Input);
			AWiremodVanillaConnections::Self->AddConnection(Dynamic, Index, Setter);
		}
	}
	
	
};
