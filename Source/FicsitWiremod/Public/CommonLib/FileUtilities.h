// 

#pragma once

#include "CoreMinimal.h"
#include "HAL/FileManagerGeneric.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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
};
