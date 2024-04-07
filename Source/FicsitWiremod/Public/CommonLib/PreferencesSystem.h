// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystem/ModSubsystem.h"
#include "Utility/CircuitryLogger.h"
#include "PreferencesSystem.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class UScopedPreferences : public UObject
{
	GENERATED_BODY()
};

UCLASS()
class FICSITWIREMOD_API APreferencesSystem : public AModSubsystem
{
	GENERATED_BODY()

public:

	APreferencesSystem()
	{
		ReplicationPolicy = ESubsystemReplicationPolicy::SpawnOnClient;
	}

	UFUNCTION(BlueprintCallable)
	void SaveObjectScopedPreferences(UObject* Object, UScopedPreferences* Preferences)
	{
		if(Object) ObjectScopedPreferences.Add(Object, Preferences);
	}

	UFUNCTION(BlueprintCallable)
	void SaveClassScopedPreferencesFromObject(UObject* Object, UScopedPreferences* Preferences)
	{
		if(Object) ClassScopedPreferences.Add(Object->GetClass(), Preferences);
	}

	UFUNCTION(BlueprintCallable)
	void SaveClassScopedPreferencesFromClass(TSubclassOf<UObject> Class, UScopedPreferences* Preferences)
	{
		ClassScopedPreferences.Add(Class, Preferences);
	}

	UFUNCTION(BlueprintPure)
	bool GetScopedPreferences(UObject* Object, UScopedPreferences*& ClassPreferences, UScopedPreferences*& ObjectPreferences)
	{
		if(!Object) return false;
		
		auto ClassPref = ClassScopedPreferences.Find(Object->GetClass());
		ClassPreferences = ClassPref ? *ClassPref : nullptr;

		auto ObjectPref = ObjectScopedPreferences.Find(Object);
		ObjectPreferences = ObjectPref ? *ObjectPref : nullptr;
		
		return ClassPref || ObjectPref;
	}


	UPROPERTY(SaveGame)
	TMap<UObject*, UScopedPreferences*> ObjectScopedPreferences;

	UPROPERTY(SaveGame)
	TMap<TSubclassOf<UObject>, UScopedPreferences*> ClassScopedPreferences;
};
