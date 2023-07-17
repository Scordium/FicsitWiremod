// 

#pragma once

#include "CoreMinimal.h"
#include "Input/FGInputMappingContext.h"
#include "CircuitryInputMappings.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UCircuitryInputMappings : public UFGInputMappingContext
{
	GENERATED_BODY()

public:

	///Primary key, LMB by default
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* PrimaryKey;
	
	///Secondary key, RMB by default
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* SecondaryKey;

	///Scroll down key, Shift + Mouse Wheel Down by default
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* ScrollDown;

	///Scroll up key, Shift + Mouse Wheel Up by default
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* ScrollUp;
	
	///Aux key, R by default
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* AuxKey;

	///Alt action 1, Alt + Primary key by default
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* AltAction1;

	///Alt action 2, Alt + Secondary key by default
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* AltAction2;

	///Special action 1, Alt + Q by default
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* SpecialAction1;

	///Special action 2, Alt + E by default
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* SpecialAction2;
};

UCLASS()
class UCircuitryBaseMappingContext : public UFGInputMappingContext
{
	GENERATED_BODY()

public:

	///Tools key, G for 0.1 seconds by default
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* OpenTools;
};
