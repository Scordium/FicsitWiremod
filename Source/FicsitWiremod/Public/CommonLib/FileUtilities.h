// 

#pragma once

#include "CoreMinimal.h"
#include "HAL/FileManagerGeneric.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Misc/URLRequestFilter.h"
#include "FileUtilities.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API UFileUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static void FindAllFilesInDirectory(const FString& Directory, const TArray<FString>& FileTypes, TArray<FString>& OutFiles)
	{
		IFileManager& FileManager = FFileManagerGeneric::Get();
		for(auto& Type : FileTypes)
			FileManager.FindFiles(OutFiles, *Directory, *Type);
	}

	UFUNCTION(BlueprintCallable)
	static void ReadFileAsString(const FString& FilePath, FString& Out) { FFileHelper::LoadFileToString(Out, *FilePath); }

	UFUNCTION(BlueprintCallable)
	static void SaveStringAsFile(const FString& Path, const FString& FileName, const FString& File)
	{
		if(!FFileManagerGeneric::Get().DirectoryExists(*Path)) FFileManagerGeneric::Get().MakeDirectory(*Path);
		FFileHelper::SaveStringToFile(File, *FString(Path + "/" + FileName));
	}

	UFUNCTION(BlueprintCallable)
	static bool DoesFileExist(const FString& FilePath) { return FFileManagerGeneric::Get().FileExists(*FilePath); }

	UFUNCTION(BlueprintCallable)
	static bool DoesDirectoryExist(const FString& DirPath) { return FFileManagerGeneric::Get().DirectoryExists(*DirPath); }

	UFUNCTION(BlueprintCallable)
	static void OpenDirectory(const FString& DirPath)
	{
		if(!DoesDirectoryExist(DirPath)) FFileManagerGeneric::Get().MakeDirectory(*DirPath);

		UE::Core::FURLRequestFilter Filter(TEXT("SystemLibrary.LaunchURLFilter"), GEngineIni);

		FPlatformProcess::LaunchURLFiltered(*DirPath, nullptr, nullptr, Filter);
	}
};
