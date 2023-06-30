// 

#pragma once

#include "CoreMinimal.h"
#include "CommonLib/ConnectionType.h"
#include "Engine/DataTable.h"
#include "UObject/Interface.h"
#include "Utility/HelpModule/BaseHelpModule.h"
#include "CircuitryConnectionsProvider.generated.h"

USTRUCT(BlueprintType)
struct FBuildingConnection : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, meta=(DeprecatedProperty=true))
	FString DisplayName;

	/**
	 * Display name for this connection that circuitry will display to the user
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FText DisplayedName = FText::FromString(DisplayName);

	/**
	 * Function/property name that circuitry will call/read to get/set the value
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FName FunctionName;
	
    /**
     * The type of value this function/property returns/stores
     * Setting this to an incorrect type might cause crashes
     */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TEnumAsByte<EConnectionType> ConnectionType = Unknown;

	/**
	 * Whether circuitry should skip trying to find the function with "FunctionName" and just directly try to get the value from property.
	 * Circuitry will try to find a property with such name anyway in case a function was not found.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool FromProperty = false;
	
	/**
	 * Description that the user will see in the UI, pretty useless as localization is not possible unless set up by the dev.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, meta=(MultiLine="true"))
	FText Description;


	/* 
	 * Module that should be executed when trying to receive help/tips for this connection
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UBaseHelpModule* HelpInfo = nullptr;
	
	FBuildingConnection(){};

	FBuildingConnection(FString DisplayName, FString FunctionName, EConnectionType ConnectionType, bool IsProperty = false)
	{
		this->DisplayName = DisplayName;
		this->FunctionName = FName(FunctionName);
		this->ConnectionType = ConnectionType;
		this->FromProperty = IsProperty;
	}
};

USTRUCT(BlueprintType)
struct FBuildableNote
{
	GENERATED_BODY()

	/**
	 * Text for the note, pretty useless as localization is not possible unless set up by the dev.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(MultiLine="true"))
	FText Text;

	/**
	 * Color for the note
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor Color1 = FLinearColor(0.783538, 0.291771, 0.059511, 1);

	/**
	 * Whether note should "blink". This will be ignored if the user enabled photoepilepsy mode.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool Animated = false;
};

USTRUCT(BlueprintType)
struct FBuildingConnections : public FTableRowBase
{
	GENERATED_BODY()

	/**
	 * The inputs this building has.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FBuildingConnection> Inputs;


	/**
	 * The outputs this building has.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FBuildingConnection> Outputs;

	/**
	 * Fill with the mod reference and name of the buildable, if this buildable uses a list of connections that already exists.
	 * Schema: ModRef__BuildableName.
	 * Note: You need to use double underscore (`_`) to separate mod reference from buildable name
	 * Note: Buildable name should not contain the following prefixes: "Build_", "BP_". For example if your buildable is called "Build_SuperCoolSmelter", you should only leave the "SuperCoolSmelter" part.
	 * Example: Satisfactory's mod reference is "Game"
	 * Reduces the amount of work required to update each list when a new output/input is added to the vanilla buildings.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FName RedirectTo;

	/**
	 * Note to show to the user when they aim at the buildable
	 * Useless to other mod devs unless shared with wiremod developer.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FBuildableNote Note;


	/* 
	 * Module that should be executed when trying to receive help/tips for this buildable
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UBaseHelpModule* HelpInfo = nullptr;
};


// This class does not need to be modified.
UINTERFACE()
class UCircuitryConnectionsProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FICSITWIREMOD_API ICircuitryConnectionsProvider
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FBuildingConnection> GetConnectionsInfo(EConnectionDirection direction, int& Count, FBuildableNote& Note);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FBuildingConnection> GetConnections(EConnectionDirection direction);
};
