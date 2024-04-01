// 

#pragma once

#include "CoreMinimal.h"
#include "HttpModule.h"
#include "CommonLib/FileUtilities.h"
#include "WiremodUtils.h"
#include "Behaviour/CircuitryConnectionsProvider.h"
#include "Buildables/FGBuildable.h"
#include "Engine/DataTable.h"
#include "HAL/FileManagerGeneric.h"
#include "Interfaces/IHttpResponse.h"
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
	FString CompatibilityPackagesBaseUrl = "https://circuitry.crosscat-is.me/packages/";
	
	virtual void BeginPlay() override
	{
		Super::BeginPlay();
		Self = this;
		ParseLists();
		FetchCompatibilityPackagesList();
	}

	UFUNCTION(BlueprintImplementableEvent)
	void SendPackageInstallNotification(const FString& ModReference);

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
			"In case circuitry already integrated the mod in question but you still want to use your own data list, you can force the file to be used as a data table by prefixing the file with \"" + ForceFileUsePrefix + "\""
			"The API will also automatically fetch packages from the server, so this folder might be unexpectedly populated.";

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


			FString ModRef;
			FString Version;

			//Made for backwards compatibility because the old jsons were not in the format of <MODREF>__<TIMESTAMP>
			//If the file name failed to split then it's an old version
			//But if it did then we'll get the mod reference from that split
			if(!File.Split("__", &ModRef, &Version))
			{
				ModRef = File
				.Replace(*ForceFileUsePrefix, *FString())
				.Replace(*FString(".json"), *FString(""));
			}
			
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
	void AddList(const FString& ModReference, UDataTable* List, bool AllowOverwrite = false)
	{
		if(Data.ConnectionLists.Contains(ModReference) && !AllowOverwrite) return;

		ACircuitryLogger::DispatchEvent("[CIRCUITRY API] New connections list was added at runtime. {Mod reference:" + ModReference + " | Entries count:" + CC_INT(List->GetRowNames().Num()) + "}", ELogVerbosity::Display);
		Data.ConnectionLists.Add(ModReference, List);
	}

	UFUNCTION(BlueprintPure)
	UDataTable* GetListOrDefault(const FString& ModReference)
	{
		if(Data.ConnectionLists.Contains(ModReference)) return *Data.ConnectionLists.Find(ModReference);
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
		
		auto ModReference = UWiremodUtils::GetModReference(Buildable->GetClass());
		auto ClassName = UWiremodUtils::GetClassName(Buildable->GetClass());

		return FindRedirectedList(ModReference + "__" + ClassName.ToString());
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

	FBuildingConnections FindRedirectedList(const FString& Redirector)
	{
		//Format the redirector
		FString ModReference;
		FString ObjectReference;
		if(!Redirector.Split("__", &ModReference, &ObjectReference))
		{
			ACircuitryLogger::DispatchErrorEvent("Redirector '" + Redirector + "' has invalid format!");
			return FBuildingConnections();
		}


		//Get the list from formatted redirector and check if it contains the row we're looking for
		const auto List = GetListOrDefault(ModReference);
		if(!List->GetRowNames().Contains(FName(ObjectReference))) return FBuildingConnections();

		//If the found row also has a redirector, recursively search for the source.
		const auto Row = List->FindRow<FBuildingConnections>(FName(ObjectReference), "");
		if(!Row->RedirectTo.IsNone()) return FindRedirectedList(Row->RedirectTo.ToString());

		//Otherwise return the data.
		return *Row;
	}

	
	void FetchCompatibilityPackagesList()
	{
		//Create request to fetch packages from the server
		auto Request = FHttpModule::Get().CreateRequest();
		
		Request->SetURL(CompatibilityPackagesBaseUrl);
		Request->OnProcessRequestComplete().BindUObject(this, &AWiremodAPI::OnCompatibilityPackagesFetched);
		Request->SetVerb("GET");
		Request->SetTimeout(60);

		if(!Request->ProcessRequest())
		{
			ACircuitryLogger::DispatchErrorEvent("[CIRCUITRY API] There was an error when initiating request to the compatibility packages server.");
			return;
		}
	}

	void OnCompatibilityPackagesFetched(FHttpRequestPtr RequestObject, FHttpResponsePtr Response, bool Success)
	{
		if(!Success)
		{
			auto Error = "[CIRCUITRY API] Something went wrong when trying to fetch compatibility packages: " + Response.Get()->GetContentAsString() + " (" + FString::FromInt(Response->GetResponseCode()) + ")";
			ACircuitryLogger::DispatchErrorEvent(Error);
			return;
		}

		//Package names are split with a newline character
		TArray<FString> PackagesList;
		Response->GetContentAsString().ParseIntoArrayLines(PackagesList);

		auto ModLib = GetWorld()->GetGameInstance()->GetSubsystem<UModLoadingLibrary>();

		TArray<FString> DownloadPackages;
		
		for(auto& Package : PackagesList)
		{
			FString ModRef;
			FString PackageVersion;

			Package.Split("__", &ModRef, &PackageVersion);
			
			if(!ModLib->IsModLoaded(ModRef)) continue;

			auto CurrentPackageVersion = FindCurrentPackageVersion(ModRef);

			if(PackageVersion != CurrentPackageVersion) DownloadPackages.Add(Package);
			else
			{
				auto Text = "[CIRCUITRY API] Compatibility package for " + ModRef + " is already at latest version (" + PackageVersion + "). Skipping download.";
				ACircuitryLogger::DispatchEvent(Text, ELogVerbosity::Display);
			}
		}

		//Download missing/outdated packages
		StartPackageDownload(DownloadPackages);
	}

	FString FindCurrentPackageVersion(const FString& ModRef) const
	{
		auto const Path = FPaths::ProjectDir() + "/CircuitryAPI";
		TArray<FString> Files;
		FFileManagerGeneric::Get().FindFiles(Files, *Path, *FString("json"));

		for(auto& File : Files)
		{
			if(File.StartsWith(ModRef))
			{
				FString ModReference;
				FString Version;
				if(File.Split("__", &ModReference, &Version))
				{
					Version.RemoveFromEnd(".json");
					return Version;
				}

				return "";
			}
		}

		return "";
	}

	void RemoveCurrentCompatibilityPackage(const FString& ModRef)
	{
		auto const Path = FPaths::ProjectDir() + "/CircuitryAPI";
		TArray<FString> Files;
		FFileManagerGeneric::Get().FindFiles(Files, *Path, *FString("json"));

		for(auto& File : Files)
		{
			auto FilePath = Path + "/" + File;
			if(File.StartsWith(ModRef)) FFileManagerGeneric::Get().Delete(*FilePath);
		}
	}

	void StartPackageDownload(const TArray<FString>& Packages)
	{
		for(auto& Package : Packages)
		{
			auto Request = FHttpModule::Get().CreateRequest();
		
			Request->SetURL(CompatibilityPackagesBaseUrl + Package);
			Request->OnProcessRequestComplete().BindUObject(this, &AWiremodAPI::OnPackageDownloaded);
			Request->SetVerb("GET");
			Request->SetTimeout(60);

			if(!Request->ProcessRequest())
			{
				ACircuitryLogger::DispatchErrorEvent("[CIRCUITRY API] Failed to start fetch for package " + Package);
				return;
			}
		}
	}

	void OnPackageDownloaded(FHttpRequestPtr RequestObject, FHttpResponsePtr Response, bool Success)
	{
		auto Package = Response->GetURL();
		Package.RemoveFromStart(CompatibilityPackagesBaseUrl);

		FString ModReference;
		FString PackageVersion;

		Package.Split("__", &ModReference, &PackageVersion);

		//Remove existing package files
		RemoveCurrentCompatibilityPackage(ModReference);

		//Create new package file table
		UDataTable* Table = NewObject<UDataTable>();
		Table->RowStruct = FBuildingConnections::StaticStruct();

		auto TableDataJson = Response->GetContentAsString();
		auto ParseErrors = Table->CreateTableFromJSONString(TableDataJson);
		if(ParseErrors.Num() != 0)
		{
			for (FString Error : ParseErrors)
			{
				ACircuitryLogger::DispatchErrorEvent("[CIRCUITRY API] There was an error when trying to parse package " + ModReference + ": " + Error);
			}
			return;
		}

		//Save the package string to a file
		auto const Path = FPaths::ProjectDir() + "/CircuitryAPI";
		UFileUtilities::SaveStringAsFile(Path, Package + ".json", TableDataJson);

		//Add the table to supported mods and notify the user that they downloaded a package for this.
		AddList(ModReference, Table, true);
		SendPackageInstallNotification(ModReference);

		ACircuitryLogger::DispatchEvent("[CIRCUITRY API] Compatibility package " + Package + " downloaded.", ELogVerbosity::Display);
	}
};
