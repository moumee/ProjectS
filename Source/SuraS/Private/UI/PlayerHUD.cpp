// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"

#include "ActorComponents/WeaponSystem/ACWeapon.h"
#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Widgets/Player/PlayerHitWidget.h"

void UPlayerHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CurrentHitStageText->SetText(FText::AsNumber(CurrentHitStage));

	UpdateWeaponIcons();

	//InitializeHUD();
}

void UPlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();

	SuraPawnPlayer = Cast<ASuraPawnPlayer>(GetOwningPlayerPawn());
	if (SuraPawnPlayer)
	{
		WeaponSystemComponent = SuraPawnPlayer->GetWeaponSystemComponent();
		if (WeaponSystemComponent)
		{
			WeaponSystemComponent->OnWeaponPickedUp.AddDynamic(this, &UPlayerHUD::UpdatePickup);
		}
	}

	// Overlay / 이미지 배열 순서대로 추가
	WeaponSlots = { WeaponSlot_0, WeaponSlot_1, WeaponSlot_2};
	WeaponImages = { Image_0, Image_1, Image_2 };
}

void UPlayerHUD::SetPlayerHUDManager(UACPlayerHUDManager* InManager)
{
	PlayerHUDManager = InManager;
}


void UPlayerHUD::UpdateHpBar()
{
	if (!PlayerHitWidget && SuraPawnPlayer)
	{
		PlayerHitWidget = SuraPawnPlayer->GetPlayerHitWidget();
		if (PlayerHitWidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("UPlayerHUD: PlayerHitWidget assigned on demand in UpdateHpBar."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UPlayerHUD: PlayerHitWidget is still null after trying to get it in UpdateHpBar."));
		}
	}
	
	// 1. 회복 타이머 초기화 (기존 대기 중이면 제거)
	GetWorld()->GetTimerManager().ClearTimer(RecoveryTimerHandle);

	// 2. 최대 5단계까지만 증가
	if (CurrentHitStage < 5)
	{
		CurrentHitStage++;
	}

	// 3. 체력 이미지 변경
	ApplyHpBarImage(CurrentHitStage);

	// 4. Hit UI 위젯이 있으면 해당 단계로 업데이트
	if (PlayerHitWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateHitScreen Called"));
		PlayerHitWidget->UpdateHitScreen(CurrentHitStage);
	}
	else if (!PlayerHitWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerHitWidget is null"));
	}

	// 5. 3초 후 회복 처리 타이머 시작
	GetWorld()->GetTimerManager().SetTimer(
		RecoveryTimerHandle,
		this,
		&UPlayerHUD::RecoverHpBarStage,
		3.0f,
		false
	);
}

void UPlayerHUD::RecoverHpBarStage()
{
	// 최소 단계 0
	if (CurrentHitStage > 0)
	{
		CurrentHitStage--;
	}

	// 체력 이미지 갱신
	ApplyHpBarImage(CurrentHitStage);

	// Hit UI 갱신
	if (PlayerHitWidget)
	{
		PlayerHitWidget->UpdateHitScreen(CurrentHitStage);
	}

	// 추가 회복을 위한 타이머 재등록 (남은 단계가 있다면)
	if (CurrentHitStage > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(
			RecoveryTimerHandle,
			this,
			&UPlayerHUD::RecoverHpBarStage,
			3.0f,
			false
		);
	}
}

void UPlayerHUD::ApplyHpBarImage(int32 Stage)
{
	if (HpBar && HPBarTextures.IsValidIndex(Stage))
	{
		HpBar->SetBrushFromTexture(HPBarTextures[Stage]);
	}
}

void UPlayerHUD::UpdateWeaponIcons()
{
	if (!WeaponSystemComponent) return;

	const TArray<AWeapon*>& Inventory = WeaponSystemComponent->GetWeaponInventory();

	// 최대 3개만 처리
	for (int32 i = 0; i < FMath::Min(3, Inventory.Num()); ++i)
	{
		AWeapon* Weapon = Inventory[i];
		if (!Weapon) continue;

		UTexture2D* Icon = Weapon->GetWeaponImage();
		if (!Icon) continue;

		switch (i)
		{
			case 0: Image_0->SetBrushFromTexture(Icon); break;
			case 1: Image_1->SetBrushFromTexture(Icon); break;
			case 2: Image_2->SetBrushFromTexture(Icon); break;
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("WeaponIcon updated!!"));
}

void UPlayerHUD::OnWeaponSwitchAnim(int32 NewIndex)
{
	// switch (NewIndex)
	// {
	// case 0:
	// 	PlayAnimation(Slot0_TopToCenter);
	// 	break;
	// case 1:
	// 	PlayAnimation(Slot1_TopToCenter);
	// 	break;
	// case 2:
	// 	PlayAnimation(Slot2_CenterToBottom, 0.f, 1, EUMGSequencePlayMode::Reverse);
	// 	break;
	// }
}

void UPlayerHUD::InitializeHUD()
{
	if (!WeaponSystemComponent) return;
	
	const TArray<AWeapon*>& Inventory = WeaponSystemComponent->GetWeaponInventory();
	int32 WeaponCount = WeaponSystemComponent->GetWeaponNum();

	WeaponInventoryNum->SetText(FText::AsNumber(WeaponCount));

	
	// 모든 슬롯을 먼저 숨김 처리
	if (WeaponSlot_0) WeaponSlot_0->SetVisibility(ESlateVisibility::Collapsed);
	if (WeaponSlot_1) WeaponSlot_1->SetVisibility(ESlateVisibility::Collapsed);
	if (WeaponSlot_2) WeaponSlot_2->SetVisibility(ESlateVisibility::Collapsed);

	// 인벤토리 수에 따라 슬롯을 보여줌
	if (WeaponCount >= 1 && WeaponSlot_0)
	{
		WeaponSlot_0->SetVisibility(ESlateVisibility::Visible);
	}
	if (WeaponCount >= 2 && WeaponSlot_1)
	{
		WeaponSlot_1->SetVisibility(ESlateVisibility::Visible);
	}
	if (WeaponCount >= 3 && WeaponSlot_2)
	{
		WeaponSlot_2->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPlayerHUD::UpdatePickup(FName WeaponName)
{
	InitializeHUD();
}


