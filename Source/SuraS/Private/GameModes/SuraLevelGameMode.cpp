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

	UE_LOG(LogTemp, Error, TEXT("ASuraLevelGameMode::BeginPlay()"));

	if (bPlayFromHere) return;

	USuraCheckpointSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USuraCheckpointSubsystem>();
	ensure(Subsystem);

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	ensure(PlayerController);

	FName CurrentMapName = FName(*UGameplayStatics::GetCurrentLevelName(this, true));
	FName SavedMapName = Subsystem->GetCurrentSave()->MapName;
	if (CurrentMapName != SavedMapName)
	{
		
		//Subsystem->SaveCheckpoint(CurrentMapName, ChoosePlayerStart(PlayerController)->GetActorTransform(), -1);
		//UE_LOG(LogTemp, Error, TEXT("Subsystem->SaveCheckpoint(CurrentMapName, ChoosePlayerStart(PlayerController)->GetActorTransform(), -1);"));
	// FName SavedMapName = Subsystem->GetCurrentSave()->MapName;
	// if (CurrentMapName != SavedMapName)
	// {
	// 	
	// 	Subsystem->SaveCheckpoint(CurrentMapName, ChoosePlayerStart(PlayerController)->GetActorTransform(), -1);
	// }
	// else
	// {
	// 	Subsystem->LoadCheckpoint();
	// }

	bool bShouldSaveNewCheckpoint = false;
	
	if (USuraSaveGame* CurrentSave = Subsystem->GetCurrentSave())
	{
		// "이어하기"인데 맵이 다르면 (예: 1레벨 -> 2레벨)
		if (CurrentSave->MapName != CurrentMapName)
		{
			bShouldSaveNewCheckpoint = true; 
		}
		// MapName이 같으면 "이어하기" 성공이므로, BeginPlay에서 아무것도 저장하지 않습니다.
	}
	else
	{
		Subsystem->LoadCheckpoint();

		//UE_LOG(LogTemp, Error, TEXT("Subsystem->LoadCheckpoint();"));
		// "새 게임" (CurrentSave가 nullptr)
		bShouldSaveNewCheckpoint = true; 
	}

	if (bShouldSaveNewCheckpoint)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameMode::BeginPlay: 새 게임/새 맵 감지. 기본 위치(비행기)를 체크포인트로 저장합니다."));
		
		// ChoosePlayerStart가 반환하는 기본 위치(비행기)를 찾아서 저장합니다.
		AActor* DefaultPlayerStart = ChoosePlayerStart(PlayerController);
		if (DefaultPlayerStart)
		{
			Subsystem->SaveCheckpoint(CurrentMapName, DefaultPlayerStart->GetActorTransform(), -1);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameMode::BeginPlay: 이어하기 감지. (저장 로직 건너뜀)"));
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





