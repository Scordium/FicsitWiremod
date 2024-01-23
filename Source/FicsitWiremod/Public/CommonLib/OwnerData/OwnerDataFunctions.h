// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/VanillaInterface/WiremodVanillaConnections.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OwnerData.h"
#include "OwnerDataFunctions.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UOwnerDataFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	static bool GetCanConfigure(const FWiremodOwnerData& Data, UObject* Actor) { return Data.GetCanConfigure(Actor); }

	UFUNCTION(BlueprintPure)
	static bool GetCanConnect(const FWiremodOwnerData& Data, UObject* Actor) { return Data.GetCanConnect(Actor); }

	UFUNCTION(BlueprintPure)
	static bool GetCanReset(const FWiremodOwnerData& Data, UObject* Actor) { return Data.GetCanDisconnect(Actor); }

	UFUNCTION(BlueprintPure)
	static bool HasOwner(const FWiremodOwnerData& Data){ return Data.HasOwner(); }

	UFUNCTION(BlueprintPure)
	static bool IsOwner(const FWiremodOwnerData& Data, UObject* Actor) { return Data.IsOwner(Actor); }

	UFUNCTION(BlueprintPure, meta=(CompactNodeTitle="=="), DisplayName="Equal (Owner data)")
	static bool Equal(const FWiremodOwnerData& Data1, const FWiremodOwnerData& Data2){return Data1 == Data2; }

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
};
