// 

#pragma once

#include "CoreMinimal.h"
#include "WiremodAPI.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Behaviour/VanillaInterface/WiremodVanillaConnections.h"
#include "CommonLib/BackwardsCompatibilityHandler.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WiremodBlueprintUtils.generated.h"
/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UWiremodBlueprintUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	static FORCEINLINE bool IsCircuitry(UObject* Object)
	{
		if(!Object) return false;

		return Object->GetClass()->ImplementsInterface(ICircuitryProcessableInterface::UClassType::StaticClass());
	}

	UFUNCTION(BlueprintPure)
	static void GetAvailableConnections(UObject* Object, EConnectionDirection Direction, TArray<FBuildingConnection>& Connections, FBuildableNote& Note)
	{
		if(!Object) return;
		
		//Circuitry
		if(Object->GetClass()->ImplementsInterface(ICircuitryConnectionsProvider::UClassType::StaticClass()))
		{
			Connections = ICircuitryConnectionsProvider::Execute_GetConnectionsInfo(Object, Direction, Note);
			return;
		}
		
		//Vanilla
		if(auto Buildable = Cast<AFGBuildable>(Object)) AWiremodAPI::Self->GetConnections(Buildable, Direction, Connections, Note);
	}

	///Whether the object is compatible for displaying inputs or outputs.
	///This returns true if the object implements ICircuitryConnectionsProvider, is itself a circuitry object, or is registered with the API.
	UFUNCTION(BlueprintPure)
	static bool IsObjectCompatible(UObject* Object)
	{
		if(!Object) return false;
		
		//Circuitry
		if(IsCircuitry(Object)) return true;

		//Interface
		if(Object->GetClass()->ImplementsInterface(ICircuitryConnectionsProvider::UClassType::StaticClass())) return true;

		//Vanilla
		return AWiremodAPI::Self->IsBuildableRegistered(Object);
	}

	UFUNCTION(BlueprintPure)
	static TArray<FConnectionData> GetDataList(UObject* Object)
	{
		if(Object)
		{
			if(auto WiremodBuildable = Cast<AFGWiremodBuildable>(Object))
				return WiremodBuildable->InputConnections;
			
			auto Connections = AWiremodVanillaConnections::Self->GetAllConnections(Object);
				
			TArray<FConnectionData> Data = TArray<FConnectionData>();
			for(auto Connection : Connections)
			{
				Data.Add(Connection.Transmitter);
			}

			return Data;
		}

		return TArray<FConnectionData>();
	}

	UFUNCTION(BlueprintPure)
	static void GetOccupationStatus(UObject* Object, EConnectionType AllowedType, TArray<TEnumAsByte<EConnectionOccupationState>>& Statuses)
	{
		if(Object)
		{
			if(auto WiremodBuildable = Cast<AFGWiremodBuildable>(Object)) WiremodBuildable->GetInputOccupationStatus(AllowedType, Statuses);
			else AWiremodVanillaConnections::Self->GetOccupationStatus(Object, AllowedType, Statuses);
		}
	}

	UFUNCTION(BlueprintPure)
	static UObject* GetSetterObject()
	{
		return UGameplayStatics::GetPlayerCharacter(UWiremodGameWorldModule::Self->GetWorld(), 0);
	}
};
