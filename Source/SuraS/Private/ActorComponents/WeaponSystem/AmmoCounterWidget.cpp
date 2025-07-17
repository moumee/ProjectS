// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/AmmoCounterWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UAmmoCounterWidget::UpdateAmmoCount(int32 NewAmmoCount)
{
	AmmoCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), NewAmmoCount)));
}

void UAmmoCounterWidget::UpdateTotalAmmo(int32 NewTotalAmmo)
{
	TotalAmmo->SetText(FText::FromString(FString::Printf(TEXT("%d"), NewTotalAmmo)));
}

void UAmmoCounterWidget::UpdateHpBar()
{
	// 타이머 초기화(기존 회복 대기 타이머 제거)
	GetWorld()->GetTimerManager().ClearTimer(RecoveryyTimerHandle);

	// 최대 5단계까지 증가
	if (CurrentHitStage < 5)
	{
		CurrentHitStage++;
	}

	// 이미지 변경
	ApplyHpBarImage(CurrentHitStage);

	// 3초 후 회복 타이머 시작
	GetWorld()->GetTimerManager().SetTimer(
		RecoveryyTimerHandle,
		this,
		&UAmmoCounterWidget::RecoverHpBarStage,
		3.0,
		false // 반복 x
		);
}

void UAmmoCounterWidget::RecoverHpBarStage()
{
	if (CurrentHitStage > 0)
	{
		CurrentHitStage--;
		ApplyHpBarImage(CurrentHitStage);

		// 다시 회복 대기 타이머 시작 (연속 회복 가능)
		GetWorld()->GetTimerManager().SetTimer(
			RecoveryyTimerHandle,
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
