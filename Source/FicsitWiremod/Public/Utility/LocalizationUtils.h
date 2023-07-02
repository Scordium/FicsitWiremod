// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetStringTableLibrary.h"
#include "LocalizationUtils.generated.h"
/**
 * 
 */

#if WITH_EDITOR

#define ENTRY_CHK(Table, Key) UKismetStringTableLibrary::IsRegisteredTableEntry(Table, Key)
#define SKIP(Key) IsAGenericString(Key)

UCLASS()
class FICSITWIREMOD_API ULocalizationUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, CallInEditor)
	static FString AssignStringTableEntries(const TArray<UClass*>& Classes)
	{
		auto FailedAssignments = TArray<FString>();
		auto OpenAssets = TArray<FString>();
		auto LocTableName = FName("/FicsitWiremod/Localization/GeneratedLocTable.GeneratedLocTable");

		for(auto Class : Classes)
		{
			if(auto Circuitry = TSubclassOf<AFGWiremodBuildable>(Class))
			{
				auto DefaultObject = Class->GetDefaultObject<AFGWiremodBuildable>();
				auto ClassName = UWiremodUtils::GetClassName(DefaultObject->GetClass()).ToString();
				
				auto KeyDisplayName = ClassName + "_DisplayName";
				auto KeyDescription = ClassName + "_Description";
				
				if(ENTRY_CHK(LocTableName, KeyDisplayName)){ DefaultObject->mDisplayName = FText::FromStringTable(LocTableName, KeyDisplayName); }
				else FailedAssignments.Add(KeyDisplayName);	
				
				if(ENTRY_CHK(LocTableName, KeyDescription)) { DefaultObject->mDescription = FText::FromStringTable(LocTableName, KeyDescription); }
				else FailedAssignments.Add(KeyDescription);

				if(auto Multistate = TSubclassOf<AMultistateWiremodBuildable>(Circuitry))
				{
					auto& States = Multistate->GetDefaultObject<AMultistateWiremodBuildable>()->States;
					for(int i = 0; i < States.Num(); i++)
					{
						auto& State = States[i];
						auto KeyModeDisplayName = ClassName + "_Modes_" + FString::FromInt(i) + "_DisplayName";
						auto KeyModeDescription = ClassName + "_Modes_" + FString::FromInt(i) + "_Description";
						
						if(ENTRY_CHK(LocTableName, KeyModeDisplayName)) State.Name = FText::FromStringTable(LocTableName, KeyModeDisplayName);
						else FailedAssignments.Add(KeyModeDisplayName);	
						
						if(ENTRY_CHK(LocTableName, KeyModeDescription)) State.Description = FText::FromStringTable(LocTableName, KeyModeDescription);
						else FailedAssignments.Add(KeyModeDescription);	

						ParseMultistateConnectionList(LocTableName, ClassName, "_Inputs_", i, State.Connections.Inputs, FailedAssignments);
						ParseMultistateConnectionList(LocTableName, ClassName, "_Outputs_", i, State.Connections.Outputs, FailedAssignments);
					}
				}
				else
				{
					ParseConnectionList(LocTableName, ClassName, "_Inputs_", DefaultObject->ConnectionsInfo.Inputs, FailedAssignments);
					ParseConnectionList(LocTableName, ClassName, "_Outputs_", DefaultObject->ConnectionsInfo.Outputs, FailedAssignments);
				}

				DefaultObject->Modify(true);
				OpenAssets.Add(DefaultObject->GetClass()->GetPathName());
			}
			else FailedAssignments.Add("FAILED TO READ CIRCUITRY BUILDABLE OF CLASS " + Class->GetClass()->GetName());
		}
		
		return FailedAssignments.Num() ? "There were errors when trying to process localization list:\n" + FString::Join(FailedAssignments, *FString("\n")) : "Successfully assigned everything";
	}


	UFUNCTION(BlueprintCallable, CallInEditor)
	static void GenerateStringTable(const TArray<UClass*>& Classes)
	{
		auto Table = FStringTable::NewStringTable();
		for(auto Class : Classes)
		{
			if(auto Circuitry = TSubclassOf<AFGWiremodBuildable>(Class))
			{
				auto DefaultObject = Class->GetDefaultObject<AFGWiremodBuildable>();
				auto ClassName = UWiremodUtils::GetClassName(DefaultObject->GetClass()).ToString();
				
				auto KeyDisplayName = ClassName + "_DisplayName";
				auto KeyDescription = ClassName + "_Description";

				Table->SetSourceString(KeyDisplayName, DefaultObject->mDisplayName.ToString());
				Table->SetSourceString(KeyDescription, DefaultObject->mDescription.ToString());

				if(auto Multistate = TSubclassOf<AMultistateWiremodBuildable>(Circuitry))
				{
					auto States = Multistate->GetDefaultObject<AMultistateWiremodBuildable>()->States;
					for(int i = 0; i < States.Num(); i++)
					{
						auto State = States[i];
						auto KeyModeDisplayName = ClassName + "_Modes_" + FString::FromInt(i) + "_DisplayName";
						auto KeyModeDescription = ClassName + "_Modes_" + FString::FromInt(i) + "_Description";

						Table->SetSourceString(KeyModeDisplayName, State.Name.ToString());
						Table->SetSourceString(KeyModeDescription, State.Description.ToString());

						ExportMultistateConnectionList(Table, ClassName, "_Inputs_", i, State.Connections.Inputs);
						ExportMultistateConnectionList(Table, ClassName, "_Outputs_", i, State.Connections.Outputs);
					}
				}
				else
				{
					ExportConnectionList(Table, ClassName, "_Inputs_", DefaultObject->ConnectionsInfo.Inputs);
					ExportConnectionList(Table, ClassName, "_Outputs_", DefaultObject->ConnectionsInfo.Outputs);
				}
			}
		}

		Table->ExportStrings("C:/Users/SCRD/Desktop/SFModding/SatisfactoryModLoader/Plugins/FicsitWiremod/Buildables.csv");
	}


	static bool IsAGenericString(const FText& Text)
	{
		if(Text.IsEmptyOrWhitespace()) return false;
		FName Table;
		FString Key;
		return FStringTableRegistry::Get().FindTableIdAndKey(Text, Table, Key) && Key.StartsWith("GENERICLOCALE__");
	}

	static void ParseMultistateConnectionList(FName TableId, FString ClassName, FString DirectionString, int StateIndex, TArray<FBuildingConnection>& Connections, TArray<FString>& FailedAssignments)
	{
		for(int x = 0; x < Connections.Num(); x++)
		{
			auto& Connection = Connections[x];
			auto KeyStateConnectionDisplayName = ClassName + "_Modes_" + FString::FromInt(StateIndex) + DirectionString + FString::FromInt(x) + "_DisplayName";
			auto KeyStateConnectionDescription = ClassName + "_Modes_" + FString::FromInt(StateIndex) + DirectionString + FString::FromInt(x) + "_Description";

			if(!SKIP(Connection.DisplayedName))
			{
				if(ENTRY_CHK(TableId, KeyStateConnectionDisplayName)) Connection.DisplayedName = FText::FromStringTable(TableId, KeyStateConnectionDisplayName);
				else FailedAssignments.Add(KeyStateConnectionDisplayName);	
			}

			if(!SKIP(Connection.Description))
			{
				if(ENTRY_CHK(TableId, KeyStateConnectionDescription)) Connection.Description = FText::FromStringTable(TableId, KeyStateConnectionDescription);
				else FailedAssignments.Add(KeyStateConnectionDescription);	
			}
		}
	}

	static void ExportMultistateConnectionList(FStringTableRef Table, FString ClassName, FString DirectionString, int StateIndex, TArray<FBuildingConnection> Connections)
	{
		for(int x = 0; x < Connections.Num(); x++)
		{
			auto Connection = Connections[x];

			auto KeyStateConnectionDisplayName = ClassName + "_Modes_" + FString::FromInt(StateIndex) + DirectionString + FString::FromInt(x) + "_DisplayName";
			auto KeyStateConnectionDescription = ClassName + "_Modes_" + FString::FromInt(StateIndex) + DirectionString + FString::FromInt(x) + "_Description";

			if(!SKIP(Connection.DisplayedName)) Table->SetSourceString(KeyStateConnectionDisplayName, Connection.DisplayName);
			if(!SKIP(Connection.Description)) Table->SetSourceString(KeyStateConnectionDescription, Connection.Description.ToString());
		}
	}

	static void ParseConnectionList(FName TableId, FString ClassName, FString DirectionString, TArray<FBuildingConnection>& Connections, TArray<FString>& FailedAssignments)
	{
		for(int x = 0; x < Connections.Num(); x++)
		{
			auto& Connection = Connections[x];
			auto KeyStateConnectionDisplayName = ClassName + DirectionString + FString::FromInt(x) + "_DisplayName";
			auto KeyStateConnectionDescription = ClassName + DirectionString + FString::FromInt(x) + "_Description";

			if(!SKIP(Connection.DisplayedName))
			{
				if(ENTRY_CHK(TableId, KeyStateConnectionDisplayName)) Connection.DisplayedName = FText::FromStringTable(TableId, KeyStateConnectionDisplayName);
				else FailedAssignments.Add(KeyStateConnectionDisplayName);	
			}

			if(!SKIP(Connection.Description))
			{
				if(ENTRY_CHK(TableId, KeyStateConnectionDescription)) Connection.Description = FText::FromStringTable(TableId, KeyStateConnectionDescription);
				else FailedAssignments.Add(KeyStateConnectionDescription);	
			}
		}
	}

	static void ExportConnectionList(FStringTableRef Table, FString ClassName, FString DirectionString, TArray<FBuildingConnection> Connections)
	{
		for(int x = 0; x < Connections.Num(); x++)
		{
			auto Connection = Connections[x];
			auto KeyConnectionDisplayName = ClassName + DirectionString + FString::FromInt(x) + "_DisplayName";
			auto KeyConnectionDescription = ClassName + DirectionString + FString::FromInt(x) + "_Description";
			
			if(!SKIP(Connection.DisplayedName)) Table->SetSourceString(KeyConnectionDisplayName, Connection.DisplayName);
			if(!SKIP(Connection.Description)) Table->SetSourceString(KeyConnectionDescription, Connection.Description.ToString());
		}
	}
};

#endif
