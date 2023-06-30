// 

#pragma once

#include "CoreMinimal.h"
#include "WiremodAPI.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Behaviour/CircuitryInterface.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "Behaviour/VanillaInterface/WiremodVanillaConnections.h"
#include "CommonLib/BackwardsCompatibilityHandler.h"
#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
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

		return Object->GetClass()->ImplementsInterface(ICircuitryConnectionsProvider::UClassType::StaticClass());
	}

	UFUNCTION(BlueprintPure)
	static void GetAvailableConnections(UObject* Object, EConnectionDirection Direction, TArray<FBuildingConnection>& Connections, int& Count, FBuildableNote& Note)
	{
		//Circuitry
		if(IsWiremod(Object))
		{
			Connections = ICircuitryConnectionsProvider::Execute_GetConnectionsInfo(Object, Direction, Count, Note);
			return;
		}
		
		//Vanilla
		auto Buildable = Cast<AFGBuildable>(Object);
		if(Buildable) AWiremodAPI::Self->GetConnections(Buildable, Direction, Connections, Count, Note);
	}

	UFUNCTION(BlueprintPure)
	static bool IsRegistered(UObject* Object)
	{
		//Circuitry
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

	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="=="))
	static bool IsOwnerDataEqual(const FWiremodOwnerData& Data1, const FWiremodOwnerData& Data2){return Data1 == Data2; }


	UFUNCTION(BlueprintPure)
	static FString GenerateNameFromValue(const FDynamicValue& Value)
	{
		return "";
	}


	UFUNCTION(BlueprintCallable, CallInEditor)
	static void GenerateStringTable(const TArray<UClass*>& Classes)
	{
		auto Table = FStringTable::NewStringTable();

		for(auto Class : Classes)
		{
			if(auto Circuitry = TSubclassOf<AFGWiremodBuildable>(Class))
			{
				auto ClassName = UWiremodUtils::GetClassName(Class->GetDefaultObject()->GetClass()).ToString();
				
				auto KeyDisplayName = ClassName + "_DisplayName";
				auto KeyDescription = ClassName + "_Description";
				
				//Table->SetSourceString(KeyDisplayName, Circuitry->GetDefaultObject<AFGWiremodBuildable>()->mDisplayName.ToString());	
				//Table->SetSourceString(KeyDescription, Circuitry->GetDefaultObject<AFGWiremodBuildable>()->mDescription.ToString());

				if(auto Multistate = TSubclassOf<AMultistateWiremodBuildable>(Circuitry))
				{
					auto States = Multistate->GetDefaultObject<AMultistateWiremodBuildable>()->States;
					for(int i = 0; i < States.Num(); i++)
					{
						auto KeyModeDisplayName = ClassName + "_Modes_" + FString::FromInt(i) + "_DisplayName";
						auto KeyModeDescription = ClassName + "_Modes_" + FString::FromInt(i) + "_Description";

						Table->SetSourceString(KeyModeDisplayName, States[i].Name.ToString());
						Table->SetSourceString(KeyModeDescription, States[i].Description.ToString());
					}
				}
			}
		}

		Table->ExportStrings("C:/Users/SCRD/Desktop/SFModding/SatisfactoryModLoader/Plugins/FicsitWiremod/MultistateLoctable.csv");
	}
};
