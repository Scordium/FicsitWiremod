// 

#pragma once

#include "CoreMinimal.h"
#include "WiremodAPI.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "..\Behaviour\CircuitryInterface.h"
#include "Behaviour/VanillaInterface/WiremodVanillaConnections.h"
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
	static FORCEINLINE bool IsWiremod(UObject* Object)
	{
		if(!Object) return false;

		return Object->GetClass()->ImplementsInterface(ICircuitryProcessableInterface::UClassType::StaticClass());
	}

	UFUNCTION(BlueprintPure)
	static void GetAvailableConnections(UObject* Object, EConnectionDirection Direction, TArray<FBuildingConnection>& Connections, int& Count, FBuildableNote& Note)
	{
		//Wiremod
		if(auto WiremodBuildable = Cast<AFGWiremodBuildable>(Object))
		{
			Connections = WiremodBuildable->GetAvailableConnections(Direction, Count, Note);
			return;
		}
		
		//Vanilla
		auto Buildable = Cast<AFGBuildable>(Object);
		if(Buildable) AWiremodAPI::Self->GetConnections(Buildable, Direction, Connections, Count, Note);
	}

	UFUNCTION(BlueprintPure)
	static bool IsRegistered(UObject* Object)
	{
		//Wiremod
		if(IsWiremod(Object)) return true;
		
		if(auto Buildable = Cast<AFGBuildable>(Object))
		{
			//Vanilla
			return AWiremodAPI::Self->IsBuildableRegistered(Buildable);
		}

		return false;
	}


	UFUNCTION(BlueprintPure)
	static FWiremodOwnerData GetOwnerInfo(UObject* Object)
	{
		if(Object)
		{
			//Wiremod
			if(auto WiremodBuildable = Cast<AFGWiremodBuildable>(Object)) return WiremodBuildable->OwnerData;

			//Vanilla
			return AWiremodVanillaConnections::Self->GetOwnerData(Object);
			
		}

		return FWiremodOwnerData();
	}

	UFUNCTION(BlueprintPure)
	static TArray<FNewConnectionData> GetDataList(UObject* Object)
	{
		if(Object)
		{
			if(auto WiremodBuildable = Cast<AFGWiremodBuildable>(Object))
				return WiremodBuildable->InputConnections;
			
			auto Connections = AWiremodVanillaConnections::Self->GetAllConnections(Object);
				
			TArray<FNewConnectionData> Data = TArray<FNewConnectionData>();
			for(auto Connection : Connections)
			{
				Data.Add(Connection.Transmitter);
			}

			return Data;
		}

		return TArray<FNewConnectionData>();
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

	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="=="))
	static bool IsOwnerDataEqual(const FWiremodOwnerData& Data1, const FWiremodOwnerData& Data2){return Data1 == Data2; }
};
