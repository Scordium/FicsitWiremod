// 

#pragma once

#include "CoreMinimal.h"
#include "FGDecorationTemplate.h"
#include "HttpModule.h"
#include "Utility/CircuitryLogger.h"
#include "Interfaces/IHttpResponse.h"
#include "Module/GameInstanceModule.h"
#include "FGEquipment.h"
#include "CircuitryStatics.generated.h"


class AWiremodVanillaConnections;

UENUM(Blueprintable, BlueprintType)
enum EWireVisibilityMode
{
	OnlyTools,
	OnlyDebugger,
	Always,
	Never
};

UENUM(Blueprintable, BlueprintType)
enum ECrosscatPatronLevel
{
	Donator,
	Bronze,
	Silver,
	Gold,
	Platinum,
	Premium
};

USTRUCT(Blueprintable, BlueprintType)
struct FCrosscatPatronData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly) FString DisplayName;
	UPROPERTY(BlueprintReadOnly) TEnumAsByte<ECrosscatPatronLevel> PatronLevel;

	bool operator<(const FCrosscatPatronData& Other) const
	{
		return Other.PatronLevel < PatronLevel;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPatronsListFetched, const TArray<FCrosscatPatronData>&, Data, bool, Success);
/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UCircuitryStatics : public UGameInstanceModule
{
	GENERATED_BODY()
	
public:
	static inline const FString CircuitryWiki = "https://cc.gotpa.ws/wiki";
	static inline const FString PatronsListBaseUrl = "https://gotpa.ws/patrons/";
	static inline const FString ApiListsUrl = "https://cc.gotpa.ws/packages/";

	static UStaticMesh* GetGateMesh(){ return Self ? Self->GateMesh : LoadObject<UStaticMesh>(NULL, *FString("/FicsitWiremod/Assets/Models/WiremodChipBase.WiremodChipBase")); }
	static UStaticMesh* GetGateDecalMesh() { return Self ? Self->GateDecalMesh : LoadObject<UStaticMesh>(NULL, *FString("/Engine/BasicShapes/Plane.Plane")); }
	static UMaterial* GetGateDecalMaterial() { return Self ? Self->GateDecalMaterial : LoadObject<UMaterial>(NULL, *FString("/FicsitWiremod/Assets/Materials/WiremodBuildableDecalMaterial.WiremodBuildableDecalMaterial")); }
	
	UFUNCTION(BlueprintImplementableEvent)
	FLinearColor GetDefaultWireColor();

	UFUNCTION(BlueprintImplementableEvent)
	void SetDefaultWireColor(const FLinearColor& Color);

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

	UFUNCTION(BlueprintImplementableEvent)
	AWiremodVanillaConnections* GetVanillaConnectionsSubsystem();

	UFUNCTION(BlueprintImplementableEvent)
	void SetWireVisibility(EWireVisibilityMode Mode);

	UFUNCTION(BlueprintImplementableEvent)
	EWireVisibilityMode GetWireVisibility();

	UFUNCTION()
	bool GetWireIsCurrentlyVisible() const { return ShowWires; }

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCurrentEquipment(AFGEquipment* NewEquipment);
	
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

	UPROPERTY(BlueprintReadWrite)
	AFGEquipment* CurrentEquipment = nullptr;

	UPROPERTY(BlueprintReadWrite)
	bool ShowWires;
	
	UFUNCTION(BlueprintCallable)
	void SetSelf() { Self = this; }

	UFUNCTION(BlueprintCallable)
	void FetchPatronsList()
	{
		auto Request = FHttpModule::Get().CreateRequest();
		
		Request->SetURL(PatronsListBaseUrl);
		Request->OnProcessRequestComplete().BindUObject(this, &UCircuitryStatics::OnPatronsListFetchFinished);
		Request->SetVerb("GET");
		Request->SetTimeout(60);

		if(!Request->ProcessRequest())
		{
			OnPatronsListFetched.Broadcast(TArray<FCrosscatPatronData>(), false);
		}
	}

	void OnPatronsListFetchFinished(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success)
	{
		if(!Success)
		{
			FString ErrorText;

			if (Response.IsValid())
				ErrorText = Response->GetContentAsString() + " (" + FString::FromInt(Response->GetResponseCode()) + ")";
			else
				ErrorText = "CURL responded with invalid data.";
			
			ACircuitryLogger::DispatchErrorEvent("Failed to fetch patrons: " + ErrorText); 
			OnPatronsListFetched.Broadcast(TArray<FCrosscatPatronData>(), false);
			return;
		}
		
		auto Json = Response->GetContentAsString();
		TSharedPtr<FJsonValue> Object;
		
		FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Json), Object);

		TArray<FCrosscatPatronData> Patrons;

		for(auto& JsonPatronData : Object->AsArray())
		{
			FCrosscatPatronData Data;
			Data.DisplayName = JsonPatronData->AsObject()->GetStringField("DisplayName");
			Data.PatronLevel = (ECrosscatPatronLevel)JsonPatronData->AsObject()->GetIntegerField("Level");

			Patrons.Add(Data);
		}

		Patrons.Sort();
		OnPatronsListFetched.Broadcast(Patrons, true);
	}

	UPROPERTY(BlueprintAssignable)
	FOnPatronsListFetched OnPatronsListFetched;
};

UCLASS()
class UCircuitryConfigBlueprintFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	static AWiremodVanillaConnections* GetVanillaConnectionSubsystem() { return UCircuitryStatics::Self->GetVanillaConnectionsSubsystem(); }
	
	UFUNCTION(BlueprintPure)
	static FLinearColor GetDefaultWireColor() { return UCircuitryStatics::Self->GetDefaultWireColor(); }

	UFUNCTION(BlueprintCallable)
	static void SetDefaultWireColor(const FLinearColor& Color) { UCircuitryStatics::Self->SetDefaultWireColor(Color); }

	UFUNCTION(BlueprintPure)
	static bool GetShouldToolsKeepStateOnUnequip() { return UCircuitryStatics::Self->ShouldToolsKeepStateOnUnequip(); }

	UFUNCTION(BlueprintPure)
	static double GetTraceDistance() { return UCircuitryStatics::Self->TraceDistance(); }

	UFUNCTION(BlueprintPure)
	static double GetWireEmission() { return UCircuitryStatics::Self->GetWireEmission(); }
	
	UFUNCTION(BlueprintCallable)
	static void SetWireEmission(double Emission = 1) { UCircuitryStatics::Self->SetWireEmission(Emission); }

	UFUNCTION(BlueprintPure)
	static double GetHologramGridSize() { return UCircuitryStatics::Self->GetHologramGridSize(); }

	UFUNCTION(BlueprintPure)
	static EWireVisibilityMode GetWireVisibilityMode() { return UCircuitryStatics::Self->GetWireVisibility(); }

	UFUNCTION(BlueprintCallable)
	static void SetWireVisibilityMode(EWireVisibilityMode Mode) { return UCircuitryStatics::Self->SetWireVisibility(Mode); }

	UFUNCTION(BlueprintPure)
	static bool ShouldWireBeVisible() { return UCircuitryStatics::Self->GetWireIsCurrentlyVisible(); }

	UFUNCTION(BlueprintPure)
	static FString GetWikiUrl() { return UCircuitryStatics::CircuitryWiki; }
};

typedef UCircuitryConfigBlueprintFunctions CircuitryConfig;
