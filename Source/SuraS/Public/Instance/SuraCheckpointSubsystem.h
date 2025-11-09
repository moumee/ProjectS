// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SuraCheckpointSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCheckpointLoaded);

class USaveGame;
class USuraSaveGame;
/**
 * 
 */
UCLASS()
class SURAS_API USuraCheckpointSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USuraSaveGame> CurrentSave;

	FString CheckpointSlotName = TEXT("Checkpoint");

	void OnLoadCompleted(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:

	FOnCheckpointLoaded OnCheckpointLoadedDelegate;

	void LoadCheckpoint();
	
	void SaveCheckpoint(FName MapName, const FTransform& SpawnTransform, int32 OrderIndex);

	USuraSaveGame* GetCurrentSave() const { return CurrentSave; }

	bool HasSavedCheckpoint() const;

	/**
	 * Should clear the saved checkpoint before loading level 1 from new game button.
	 */
	UFUNCTION(BlueprintCallable)
	void ClearSavedCheckpoint();
	
};
