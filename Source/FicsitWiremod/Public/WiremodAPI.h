// 

#pragma once

#include "CoreMinimal.h"
#include "WiremodReflection.h"
#include "WiremodUtils.h"
#include "Buildables/FGBuildable.h"
#include "Engine/DataTable.h"
#include "HAL/FileManagerGeneric.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Subsystem/ModSubsystem.h"
#include "UObject/Object.h"
#include "WiremodAPI.generated.h"

USTRUCT(BlueprintType)
struct FWiremodAPIData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, UDataTable*> ConnectionLists;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* FactoryGameList;
	
};

#define Ref UWiremodUtils::GetModReference

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class FICSITWIREMOD_API AWiremodAPI : public AModSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWiremodAPIData Data;
	
	UFUNCTION(BlueprintCallable)
	void AddLists(TMap<FString, UDataTable*> lists, UDataTable* factoryGameList, bool override)
	{
		Data.FactoryGameList = factoryGameList;
		for (TTuple<FString, UDataTable*> List : lists)
		{
			if(Data.ConnectionLists.Contains(List.Key) && !override) continue;
			Data.ConnectionLists.Add(List);
		}
	}

	UFUNCTION(BlueprintPure)
	UDataTable* GetListOrDefault(FString modReference)
	{
		if(Data.ConnectionLists.Contains(modReference)) return *Data.ConnectionLists.Find(modReference);
		return Data.FactoryGameList;
	}

	UFUNCTION(BlueprintPure, DisplayName="Is Registered", Category="Wiremod API")
	bool IsBuildableRegistered(AFGBuildable* buildable)
	{
		if(!IsValid(buildable)) return false;
		
		auto list = GetListOrDefault(Ref(buildable->GetClass()));
		auto className = UWiremodUtils::GetClassName(buildable->GetClass());
		
		return list->GetRowNames().Contains(className);
	}


	UFUNCTION(BlueprintCallable)
	FBuildingConnections FindConnections(AFGBuildable* Buildable)
	{
		if(!IsValid(Buildable)) return FBuildingConnections();
		
		auto modReference = Ref(Buildable->GetClass());
		
		auto list = GetListOrDefault(modReference);
		auto className = UWiremodUtils::GetClassName(Buildable->GetClass());

		if(list->GetRowNames().Contains(className))
		{
			auto row = list->FindRow<FBuildingConnections>(className, "");
			
			if(row->RedirectTo.ToString() == "None") return *row;

			return FindRedirectedList(row->RedirectTo.ToString());
		}

		return FBuildingConnections();
	}

	FBuildingConnections FindRedirectedList(FString redirector)
	{
		//Format the redirector
		FString listReference;
		FString objectReference;
		redirector.Split("__", &listReference, &objectReference);


		//Get the list from formatted redirector and check if it contains the row we're looking for
		auto list = GetListOrDefault(listReference);
		if(!list->GetRowNames().Contains(FName(objectReference))) return FBuildingConnections();

		//If the found row also has a redirector, recursively search for the source.
		auto row = list->FindRow<FBuildingConnections>(FName(objectReference), "");
		if(row->RedirectTo.ToString() == "None") return *row;

		return FindRedirectedList(row->RedirectTo.ToString());
	}

	
};
