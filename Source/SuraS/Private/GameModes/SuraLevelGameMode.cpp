// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/SuraLevelGameMode.h"

#include "ActorComponents/DamageComponent/ACDamageSystem.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Engine/PlayerStartPIE.h"
#include "Instance/SuraCheckpointSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/SuraSaveGame.h"

AActor* ASuraLevelGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	AActor* FoundPlayerStart = Super::ChoosePlayerStart_Implementation(Player);
	
	if (IsValid(FoundPlayerStart) && FoundPlayerStart->IsA<APlayerStartPIE>())
	{
		bPlayFromHere = true;
	}

	return FoundPlayerStart;
}

void ASuraLevelGameMode::BeginPlay()
{
	Super::BeginPlay();

    //UE_LOG(LogTemp, Error, TEXT("ASuraLevelGameMode::BeginPlay()"));

    if (bPlayFromHere) return;

    USuraCheckpointSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USuraCheckpointSubsystem>();
    if (!Subsystem) return;

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (!PlayerController) return; 

    FName CurrentMapName = FName(*UGameplayStatics::GetCurrentLevelName(this, true));

	USuraSaveGame* CurrentSave = Subsystem->GetCurrentSave();
	
	if (Subsystem->HasSavedCheckpoint())
	{
		TeleportToLastCheckpoint();
	}
	else
	{
		CurrentSave->MapName = CurrentMapName;
		CurrentSave->SpawnTransform = UGameplayStatics::GetPlayerPawn(this, 0)->GetActorTransform();
		CurrentSave->CheckpointOrderIndex = -1;

		Subsystem->SaveCheckpoint(CurrentSave->MapName, CurrentSave->SpawnTransform, CurrentSave->CheckpointOrderIndex);
	}
    
}

void ASuraLevelGameMode::RespawnToLastCheckpoint(ASuraPawnPlayer* Player)
{
	USuraCheckpointSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USuraCheckpointSubsystem>();
	ensure(Subsystem);

	float MaxHealth = Player->GetDamageSystemComponent()->GetMaxHealth();
	Player->GetDamageSystemComponent()->SetHealth(MaxHealth);
	Player->GetDamageSystemComponent()->SetIsDead(false);

	FTransform SpawnTransform = Subsystem->GetCurrentSave()->SpawnTransform;
	Player->TeleportTo(SpawnTransform.GetLocation(), SpawnTransform.Rotator());
}

void ASuraLevelGameMode::OnPlayerFellOutOfWorld(ASuraPawnPlayer* Player)
{
	USuraCheckpointSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USuraCheckpointSubsystem>();
	ensure(Subsystem);

	float CurrentHealth = Player->GetDamageSystemComponent()->GetHealth();
	float DamageAmount = CurrentHealth > PlayerMinimumRespawnHealth ? PlayerFellRespawnHealthReduction : 0.f;

	FDamageData DamageData;
	DamageData.DamageAmount = DamageAmount;
	
	Player->TakeDamage(DamageData, this);

	FTransform SpawnTransform = Subsystem->GetCurrentSave()->SpawnTransform;
	Player->TeleportTo(SpawnTransform.GetLocation(), SpawnTransform.Rotator());
}

void ASuraLevelGameMode::TeleportToLastCheckpoint()
{
	USuraCheckpointSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USuraCheckpointSubsystem>();
	ensure(Subsystem);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!IsValid(PlayerPawn)) return;
	FTransform SpawnTransform = Subsystem->GetCurrentSave()->SpawnTransform;
	PlayerPawn->TeleportTo(SpawnTransform.GetLocation(), SpawnTransform.Rotator());
}





