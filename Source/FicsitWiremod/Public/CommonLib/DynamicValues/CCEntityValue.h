// 

#pragma once

#include "CoreMinimal.h"
#include "CCDynamicValueBase.h"
#include "FGCharacterPlayer.h"
#include "CCEntityValue.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCEntityValue : public UCCDynamicValueBase
{
	GENERATED_BODY()

public:
	UCCEntityValue() : Super(Entity){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCEntityValue, Value)
	}

	virtual void SetValue(const FValueReflectionSource& Source) override
	{
		if(!Source.Object) return;
		if(Source.Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCEntityValue>(IDynamicValuePasser::Execute_GetValue(Source.Object, Source.SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Source.SetByAddress(Value);
	}

	virtual bool Equals(UCCDynamicValueBase* Other) override
	{
		if(this == Other) return true;

		if(auto OtherSource = Cast<UCCEntityValue>(Other))
			return OtherSource->Value == Value;

		return false;
	}


	virtual FString ToString() override
	{
		auto ObjectName = UKismetSystemLibrary::GetObjectName(Value);
				
		if(auto Player = Cast<AFGCharacterPlayer>(Value))
		{
			//Check if the player state is valid. If the player is offline it will be null and crash if not handled properly
			if(auto State = Player->GetPlayerState()) return ObjectName + "(Player " + State->GetPlayerName() + ")";
			return ObjectName + "(Offline player)";
		}
		return ObjectName;
	}
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	AActor* Value;
};
