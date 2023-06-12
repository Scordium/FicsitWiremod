// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ScreenDrawSegment.generated.h"

USTRUCT(BlueprintType)
struct FScreenDrawSegment
{
	GENERATED_BODY()


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D StartPoint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D EndPoint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FLinearColor Color;

	FScreenDrawSegment(){}
	FScreenDrawSegment(FVector2D Start, FVector2D End, FLinearColor Color)
	{
		StartPoint = Start;
		EndPoint = End;
		this->Color = Color;
	}
};
