// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "Internationalization/StringTableCore.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetStringTableLibrary.h"
#include "LocalizationUtils.generated.h"

#define ENTRY_CHK(Table, Key) UKismetStringTableLibrary::IsRegisteredTableEntry(Table, Key)
/**
 * 
 */

#if WITH_EDITOR

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
					auto States = Multistate->GetDefaultObject<AMultistateWiremodBuildable>()->States;
					for(int i = 0; i < States.Num(); i++)
					{
						auto& State = States[i];
						auto KeyModeDisplayName = ClassName + "_Modes_" + FString::FromInt(i) + "_DisplayName";
						auto KeyModeDescription = ClassName + "_Modes_" + FString::FromInt(i) + "_Description";

						if(ENTRY_CHK(LocTableName, KeyModeDisplayName)) State.Name = FText::FromStringTable(LocTableName, KeyModeDisplayName);
						else FailedAssignments.Add(KeyModeDisplayName);
						
						if(ENTRY_CHK(LocTableName, KeyModeDescription)) State.Description = FText::FromStringTable(LocTableName, KeyModeDescription);
						else FailedAssignments.Add(KeyModeDescription);

						for(int x = 0; x < State.Connections.Inputs.Num(); x++)
						{
							auto& Input = State.Connections.Inputs[x];

							auto KeyStateConnectionDisplayName = ClassName + "_Modes_" + FString::FromInt(i) + "_Inputs_" + FString::FromInt(x) + "_DisplayName";
							auto KeyStateConnectionDescription = ClassName + "_Modes_" + FString::FromInt(i) + "_Inputs_" + FString::FromInt(x) + "_Description";

							//TODO
							//if(ENTRY_CHK(LocTableName, KeyStateConnectionDisplayName)) Input.DisplayName = FText::FromStringTable(LocTableName, KeyStateConnectionDisplayName);
							//else FailedAssignments.Add(KeyStateConnectionDisplayName);

							if(ENTRY_CHK(LocTableName, KeyStateConnectionDescription)) Input.Description = FText::FromStringTable(LocTableName, KeyStateConnectionDescription);
							else FailedAssignments.Add(KeyStateConnectionDescription);
						}

						for(int x = 0; x < State.Connections.Outputs.Num(); x++)
						{
							auto& Output = State.Connections.Outputs[x];

							auto KeyStateConnectionDisplayName = ClassName + "_Modes_" + FString::FromInt(i) + "_Outputs_" + FString::FromInt(x) + "_DisplayName";
							auto KeyStateConnectionDescription = ClassName + "_Modes_" + FString::FromInt(i) + "_Outputs_" + FString::FromInt(x) + "_Description";

							//TODO
							//if(ENTRY_CHK(LocTableName, KeyStateConnectionDisplayName)) Input.DisplayName = FText::FromStringTable(LocTableName, KeyStateConnectionDisplayName);
							//else FailedAssignments.Add(KeyStateConnectionDisplayName);

							if(ENTRY_CHK(LocTableName, KeyStateConnectionDescription)) Output.Description = FText::FromStringTable(LocTableName, KeyStateConnectionDescription);
							else FailedAssignments.Add(KeyStateConnectionDescription);
						}
					}
				}
				else
				{
					for(int x = 0; x < DefaultObject->ConnectionsInfo.Inputs.Num(); x++)
					{
						auto& Input = DefaultObject->ConnectionsInfo.Inputs[x];

						auto KeyConnectionDisplayName = ClassName + "_Inputs_" + FString::FromInt(x) + "_DisplayName";
						auto KeyConnectionDescription = ClassName + "_Inputs_" + FString::FromInt(x) + "_Description";

						//TODO
						//if(ENTRY_CHK(LocTableName, KeyStateConnectionDisplayName)) Input.DisplayName = FText::FromStringTable(LocTableName, KeyStateConnectionDisplayName);
						//else FailedAssignments.Add(KeyStateConnectionDisplayName);

						if(ENTRY_CHK(LocTableName, KeyConnectionDescription)) Input.Description = FText::FromStringTable(LocTableName, KeyConnectionDescription);
						else FailedAssignments.Add(KeyConnectionDescription);
					}

					for(int x = 0; x < DefaultObject->ConnectionsInfo.Outputs.Num(); x++)
					{
						auto& Output = DefaultObject->ConnectionsInfo.Outputs[x];

						auto KeyConnectionDisplayName = ClassName + "_Outputs_" + FString::FromInt(x) + "_DisplayName";
						auto KeyConnectionDescription = ClassName + "_Outputs_" + FString::FromInt(x) + "_Description";

						//TODO
						//if(ENTRY_CHK(LocTableName, KeyStateConnectionDisplayName)) Input.DisplayName = FText::FromStringTable(LocTableName, KeyStateConnectionDisplayName);
						//else FailedAssignments.Add(KeyStateConnectionDisplayName);

						if(ENTRY_CHK(LocTableName, KeyConnectionDescription)) Output.Description = FText::FromStringTable(LocTableName, KeyConnectionDescription);
						else FailedAssignments.Add(KeyConnectionDescription);
					}
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

						for(int x = 0; x < State.Connections.Inputs.Num(); x++)
						{
							auto Input = State.Connections.Inputs[x];

							auto KeyStateConnectionDisplayName = ClassName + "_Modes_" + FString::FromInt(i) + "_Inputs_" + FString::FromInt(x) + "_DisplayName";
							auto KeyStateConnectionDescription = ClassName + "_Modes_" + FString::FromInt(i) + "_Inputs_" + FString::FromInt(x) + "_Description";

							Table->SetSourceString(KeyStateConnectionDisplayName, Input.DisplayName);
							Table->SetSourceString(KeyStateConnectionDescription, Input.Description.ToString());
						}

						for(int x = 0; x < State.Connections.Outputs.Num(); x++)
						{
							auto Output = State.Connections.Outputs[x];

							auto KeyStateConnectionDisplayName = ClassName + "_Modes_" + FString::FromInt(i) + "_Outputs_" + FString::FromInt(x) + "_DisplayName";
							auto KeyStateConnectionDescription = ClassName + "_Modes_" + FString::FromInt(i) + "_Outputs_" + FString::FromInt(x) + "_Description";

							Table->SetSourceString(KeyStateConnectionDisplayName, Output.DisplayName);
							Table->SetSourceString(KeyStateConnectionDescription, Output.Description.ToString());
						}
					}
				}
				else
				{
					for(int x = 0; x < DefaultObject->ConnectionsInfo.Inputs.Num(); x++)
					{
						auto Input = DefaultObject->ConnectionsInfo.Inputs[x];

						auto KeyConnectionDisplayName = ClassName + "_Inputs_" + FString::FromInt(x) + "_DisplayName";
						auto KeyConnectionDescription = ClassName + "_Inputs_" + FString::FromInt(x) + "_Description";

						Table->SetSourceString(KeyConnectionDisplayName, Input.DisplayName);
						Table->SetSourceString(KeyConnectionDescription, Input.Description.ToString());
					}

					for(int x = 0; x < DefaultObject->ConnectionsInfo.Outputs.Num(); x++)
					{
						auto Output = DefaultObject->ConnectionsInfo.Outputs[x];

						auto KeyConnectionDisplayName = ClassName + "_Outputs_" + FString::FromInt(x) + "_DisplayName";
						auto KeyConnectionDescription = ClassName + "_Outputs_" + FString::FromInt(x) + "_Description";

						Table->SetSourceString(KeyConnectionDisplayName, Output.DisplayName);
						Table->SetSourceString(KeyConnectionDescription, Output.Description.ToString());
					}
				}
			}
		}

		Table->ExportStrings("C:/Users/SCRD/Desktop/SFModding/SatisfactoryModLoader/Plugins/FicsitWiremod/Buildables.csv");
	}
};

#endif
