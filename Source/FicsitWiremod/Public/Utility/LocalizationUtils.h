// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "UObject/SavePackage.h"
#include "WiremodUtils.h"
#include "LocalizationUtils.generated.h"
/**
 * 
 */

#define ENTRY_CHK(Table, Key) UKismetStringTableLibrary::IsRegisteredTableEntry(Table, Key)
#define SKIP(Key) IsAGenericString(Key)

class AFGWiremodBuildable;

UCLASS()
class FICSITWIREMOD_API ULocalizationUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	static bool IsAGenericString(const FText& Text)
	{
		if(Text.IsEmptyOrWhitespace()) return true;
		FName Table;
		FString Key;
		return FTextInspector::GetTableIdAndKey(Text, Table, Key) && Key.StartsWith("GENERICLOCALE__");
	}
	
	UFUNCTION(BlueprintCallable, CallInEditor)
	static void GenerateStringTable(const TArray<UClass*>& Classes, FName LocTableName = FName("/FicsitWiremod/Localization/GeneratedLocTable.GeneratedLocTable"))
	{
		auto Table = FStringTableRegistry::Get().FindMutableStringTable(LocTableName);
		for(auto Class : Classes)
		{
			if(auto Circuitry = TSubclassOf<AFGWiremodBuildable>(Class))
			{
				auto DefaultObject = Class->GetDefaultObject<AFGWiremodBuildable>();
				auto ClassName = UWiremodUtils::GetClassName(DefaultObject->GetClass()).ToString() + "_";
				
				auto KeyDisplayName = ClassName + "_DisplayName";
				auto KeyDescription = ClassName + "_Description";

				Table->SetSourceString(KeyDisplayName, DefaultObject->mDisplayName.ToString());
				DefaultObject->mDisplayName = FText::FromStringTable(LocTableName, KeyDisplayName);
				
				Table->SetSourceString(KeyDescription, DefaultObject->mDescription.ToString());
				DefaultObject->mDescription = FText::FromStringTable(LocTableName, KeyDescription);

				if(auto Multistate = TSubclassOf<AMultistateWiremodBuildable>(Circuitry))
				{
					auto& States = Multistate->GetDefaultObject<AMultistateWiremodBuildable>()->States;
					for(int i = 0; i < States.Num(); i++)
					{
						auto& State = States[i];
						auto KeyModeDisplayName = ClassName + "_Modes_" + FString::FromInt(i) + "_DisplayName";
						auto KeyModeDescription = ClassName + "_Modes_" + FString::FromInt(i) + "_Description";

						if(!SKIP(State.Name))
						{
							Table->SetSourceString(KeyModeDisplayName, State.Name.ToString());
							State.Name = FText::FromStringTable(LocTableName, KeyModeDisplayName);
						}
						if(!SKIP(State.Description))
						{
							Table->SetSourceString(KeyModeDescription, State.Description.ToString());
							State.Description = FText::FromStringTable(LocTableName, KeyModeDescription);
						}

						GenerateMultistateConnectionList(Table, LocTableName, ClassName, "_Inputs_", i, State.Connections.Inputs);
						GenerateMultistateConnectionList(Table, LocTableName, ClassName, "_Outputs_", i, State.Connections.Outputs);
					}
				}
				else
				{
					GenerateConnectionList(Table, LocTableName, ClassName, "_Inputs_", DefaultObject->ConnectionsInfo.Inputs);
					GenerateConnectionList(Table, LocTableName, ClassName, "_Outputs_", DefaultObject->ConnectionsInfo.Outputs);
				}


				Circuitry->Modify(true);
			}
		}
	}

	static void GenerateMultistateConnectionList(FStringTablePtr Table, FName LocTableName, FString ClassName, FString DirectionString, int StateIndex, TArray<FBuildingConnection>& Connections)
	{
		for(int x = 0; x < Connections.Num(); x++)
		{
			auto& Connection = Connections[x];

			auto KeyStateConnectionDisplayName = ClassName + "_Modes_" + FString::FromInt(StateIndex) + DirectionString + FString::FromInt(x) + "_DisplayName";
			auto KeyStateConnectionDescription = ClassName + "_Modes_" + FString::FromInt(StateIndex) + DirectionString + FString::FromInt(x) + "_Description";

			if(!SKIP(Connection.DisplayedName))
			{
				Table->SetSourceString(KeyStateConnectionDisplayName, Connection.DisplayedName.ToString());
				Connection.DisplayedName = FText::FromStringTable(LocTableName, KeyStateConnectionDisplayName);
			}
			if(!SKIP(Connection.Description))
			{
				Table->SetSourceString(KeyStateConnectionDescription, Connection.Description.ToString());
				Connection.Description = FText::FromStringTable(LocTableName, KeyStateConnectionDescription);
			}
		}
	}

	static void GenerateConnectionList(FStringTablePtr Table, FName LocTableName, FString ClassName, FString DirectionString, TArray<FBuildingConnection>& Connections)
	{
		for(int x = 0; x < Connections.Num(); x++)
		{
			auto& Connection = Connections[x];
			
			auto KeyConnectionDisplayName = ClassName + DirectionString + FString::FromInt(x) + "_DisplayName";
			auto KeyConnectionDescription = ClassName + DirectionString + FString::FromInt(x) + "_Description";
			
			if(!SKIP(Connection.DisplayedName))
			{
				Table->SetSourceString(KeyConnectionDisplayName, Connection.DisplayedName.ToString());
				Connection.DisplayedName = FText::FromStringTable(LocTableName, KeyConnectionDisplayName);
			}
			if(!SKIP(Connection.Description))
			{
				Table->SetSourceString(KeyConnectionDescription, Connection.Description.ToString());
				Connection.Description = FText::FromStringTable(LocTableName, KeyConnectionDescription);
			}
		}
	}
};