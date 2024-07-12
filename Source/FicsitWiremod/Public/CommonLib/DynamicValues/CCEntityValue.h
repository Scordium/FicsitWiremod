// 

#pragma once

#include "CoreMinimal.h"
#include "CCDynamicValueBase.h"
#include "CCArrayValueBase.h"
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

	virtual void FromConnectionValue(UObject* Object, FName SourceName, bool FromProperty) override
	{
		if(!Object) return;
		if(Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCEntityValue>(IDynamicValuePasser::Execute_GetValue(Object, SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Value = UReflectionUtilities::GetEntity(REFLECTION_ARGS);
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


UCLASS(Blueprintable,BlueprintType)
class FICSITWIREMOD_API UCCEntityArrayValue : public UCCArrayValueBase
{
	GENERATED_BODY()

public:
	UCCEntityArrayValue() : Super(ArrayOfEntity){}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(UCCEntityArrayValue, Value)
	}

	virtual void FromConnectionValue(UObject* Object, FName SourceName, bool FromProperty) override
	{
		if(!Object) return;
		if(Object->GetClass()->ImplementsInterface(IDynamicValuePasser::UClassType::StaticClass()))
			if(auto SameType = Cast<UCCEntityArrayValue>(IDynamicValuePasser::Execute_GetValue(Object, SourceName.ToString())))
			{
				Value = SameType->Value;
				return;
			}
		
		Value = UReflectionUtilities::GetEntityArray(REFLECTION_ARGS);
	}

	virtual bool Equals(UCCDynamicValueBase* Other) override
	{
		if(this == Other) return true;

		if(auto OtherSource = Cast<UCCEntityArrayValue>(Other))
			return OtherSource->Value == Value;

		return false;
	}

	virtual void AddElement(const FConnectionData& Element) override{ Value.Add(Element.GetEntity()); }
	virtual UCCDynamicValueBase* GetElement(int Index, UObject* Outer) override
	{
		if(!Value.IsValidIndex(Index)) return nullptr;
		
		UCCEntityValue* ValueOut;
		if(auto OuterCast = Cast<UCCEntityValue>(Outer)) ValueOut = OuterCast;
		else ValueOut = NewObject<UCCEntityValue>(this->GetWorld()->PersistentLevel);
		
		ValueOut->Value = Value[Index];
		return ValueOut;
	}
	
	virtual void InsertElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;

		Value.Insert(Element.GetEntity(), Index);
	}
	virtual void Clear() override{ Value.Empty(); }
	virtual int Length() override { return Value.Num(); }
	virtual void RemoveElement(int Index) override { if(Value.IsValidIndex(Index)) Value.RemoveAt(Index); }
	virtual void Resize(int NewSize) override { Value.SetNum(NewSize); }
	virtual void SetElement(const FConnectionData& Element, int Index) override
	{
		if(!Value.IsValidIndex(Index)) return;
		Value[Index] = Element.GetEntity();
	}
	virtual bool Contains(const FConnectionData& Element) override { return Value.Contains(Element.GetEntity()); }

	virtual FString ToString() override { return FString::Join(ToStringArray(), *FString(", ")); }

	virtual TArray<FString> ToStringArray() override
	{
		TArray<FString> Out;

		for(const auto& Val : Value)
		{
			auto ObjectName = UKismetSystemLibrary::GetObjectName(Val);
				
			if(auto Player = Cast<AFGCharacterPlayer>(Val))
			{
				//Check if the player state is valid. If the player is offline it will be null and crash if not handled properly
				if(auto State = Player->GetPlayerState()) Out.Add("Player " + State->GetPlayerName());
				else Out.Add(UKismetSystemLibrary::GetObjectName(Player) + "(Offline player)");
			}
			else Out.Add(UKismetSystemLibrary::GetObjectName(Val));
		}

		return Out;
	}

	virtual int FindFirst(const FConnectionData& Element) override
	{
		auto SearchValue = Element.GetEntity();
		for(int i = 0; i < Value.Num(); i++)
		{
			if(Value[i] == SearchValue) return i; 
		}

		return -1;
	}

	virtual TArray<double> FindAll(const FConnectionData& Element) override
	{
		auto Out = TArray<double>();
		auto SearchValue = Element.GetEntity();
		for(int i = 0; i < Value.Num(); i++)
		{
			if(Value[i] == SearchValue) Out.Add(i);
		}

		return Out;
	}

	virtual void AppendArray(const UCCArrayValueBase* Array) override
	{
		if(auto ThisArray = Cast<ThisClass>(Array))
		{
			Value.Append(ThisArray->Value);
		}
	}
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	TArray<AActor*> Value;
};
