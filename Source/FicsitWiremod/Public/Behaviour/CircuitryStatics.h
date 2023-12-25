// 

#pragma once

#include "CoreMinimal.h"
#include "FGDecorationTemplate.h"
#include "Module/GameInstanceModule.h"
#include "CircuitryStatics.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UCircuitryStatics : public UGameInstanceModule
{
	GENERATED_BODY()
	
public:

	static UStaticMesh* GetGateMesh(){ return Self ? Self->GateMesh : LoadObject<UStaticMesh>(NULL, *FString("/FicsitWiremod/Assets/Models/WiremodChipBase.WiremodChipBase")); }
	static UStaticMesh* GetGateDecalMesh() { return Self ? Self->GateDecalMesh : LoadObject<UStaticMesh>(NULL, *FString("/Engine/BasicShapes/Plane.Plane")); }
	static UMaterial* GetGateDecalMaterial() { return Self ? Self->GateDecalMaterial : LoadObject<UMaterial>(NULL, *FString("/FicsitWiremod/Assets/Materials/WiremodBuildableDecalMaterial.WiremodBuildableDecalMaterial")); }
	static UTexture* GetDefaultDecalTexture() { return Self ? Self->DecalDefaultTexture : LoadObject<UTexture>(NULL, *FString("/FicsitWiremod/Assets/Icons/WiremodIcon.WiremodIcon")); }
	static TSubclassOf<AFGDecorationTemplate> GetDefaultDecoratorClass()
	{
		if(Self) return Self->Decorator;
		
		return TSubclassOf<AFGDecorationTemplate>();
	}

	UFUNCTION(BlueprintImplementableEvent)
	bool GetIsEpilepsyModeOn();

	UFUNCTION(BlueprintImplementableEvent)
	void SetIsEpilepsyModeOn(bool Value);
	
	UFUNCTION(BlueprintImplementableEvent)
	FLinearColor GetDefaultWireColor();

	UFUNCTION(BlueprintImplementableEvent)
	void SetDefaultWireColor(const FLinearColor& Color);

	UFUNCTION(BlueprintImplementableEvent)
	bool GetIsWireDefaultHidden();

	UFUNCTION(BlueprintImplementableEvent)
	void SetIsWireDefaultHidden(bool Value);

	UFUNCTION(BlueprintImplementableEvent)
	double GetWireEmission();

	UFUNCTION(BlueprintImplementableEvent)
	void SetWireEmission(double Emission = 1);

	UFUNCTION(BlueprintImplementableEvent)
	double TraceDistance();
	
	UFUNCTION(BlueprintImplementableEvent)
	bool ShouldToolsKeepStateOnUnequip();
	
	UFUNCTION(BlueprintImplementableEvent)
	double GetHologramGridSize();

	inline static UCircuitryStatics* Self;
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* GateMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* GateDecalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* GateDecalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture* DecalDefaultTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AFGDecorationTemplate> Decorator;
	
	UFUNCTION(BlueprintCallable)
	void SetSelf() { Self = this; }
};

UCLASS()
class UCircuitryConfigBlueprintFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	static bool GetIsEpilepsyModeOn() { return UCircuitryStatics::Self->GetIsEpilepsyModeOn(); }

	UFUNCTION(BlueprintCallable)
	static void SetIsEpilepsyModeOn(bool Value) { return UCircuitryStatics::Self->SetIsEpilepsyModeOn(Value); } 

	UFUNCTION(BlueprintPure)
	static FLinearColor GetDefaultWireColor() { return UCircuitryStatics::Self->GetDefaultWireColor(); }

	UFUNCTION(BlueprintCallable)
	static void SetDefaultWireColor(const FLinearColor& Color) { UCircuitryStatics::Self->SetDefaultWireColor(Color); }

	UFUNCTION(BlueprintPure)
	static bool GetShouldToolsKeepStateOnUnequip() { return UCircuitryStatics::Self->ShouldToolsKeepStateOnUnequip(); }

	UFUNCTION(BlueprintPure)
	static bool GetIsWireDefaultHidden() { return UCircuitryStatics::Self->GetIsWireDefaultHidden(); }

	UFUNCTION(BlueprintCallable)
	static void SetIsWireDefaultHidden(bool Value) { UCircuitryStatics::Self->SetIsWireDefaultHidden(Value); }

	UFUNCTION(BlueprintPure)
	static double GetTraceDistance() { return UCircuitryStatics::Self->TraceDistance(); }

	UFUNCTION(BlueprintPure)
	static double GetWireEmission() { return UCircuitryStatics::Self->GetWireEmission(); }
	
	UFUNCTION(BlueprintCallable)
	static void SetWireEmission(double Emission = 1) { UCircuitryStatics::Self->SetWireEmission(Emission); }

	UFUNCTION(BlueprintPure)
	static double GetHologramGridSize() { return UCircuitryStatics::Self->GetHologramGridSize(); }
	
};

typedef UCircuitryConfigBlueprintFunctions CircuitryConfig;
