// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SuraCheckpointSubsystem.generated.h"

enum class ESuraVideo : uint8;
class ASuraPawnPlayer;
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
	TObjectPtr<USuraSaveGame> CurrentSave = nullptr;

	FString CheckpointSlotName = TEXT("Checkpoint");

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:

	void LoadCheckpoint();
	
	void SaveCheckpoint(FName MapName, const FTransform& SpawnTransform, int32 OrderIndex);

	USuraSaveGame* GetCurrentSave() const { return CurrentSave; }

	bool HasSavedCheckpoint() const;

	void SetCurrentSave(USuraSaveGame* SaveGame);

	/**
	 * Should clear the saved checkpoint before loading level 1 from new game button.
	 */
	UFUNCTION(BlueprintCallable)
	void ClearSavedCheckpoint();
	
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	FString GetCheckpointSlotName() const { return CheckpointSlotName; };

	UFUNCTION(BlueprintCallable)
	void SavePlayedVideo(ESuraVideo InPlayedVideo);

	UFUNCTION(BlueprintCallable)
	bool ShouldPlayVideo(ESuraVideo VideoToCheck);
};
