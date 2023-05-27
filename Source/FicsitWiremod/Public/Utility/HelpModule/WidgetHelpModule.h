// 

#pragma once

#include "CoreMinimal.h"
#include "BaseHelpModule.h"
#include "FGCharacterPlayer.h"
#include "UI/FGGameUI.h"
#include "UI/FGInteractWidget.h"
#include "WidgetHelpModule.generated.h"

/**
 * 
 */
UCLASS(DefaultToInstanced, EditInlineNew, BlueprintType)
class FICSITWIREMOD_API UWidgetHelpModule : public UBaseHelpModule
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Settings", meta=(ExposeOnSpawn = true))
	TSubclassOf<UFGInteractWidget> Widget;


	virtual void Execute_Implementation() override
	{
		//Cursed
		Cast<AFGCharacterPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->GetGameUI()->PushWidget(Widget.GetDefaultObject());
	}
};
