// 

#pragma once

#include "CoreMinimal.h"
#include "ConnectionWireBase.h"
#include "FGSchematicManager.h"
#include "Configuration/ModConfiguration.h"
#include "ModLoading/ModLoadingLibrary.h"
#include "Module/GameWorldModule.h"
#include "Util/SemVersion.h"
#include <Kismet/RuntimeBlueprintFunctionLibrary.h>
#include "WiremodGameWorldModule.generated.h"


USTRUCT()
struct FModlockedSchematic
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int64 MajorVersion;

	UPROPERTY(EditDefaultsOnly)
	int64 MinorVersion;
	
	UPROPERTY(EditDefaultsOnly)
	int64 Patch;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFGSchematic> Schematic;
};

/**
 * 
 */
UCLASS(BlueprintType)
class FICSITWIREMOD_API UWiremodGameWorldModule : public UGameWorldModule
{
	GENERATED_BODY()
	
public:
	inline static UWiremodGameWorldModule* Self;
	
	virtual void DispatchLifecycleEvent(ELifecyclePhase Phase) override
	{
		Super::DispatchLifecycleEvent(Phase);
		Self = this;
		
		if(Phase == ELifecyclePhase::POST_INITIALIZATION)
		{
			GetWorld()->SpawnActor(DebuggerGimbal, &FTransform::Identity);
			WirePreviewActor = Cast<AConnectionWireBase>(GetWorld()->SpawnActor(AConnectionWireBase::StaticClass(), &FTransform::Identity));
			WirePreviewActor->PreviewMode = true;
			
			RegisterModLockedRecipes();
			SearchForInvalidClasses();
		}
	}

	void RegisterModLockedRecipes()
	{
		auto Player = UGameplayStatics::GetPlayerCharacter(this, 0);
		if(!Player || !Player->HasAuthority()) return;

		auto ModLib = GEngine->GetEngineSubsystem<UModLoadingLibrary>();
		auto SchematicManager = AFGSchematicManager::Get(this);
		for(auto ModRef : ModLockedSchematics)
		{
			FModInfo Mod;
			if(!ModLib->GetLoadedModInfo(ModRef.Key, Mod)) continue;

			FVersion Version = FVersion(
				ModRef.Value.MajorVersion,
				ModRef.Value.MinorVersion,
				ModRef.Value.Patch
				);
			
			if(!Compare(Version, Mod.Version)) continue;

			if(ModRef.Value.Schematic.GetDefaultObject())
				SchematicManager->GiveAccessToSchematic(ModRef.Value.Schematic, Cast<AFGCharacterPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0)));
		}
	}

	static FORCEINLINE bool Compare(const FVersion& Version, const FVersion& Other)
	{
		return FVersionComparator(EVersionComparisonOp::GREATER_EQUALS, Version).Matches(Other);
	}

	void SearchForInvalidClasses()
	{
		auto Player = UGameplayStatics::GetPlayerCharacter(this, 0);
		if(!Player || !Player->HasAuthority()) return;
		if(DeprecatedClasses.Num() == 0) return;

		TArray<FText> Result;
		for(auto deprecatedClass : DeprecatedClasses)
		{
			if(auto actor = Cast<AFGBuildable>(UGameplayStatics::GetActorOfClass(this, deprecatedClass)))
			{
				Result.Add(actor->mDisplayName);
			}
		}

		if(Result.Num() > 0) CreateWiremodDeprecationNotifier(Result);
	}
	
	UFUNCTION(BlueprintImplementableEvent)
	void CreateWiremodDeprecationNotifier(const TArray<FText>& Deprecated);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> DebuggerGimbal;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AConnectionWireBase> WirePreviewActorClass;

	UPROPERTY()
	AConnectionWireBase* WirePreviewActor;

	FORCEINLINE UConfigProperty* GetConfig() const
	{
		return URuntimeBlueprintFunctionLibrary::GetModConfigurationPropertyByClass(WiremodConfig);
	}

protected:
	UPROPERTY(EditDefaultsOnly)
	TMap<FString, FModlockedSchematic> ModLockedSchematics;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AFGBuildable>> DeprecatedClasses;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UModConfiguration> WiremodConfig;
};
