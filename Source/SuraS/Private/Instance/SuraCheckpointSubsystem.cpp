// Fill out your copyright notice in the Description page of Project Settings.


#include "Instance/SuraCheckpointSubsystem.h"

#include "ActorComponents/WeaponSystem/ACWeapon.h"
#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/SuraSaveGame.h"


void USuraCheckpointSubsystem::OnLoadCompleted(const FString& SlotName, const int32 UserIndex,
	USaveGame* LoadedGameData)
{
	if (USuraSaveGame* SaveData = Cast<USuraSaveGame>(LoadedGameData))
	{
		CurrentSave = SaveData;
		OnCheckpointLoadedDelegate.Broadcast();
	}
}

void USuraCheckpointSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	// if (UGameplayStatics::DoesSaveGameExist(CheckpointSlotName, 0))
	// {
	// 	CurrentSave = Cast<USuraSaveGame>(UGameplayStatics::LoadGameFromSlot(CheckpointSlotName, 0));
	// }
	// else
	// {
	// 	CurrentSave = Cast<USuraSaveGame>(UGameplayStatics::CreateSaveGameObject(USuraSaveGame::StaticClass()));
	// }
}

void USuraCheckpointSubsystem::SetCurrentSave(USuraSaveGame* SaveGame)
{
	CurrentSave = SaveGame;
}

void USuraCheckpointSubsystem::LoadCheckpoint()
{
	FAsyncLoadGameFromSlotDelegate LoadedDelegate;
	LoadedDelegate.BindUObject(this, &ThisClass::OnLoadCompleted);
	
	if (UGameplayStatics::DoesSaveGameExist(CheckpointSlotName, 0))
	{
		UGameplayStatics::AsyncLoadGameFromSlot(CheckpointSlotName, 0, LoadedDelegate);
	}
	else
	{
		CurrentSave = Cast<USuraSaveGame>(UGameplayStatics::CreateSaveGameObject(USuraSaveGame::StaticClass()));
	}

	
}

void USuraCheckpointSubsystem::SaveCheckpoint(FName MapName, const FTransform& SpawnTransform, int32 OrderIndex)
{
	// if (USuraSaveGame* SaveGameInstance = Cast<USuraSaveGame>(
	// 	UGameplayStatics::CreateSaveGameObject(USuraSaveGame::StaticClass())))
	// {
	// 	SaveGameInstance->MapName = MapName;
	// 	SaveGameInstance->SpawnTransform = SpawnTransform;
	// 	SaveGameInstance->CheckpointOrderIndex = OrderIndex;
	//
	// 	// [추가] 무기 소지 현황 저장
	// 	ASuraPawnPlayer* PlayerPawn = Cast<ASuraPawnPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	// 	if (PlayerPawn)
	// 	{
	// 		UWeaponSystemComponent* WeaponSystem = PlayerPawn->GetWeaponSystemComponent();
	// 		if (WeaponSystem)
	// 		{
	// 			// WeaponSystemComponent의 실제 무기 인벤토리(AWeapon* 배열)를 순회
	// 			for (AWeapon* Weapon : WeaponSystem->GetWeaponInventory())
	// 			{
	// 				if (Weapon)
	// 				{
	// 					// TMap<EWeaponName, bool>에 저장
	// 					SaveGameInstance->OwnedWeapons.Add(Weapon->GetWeaponName(), true);
	// 				}
	// 			}
	// 		}
	// 	}
	//
	// 	UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, CheckpointSlotName, 0);
	//
	// 	CurrentSave = SaveGameInstance;
	//
	// 	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Subsystem Save!"));
	// }
	
	// [수정] 새 객체를 만들지 않고, 기존 CurrentSave를 업데이트하거나 (없으면) 생성합니다.
	if (!CurrentSave)
	{
		// "새 게임" 후 첫 저장. 새 객체 생성.
		CurrentSave = Cast<USuraSaveGame>(UGameplayStatics::CreateSaveGameObject(USuraSaveGame::StaticClass()));
		if (!CurrentSave) return; // 생성 실패
	}
	
	// --- SaveGameInstance 대신 CurrentSave를 업데이트 ---
	CurrentSave->MapName = MapName;
	CurrentSave->SpawnTransform = SpawnTransform;
	CurrentSave->CheckpointOrderIndex = OrderIndex;

	// [수정] 무기 정보도 '현재 시점'으로 업데이트합니다.
	ASuraPawnPlayer* PlayerPawn = Cast<ASuraPawnPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (PlayerPawn)
	{
		UWeaponSystemComponent* WeaponSystem = PlayerPawn->GetWeaponSystemComponent();
		if (WeaponSystem)
		{
			CurrentSave->OwnedWeapons.Empty(); // [중요] 맵을 비우고 새로 채웁니다.
			for (AWeapon* Weapon : WeaponSystem->GetWeaponInventory())
			{
				if (Weapon)
				{
					CurrentSave->OwnedWeapons.Add(Weapon->GetWeaponName(), true);
				}
			}
		}
	}

	// 비동기로 '업데이트된' CurrentSave를 저장
	UGameplayStatics::AsyncSaveGameToSlot(CurrentSave, CheckpointSlotName, 0);

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Subsystem Save!"));
}

bool USuraCheckpointSubsystem::HasSavedCheckpoint() const
{
	return UGameplayStatics::DoesSaveGameExist(CheckpointSlotName, 0);
}

void USuraCheckpointSubsystem::ClearSavedCheckpoint()
{
	if (UGameplayStatics::DoesSaveGameExist(CheckpointSlotName, 0))
	{
		UGameplayStatics::DeleteGameInSlot(CheckpointSlotName, 0);
		
		CurrentSave = Cast<USuraSaveGame>(
			UGameplayStatics::CreateSaveGameObject(USuraSaveGame::StaticClass()));
	}
}

void USuraCheckpointSubsystem::SaveOnQuit()
{
	if (CurrentSave)
	{
		// [핵심 수정] GetWorld() / GetPlayerPawn() 대신 캐시된 포인터를 사용합니다.
		if (CachedPlayerPawn.IsValid() && CachedWorld.IsValid())
		{
			ASuraPawnPlayer* PlayerPawn = CachedPlayerPawn.Get();
			UWorld* World = CachedWorld.Get();

			// 2a. 현재 위치(비상계단)로 Transform을 덮어씁니다.
			CurrentSave->SpawnTransform = PlayerPawn->GetActorTransform();
			// 2b. 현재 맵 이름도 덮어씁니다.
			CurrentSave->MapName = FName(*World->GetName());

			// 2c. 현재 무기 목록으로 갱신합니다.
			CurrentSave->OwnedWeapons.Empty(); 
			UWeaponSystemComponent* WeaponSystem = PlayerPawn->GetWeaponSystemComponent();
			if (WeaponSystem)
			{
				for (AWeapon* Weapon : WeaponSystem->GetWeaponInventory())
				{
					if (Weapon)
					{
						CurrentSave->OwnedWeapons.Add(Weapon->GetWeaponName(), true);
					}
				}
			}
			
			UE_LOG(LogTemp, Warning, TEXT("[SuraCheckpointSubsystem] SaveOnQuit: '캐시된' 새 위치(%s)와 무기(%d개)로 CurrentSave 업데이트."),
				*CurrentSave->SpawnTransform.GetLocation().ToString(), CurrentSave->OwnedWeapons.Num());
		}
		else
		{
			// [수정] 이제 이 에러는 캐시가 비어있다는 뜻입니다.
			UE_LOG(LogTemp, Error, TEXT("[SuraCheckpointSubsystem] SaveOnQuit: 캐시된 PlayerPawn/World가 유효하지 않아 업데이트 실패!"));
		}

		// 3. 동기식으로 '업데이트된' CurrentSave 객체를 저장합니다.
		UE_LOG(LogTemp, Warning, TEXT("[SuraCheckpointSubsystem] SaveOnQuit() 호출. 동기식 저장 실행..."));
		UGameplayStatics::SaveGameToSlot(CurrentSave, CheckpointSlotName, 0);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[SuraCheckpointSubsystem] SaveOnQuit() 호출. CurrentSave가 없어 저장할 내용 없음."));
	}
}

void USuraCheckpointSubsystem::RegisterPlayerAndWorld(ASuraPawnPlayer* PlayerPawn, UWorld* World)
{
	CachedPlayerPawn = PlayerPawn;
	CachedWorld = World;
	UE_LOG(LogTemp, Log, TEXT("[SuraCheckpointSubsystem] PlayerPawn과 World가 캐시되었습니다."));
}




