// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SuraLevelGameMode.generated.h"

class ASuraPawnPlayer;
/**
 * 
 */
UCLASS()
class SURAS_API ASuraLevelGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	bool bPlayFromHere = false;

	UPROPERTY(EditDefaultsOnly)
	float PlayerFellRespawnHealthReduction = 10.f;

	UPROPERTY(EditDefaultsOnly)
	float PlayerMinimumRespawnHealth = 20.f;
	
public:

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
	virtual void BeginPlay() override;
	
	void RespawnToLastCheckpoint(ASuraPawnPlayer* Player);

	void OnPlayerFellOutOfWorld(ASuraPawnPlayer* Player);

	void TeleportToLastCheckpoint();

	
	
};
