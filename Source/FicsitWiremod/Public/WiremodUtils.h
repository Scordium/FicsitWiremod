// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once
#include "AbstractInstanceManager.h"
#include "FGCharacterPlayer.h"
#include "FGTimeSubsystem.h"
#include "Behaviour/CircuitryInterface.h"
#include "Buildables/FGBuildable.h"
#include "Configuration/Properties/ConfigPropertySection.h"
#include "Kismet/KismetStringLibrary.h"
#include "Utility/WiremodGameWorldModule.h"

#include "WiremodUtils.generated.h"


/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UWiremodUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static AActor* GetActualHitTarget(const FHitResult& hit, FVector& Location)
	{
		Location = hit.Location;
		if(auto abstractHit = Cast<AAbstractInstanceManager>(hit.Actor))
		{
			FInstanceHandle handle;
			abstractHit->ResolveHit(hit, handle);
			return handle.GetOwner<AActor>();
		}
		return hit.Actor.Get();
	}
	
	UFUNCTION(BlueprintPure)
	static FString GetStringifiedValue(const FConnectionData& Value) { return Value.GetStringifiedValue(); }
	
	
	static FString GetModReference(UObject* object)
	{
		if(!IsValid(object)) return "";
		TArray<FString> pathParse;
		UKismetSystemLibrary::GetPathName(object).ParseIntoArray(pathParse, *FString("/"), true);
		if(pathParse.Num() == 0) return "";
		
		return pathParse[0];
	}
    
    	
	UFUNCTION(BlueprintPure)
	static FName GetClassName(UClass* inClass)
	{
		auto unparsed = UKismetSystemLibrary::GetClassDisplayName(inClass);
		TArray<FString> Remove = {"Build_", "BP_", "SF+_", "RP_", "SF_", "MP_", "FF_"};
		FString ReplaceWith = "";
		for (FString Element : Remove)
		{
			unparsed = unparsed.Replace(*Element, *ReplaceWith);
		}
    
		auto parsed = UKismetStringLibrary::GetSubstring(unparsed, 0, unparsed.Len() - 2);
    		
		return FName(parsed);
	}

	

	UFUNCTION(BlueprintCallable)
	static void CopyTextToClipboard(FString text) { FPlatformMisc::ClipboardCopy(*text); }

	UFUNCTION(BlueprintPure)
	static UTexture2D* GetTexture(AFGBuildable* Buildable)
	{
		if(!Buildable) return nullptr;
		
		if(auto Wiremod = Cast<ICircuitryProcessableInterface>(Buildable))
		{
			return Wiremod->GetTexture();
		}

		return UFGItemDescriptor::GetBigIcon(Buildable->GetBuiltWithDescriptor());
	}


	UFUNCTION(BlueprintPure)
	static FORCEINLINE float TraceDistance()
	{
		return Cast<UConfigPropertyFloat>(Cast<UConfigPropertySection>(UWiremodGameWorldModule::Self->GetConfig())->SectionProperties["WiremodTool_RaycastDistance"])->Value;
	}

	UFUNCTION(BlueprintPure)
	static UConfigPropertyArray* DefaultWireColor(FLinearColor& Color)
	{
		auto Config = Cast<UConfigPropertySection>(UWiremodGameWorldModule::Self->GetConfig());
		auto ColorProperty = Cast<UConfigPropertyArray>(Config->SectionProperties["Wire_Color"]);

		float R = Cast<UConfigPropertyFloat>(ColorProperty->Values[0])->Value;
		float G = Cast<UConfigPropertyFloat>(ColorProperty->Values[1])->Value;
		float B = Cast<UConfigPropertyFloat>(ColorProperty->Values[2])->Value;
		
		Color = FLinearColor(R, G, B);
		return ColorProperty;
	}

	UFUNCTION(BlueprintPure)
	static bool ShouldToolKeepState()
	{
		auto Config = Cast<UConfigPropertySection>(UWiremodGameWorldModule::Self->GetConfig());

		return Cast<UConfigPropertyBool>(Config->SectionProperties["WiremodTool_KeepState"])->Value;
	}

	UFUNCTION(BlueprintPure)
	static UConfigPropertyBool* WireDefaultHidden(bool& Out)
	{
		auto Config = Cast<UConfigPropertySection>(UWiremodGameWorldModule::Self->GetConfig());
		auto BoolProperty = Cast<UConfigPropertyBool>(Config->SectionProperties["Wire_Hidden"]);

		Out = BoolProperty->Value;
		return BoolProperty;
	}

	UFUNCTION(BlueprintPure)
	static UConfigPropertyFloat* WireEmission(float& Out)
	{
		auto Config = Cast<UConfigPropertySection>(UWiremodGameWorldModule::Self->GetConfig());
		auto FloatProperty = Cast<UConfigPropertyFloat>(Config->SectionProperties["Wire_Emission"]);

		Out = FloatProperty->Value;
		return FloatProperty;
	}

};
