#pragma once
#include "CommonLib/ConnectionType.h"
#include "SignBindingsProvider.generated.h"

USTRUCT(BlueprintType, meta=(HasNativeMake="FicsitWiremod.SignBindingsFunctions.CreateBinding"))
struct FManagedSignConnectionSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FString Name;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	FName InternalName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TEnumAsByte<EConnectionType> Type;


	bool operator==(const FManagedSignConnectionSettings& Other) const
	{
		return Name == Other.Name && Type == Other.Type;
	}
};

UCLASS(Blueprintable, BlueprintType)
class USignBindingPayload : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	FManagedSignConnectionSettings Data;
	
};

UINTERFACE(BlueprintType, Blueprintable)
class USignComponentVariableBindingsProvider : public UInterface
{
	GENERATED_BODY()
};

class ISignComponentVariableBindingsProvider
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	TArray<FManagedSignConnectionSettings> GetAllBindings();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	TArray<FManagedSignConnectionSettings> GetAllBindingsOfType(EConnectionType Type);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	FManagedSignConnectionSettings GetBindingByInternalName(FName InternalName);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CreateBinding(const FString& Name, EConnectionType Type, FManagedSignConnectionSettings& OutBinding);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool DoesBindingExist(const FString& Name);
};


UCLASS()
class USignBindingsFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	static FString GetBindingPrefixString() { return "\u25C8"; }

	UFUNCTION(BlueprintPure)
	static FName GetBindingNameFromString(const FString& ValueString)
	{
		return FName(ValueString.Replace(*GetBindingPrefixString(), *FString()));
	}

	UFUNCTION(BlueprintPure)
	static bool IsBinding(const FString& ValueString) { return ValueString.StartsWith(GetBindingPrefixString()); }

	UFUNCTION(BlueprintCallable)
	static FManagedSignConnectionSettings ResolveBinding(TScriptInterface<ISignComponentVariableBindingsProvider> Provider, const FString& ValueString, bool& Success)
	{
		if(!IsBinding(ValueString))
		{
			Success = false;
			return FManagedSignConnectionSettings();
		}

		auto BindingName = GetBindingNameFromString(ValueString);
		
		auto Bindings = ISignComponentVariableBindingsProvider::Execute_GetAllBindings(Provider.GetObject());

		for(auto& Binding : Bindings)
		{
			if(Binding.InternalName == BindingName)
			{
				Success = true;
				return Binding;
			}
		}

		Success = false;
		return FManagedSignConnectionSettings();
	}


	UFUNCTION(BlueprintPure)
	static FManagedSignConnectionSettings CreateBinding(const FString& Name, EConnectionType Type)
	{
		auto Out = FManagedSignConnectionSettings();
		Out.Name = Name;
		Out.Type = Type;
		Out.InternalName = FName(FGuid::NewGuid().ToString());

		return Out;
	}
};

