// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckAssetCommandlet.h"
#include "UObject/Object.h"
#include "UObject/Package.h"

UCheckAssetCommandlet::UCheckAssetCommandlet()
{
	IsClient = false;
	IsServer = false;
	IsEditor = true; // Needs editor data to load assets
	LogToConsole = true;
}

int32 UCheckAssetCommandlet::Main(const FString& Params)
{
	FString FilePath;
	if (!FParse::Value(*Params, TEXT("FilePath="), FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("No -FilePath= specified. Usage: -run=CheckAsset -FilePath=\"/Game/Path/To/Asset.Asset\""));
		return 1; // Return 1 for failure
	}

	// Convert the file path to a package path
	// Example: "D:/MyProject/Content/Blueprints/BP_MyActor.uasset" -> "/Game/Blueprints/BP_MyActor"
	FString PackagePath;
	if (!FPackageName::TryConvertFilenameToLongPackageName(FilePath, PackagePath))
		// C:\MyProject\Content\Blueprints\BP_MyActor.uasset
		{
			UE_LOG(LogTemp, Error, TEXT("Could not convert file path to package path: %s"), *FilePath);
			return 1;
		}

	UE_LOG(LogTemp, Log, TEXT("Attempting to load package: %s"), *PackagePath);

	UPackage* Package = LoadPackage(nullptr, *PackagePath, LOAD_None);

	if (Package)
	{
		// Optional: You can try to load the primary asset from the package
		// UObject* Asset = Package->FindAssetInPackage();
		// if(Asset) { ... }
        
		UE_LOG(LogTemp, Log, TEXT("SUCCESS: Package loaded successfully."));
		return 0; // Return 0 for SUCCESS!
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("FAILURE: Failed to load package. Asset is likely corrupt."));
		return 1; // Return 1 for FAILURE!
	}
}