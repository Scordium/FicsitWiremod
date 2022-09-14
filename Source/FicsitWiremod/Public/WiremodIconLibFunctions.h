// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FGIconLibrary.h"
#include "HAL/FileManagerGeneric.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WiremodIconLibFunctions.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UWiremodIconLibFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure)
	static FIconData MakeIcon(UObject* Texture, FText IconName, EIconType IconType, bool Hidden, bool SearchOnly, bool Animated)
	{
		FIconData data;

		data.Texture = Texture;
		data.IconName = IconName;
		data.IconType = IconType;
		data.Hidden = Hidden;
		data.SearchOnly = SearchOnly;
		data.Animated = Animated;

		return data;
	};

	UFUNCTION(BlueprintCallable)
	static void AddIconToCustomList(FIconData data)
	{
		UFGIconLibrary::Get()->mCustomIconData.Add(data);
	}

	/*static TArray<FString> GetAllFiles()
	{
		
	}*/
 
	
};
