#pragma once
#include "Behaviour/CircuitryConnectionsProvider.h"
#include "WikiParser.generated.h"

USTRUCT(Blueprintable)
struct FModuleConnection
{
	GENERATED_BODY()

public:
	FModuleConnection(){}

	FModuleConnection(const FBuildingConnection& BuildingConnection)
	{
		Name = BuildingConnection.DisplayedName.ToString();
		Description = BuildingConnection.Description.ToString();
		Type = UEnum::GetValueAsString(BuildingConnection.ConnectionType.GetValue());
	}
	
	UPROPERTY()
	FString Name;

	UPROPERTY()
	FString Description;

	UPROPERTY()
	FString Type;
};

USTRUCT(Blueprintable)
struct FModuleMode
{
	GENERATED_BODY()

public:

	FModuleMode(){}

	UPROPERTY()
	FString Name;

	UPROPERTY()
	FString Description;

	UPROPERTY()
	FString Image;

	UPROPERTY()
	bool ResetsInputs = false;

	UPROPERTY()
	TArray<FModuleConnection> Inputs;

	UPROPERTY()
	TArray<FModuleConnection> Outputs;
};

USTRUCT(Blueprintable)
struct FModuleDocumentation
{
	GENERATED_BODY()

public:

	FModuleDocumentation(){}

	UPROPERTY()
	FString ClassName;

	UPROPERTY()
	FString ModuleName;

	UPROPERTY()
	FString ModuleDescription;

	UPROPERTY()
	FString ModuleImage;

	UPROPERTY()
	TArray<FModuleMode> Modes;
	
};

UCLASS()
class FICSITWIREMOD_API UCircuitryWikiParser : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void SaveAssets(TMap<AFGWiremodBuildable*, UTexture2D*> Buildables);

};