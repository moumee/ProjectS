// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SuraCheckpointSubsystem.generated.h"

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
	TObjectPtr<USuraSaveGame> CurrentSave;

	FString CheckpointSlotName = TEXT("Checkpoint");

	void OnLoadCompleted(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;


	/** [추가] 종료 시 사용할 World와 PlayerPawn을 미리 캐시합니다. */
	TWeakObjectPtr<UWorld> CachedWorld;
	TWeakObjectPtr<ASuraPawnPlayer> CachedPlayerPawn;

public:

	FOnCheckpointLoaded OnCheckpointLoadedDelegate;

	void LoadCheckpoint();
	
	void SaveCheckpoint(FName MapName, const FTransform& SpawnTransform, int32 OrderIndex);

	USuraSaveGame* GetCurrentSave() const { return CurrentSave; }

	bool HasSavedCheckpoint() const;

	void SetCurrentSave(USuraSaveGame* SaveGame);

	/**
	 * Should clear the saved checkpoint before loading level 1 from new game button.
	 */
	void ClearSavedCheckpoint();

	//suhyeon
	void SaveOnQuit(); // save when shutdown

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	FString GetCheckpointSlotName() const {return CheckpointSlotName;};

	/** [추가] PlayerPawn이 스폰될 때 호출할 등록 함수입니다. */
	void RegisterPlayerAndWorld(ASuraPawnPlayer* PlayerPawn, UWorld* World);
	
};
