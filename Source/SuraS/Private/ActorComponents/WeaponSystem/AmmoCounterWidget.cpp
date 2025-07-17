// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/AmmoCounterWidget.h"

#include "ActorComponents/WeaponSystem/ACWeapon.h"
#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h"
#include "AnimNodes/AnimNode_RandomPlayer.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Widgets/Player/PlayerHitWidget.h"

void UAmmoCounterWidget::UpdateAmmoCount(int32 NewAmmoCount)
{
	AmmoCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), NewAmmoCount)));
}

void UAmmoCounterWidget::UpdateTotalAmmo(int32 NewTotalAmmo)
{
	TotalAmmo->SetText(FText::FromString(FString::Printf(TEXT("%d"), NewTotalAmmo)));
}

void UAmmoCounterWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CurrentHitStageText->SetText(FText::AsNumber(CurrentHitStage));
}

void UAmmoCounterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SuraPawnPlayer = Cast<ASuraPawnPlayer>(GetOwningPlayerPawn());
	if (!SuraPawnPlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("UAmmoCounterWidget: SuraPawnPlayer is null! Check GetOwningPlayerPawn()."));
	}
}


void UAmmoCounterWidget::UpdateHpBar()
{
	if (!PlayerHitWidget && SuraPawnPlayer)
	{
		PlayerHitWidget = SuraPawnPlayer->GetPlayerHitWidget();
		if (PlayerHitWidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("UAmmoCounterWidget: PlayerHitWidget assigned on demand in UpdateHpBar."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UAmmoCounterWidget: PlayerHitWidget is still null after trying to get it in UpdateHpBar."));
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
		&UAmmoCounterWidget::RecoverHpBarStage,
		3.0f,
		false
	);
}

void UAmmoCounterWidget::RecoverHpBarStage()
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
			&UAmmoCounterWidget::RecoverHpBarStage,
			3.0f,
			false
		);
	}
}

void UAmmoCounterWidget::ApplyHpBarImage(int32 Stage)
{
	if (HpBar && HPBarTextures.IsValidIndex(Stage))
	{
		HpBar->SetBrushFromTexture(HPBarTextures[Stage]);
	}
}
