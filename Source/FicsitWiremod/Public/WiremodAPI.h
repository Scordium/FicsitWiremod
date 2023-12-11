// 

#pragma once

#include "CoreMinimal.h"
#include "WiremodUtils.h"
#include "Behaviour/CircuitryConnectionsProvider.h"
#include "Buildables/FGBuildable.h"
#include "Engine/DataTable.h"
#include "HAL/FileManagerGeneric.h"
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

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class FICSITWIREMOD_API AWiremodAPI : public AModSubsystem
{
	GENERATED_BODY()
public:

	//A shitty bandaid because USubsystemActorManager can't find the subsystem instance despite it being registered and accessible through this variable.
	inline static AWiremodAPI* Self;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWiremodAPIData Data;
	
	FString ForceFileUsePrefix = "CIRCUITRY_FORCEUSE_";
	
	virtual void BeginPlay() override
	{
		Super::BeginPlay();
		Self = this;
		ParseLists();
	}

	UFUNCTION(BlueprintCallable)
	FString ParseLists(bool ForceOverwrite = false)
	{
		FString ErrorList;
		
		auto const Path = FPaths::ProjectDir() + "/CircuitryAPI";
		
		if(!FFileManagerGeneric::Get().DirectoryExists(*Path))
		{
			FFileManagerGeneric::Get().MakeDirectory(*Path);
			FString PlaceholderFile = 
			"This is a folder for circuitry API lists!\nDrop your JSON table in this folder and circuitry will attempt to add connections to buildings.\n"
			"The file name must match the mod reference! i.e. Circuitry's mod reference is \"FicsitWiremod\"\n"
			"In case circuitry already integrated the mod in question but you still want to use your own data list, you can force the file to be used as a data table by prefixing the file with \"" + ForceFileUsePrefix + "\"";

			FFileHelper::SaveStringToFile(PlaceholderFile, *FString(Path + "/README.txt"));
			return "CIRCUITRY API directory does not exist";
		}


		TArray<FString> Files;
		FFileManagerGeneric::Get().FindFiles(Files, *Path, *FString("json"));
		int SuccessParse = 0;
		for (FString File : Files)
		{
			UDataTable* Table = NewObject<UDataTable>();
			Table->RowStruct = FBuildingConnections::StaticStruct();
	
			FString JSON;
			FFileHelper::LoadFileToString(JSON, *FString(Path + "/" + File));
			auto ParseErrors = Table->CreateTableFromJSONString(JSON);
			
			if(ParseErrors.Num() != 0)
			{
				for (FString Error : ParseErrors)
				{
					ACircuitryLogger::DispatchErrorEvent("[CIRCUITRY API] There was an error when trying to parse file " + File + ": " + Error);
					ErrorList.Append("[" + File + "]: " + Error + "\n");
				}
				continue;
			} else SuccessParse++;


			FString ModRef = File
			.Replace(*ForceFileUsePrefix, *FString())
			.Replace(*FString(".json"), *FString(""));
			
			AddList(ModRef, Table, File.StartsWith(ForceFileUsePrefix) || ForceOverwrite);
		}

		ACircuitryLogger::DispatchEvent("[CIRCUITRY API] Finished parsing lists. Parsed without errors " + CC_INT(SuccessParse) + " out of " +CC_INT(Files.Num()) + " lists", ELogVerbosity::Display);
		return ErrorList.Len() > 0 ? ErrorList : "Successfully parsed all lists!";
	}
	
	
	
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

	UFUNCTION(BlueprintCallable)
	void AddList(FString modReference, UDataTable* list, bool allowOverwrite = false)
	{
		if(Data.ConnectionLists.Contains(modReference) && !allowOverwrite) return;

		ACircuitryLogger::DispatchEvent("[CIRCUITRY API] New connections list was added at runtime. {Mod reference:" + modReference + " | Entries count:" + CC_INT(list->GetRowNames().Num()) + "}", ELogVerbosity::Display);
		Data.ConnectionLists.Add(modReference, list);
	}

	UFUNCTION(BlueprintPure)
	UDataTable* GetListOrDefault(FString modReference)
	{
		if(Data.ConnectionLists.Contains(modReference)) return *Data.ConnectionLists.Find(modReference);
		return Data.FactoryGameList;
	}

	UFUNCTION(BlueprintPure, DisplayName="Is Registered", Category="Wiremod API")
	bool IsBuildableRegistered(UObject* Object)
	{
		auto Buildable = Cast<AFGBuildable>(Object);
		if(!IsValid(Buildable)) return false;
		
		auto list = GetListOrDefault(UWiremodUtils::GetModReference(Buildable->GetClass()));
		auto className = UWiremodUtils::GetClassName(Buildable->GetClass());
		
		return list->GetRowNames().Contains(className);
	}


	UFUNCTION(BlueprintCallable)
	FBuildingConnections FindConnections(AFGBuildable* Buildable)
	{
		if(!IsValid(Buildable)) return FBuildingConnections();
		
		auto modReference = UWiremodUtils::GetModReference(Buildable->GetClass());
		
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

	UFUNCTION(BlueprintCallable)
	void GetConnections(AFGBuildable* Buildable, EConnectionDirection Direction, TArray<FBuildingConnection>& Connections, FBuildableNote& Note)
	{
		auto FoundData = FindConnections(Buildable);
		Note = FoundData.Note;
		
		switch (Direction)
		{
		case Input: Connections = FoundData.Inputs; break;
		case Output: Connections = FoundData.Outputs; break;
		}
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
