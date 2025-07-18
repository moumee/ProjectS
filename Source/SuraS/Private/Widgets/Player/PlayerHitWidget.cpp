// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Player/PlayerHitWidget.h"

void UPlayerHitWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HitEffect->SetVisibility(ESlateVisibility::Hidden);
	HitEffectBackground->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerHitWidget::PlayFadeAnimation()
{
	PlayAnimation(FadeInOutAnimation);
}

void UPlayerHitWidget::UpdateHitScreen(int32 Stage)
{
	
	// 0단계는 아무 것도 표시하지 않음
	if (Stage <= 0)
	{
		// PlayAnimation(FadeOutHitEffect);
		HitEffect->SetVisibility(ESlateVisibility::Hidden);
		HitEffectBackground->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	// ---------------- HitEffect 이미지 처리 ----------------
	if (HitEffectTextures.IsValidIndex(Stage - 1))
	{
		HitEffect->SetBrushFromTexture(HitEffectTextures[Stage - 1]);
		HitEffect->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		HitEffect->SetVisibility(ESlateVisibility::Hidden);
		HitEffectBackground->SetVisibility(ESlateVisibility::Hidden);
	}

	// ---------------- 배경 이미지 인덱스 계산 ----------------
	int32 BackgroundIndex = -1;
	if (Stage >= 1 && Stage <= 2)
	{
		BackgroundIndex = 0;
	}
	else if (Stage >= 3 && Stage <= 4)
	{
		BackgroundIndex = 1;
	}
	else if (Stage == 5)
	{
		BackgroundIndex = 2;
	}

	// ---------------- 배경 이미지 매칭 + 애니메이션 ----------------
	if (HitEffectBackgrounds.IsValidIndex(BackgroundIndex))
	{
		HitEffectBackground->SetBrushFromTexture(HitEffectBackgrounds[BackgroundIndex]);
		HitEffectBackground->SetVisibility(ESlateVisibility::Visible);
		PlayFadeAnimation();  // 애니메이션 실행 (UMG에서 만든)
	}
}
