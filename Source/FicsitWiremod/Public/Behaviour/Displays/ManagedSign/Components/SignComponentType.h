// 

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum ESignComponentType
{
	Generic,
	Text,
	Image,
	Interactive,
	Animated,
	Other,
	Integrated,
	SCT_MAX UMETA(Hidden)
};
