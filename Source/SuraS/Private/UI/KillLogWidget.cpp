// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KillLogWidget.h"

#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void UKillLogWidget::SetKillLogManager(UACKillLogManager* InManager)
{
	KillLogManager = InManager;
}

void UKillLogWidget::SetSkullTexture(UTexture2D* InTexture)
{
	SkullTexture = InTexture;
}


void UKillLogWidget::AddSkull()
{
	if (!SkullBox || !SkullTexture) return;

	// [1] 해골 이미지 생성
	UImage* SkullImage = NewObject<UImage>(this);
	SkullImage->SetBrushFromTexture(SkullTexture);
	SkullImage->SetRenderOpacity(0.f); // 초기 투명도
	SkullImage->SetDesiredSizeOverride(FVector2D(32.f, 32.f));
	SkullImage->SetRenderTransform(FWidgetTransform(FVector2D(0.f, 0.f), FVector2D(2.f, 2.f), FVector2D(0.5f, 0.5f), 0.f));

	// [2] 안전한 포인터로 관리
	TWeakObjectPtr<UImage> WeakSkull = SkullImage;

	// [3] 왼쪽에 추가하려면 인덱스 0, 그리고 HorizontalBox 정렬은 반드시 Left
	SkullBox->InsertChildAt(0, SkullImage);

	// [4] 빠른 Fade-In + Scale-In (0.2초)
	const float AppearDuration = 0.2f;
	const float AppearInterval = 0.02f;
	const int32 AppearSteps = AppearDuration / AppearInterval;
	int32 AppearCounter = 0;

	FTimerHandle AppearTimer;
	GetWorld()->GetTimerManager().SetTimer(AppearTimer, [WeakSkull, AppearCounter = 0, AppearSteps]() mutable
	{
		if (!WeakSkull.IsValid()) return;

		float Progress = FMath::Clamp((float)AppearCounter / AppearSteps, 0.f, 1.f);
		float Scale = FMath::Lerp(2.f, 1.f, Progress);
		float Opacity = FMath::Lerp(0.f, 1.f, Progress);

		WeakSkull->SetRenderOpacity(Opacity);
		WeakSkull->SetRenderTransform(FWidgetTransform(FVector2D(0.f, 0.f), FVector2D(Scale, Scale), FVector2D(0.5f, 0.5f), 0.f));

		AppearCounter++;

	}, AppearInterval, true);

	// [5] 5초 뒤 → 빠른 Fade-out 후 제거
	const float FadeDelay = 5.0f;
	const float FadeDuration = 0.3f;
	const float FadeInterval = 0.02f;
	const int32 FadeSteps = FadeDuration / FadeInterval;

	FTimerHandle FadeStartTimer;
	GetWorld()->GetTimerManager().SetTimer(FadeStartTimer, [this, WeakSkull, FadeSteps, FadeInterval]()
	{
		if (!WeakSkull.IsValid()) return;

		int32 FadeCounter = 0;

		FTimerHandle FadeTimer;
		GetWorld()->GetTimerManager().SetTimer(FadeTimer, [this, WeakSkull, FadeCounter = 0, FadeSteps]() mutable
		{
			if (!WeakSkull.IsValid()) return;

			float Alpha = FMath::Clamp(1.f - (float)FadeCounter / FadeSteps, 0.f, 1.f);
			WeakSkull->SetRenderOpacity(Alpha);
			FadeCounter++;

			if (FadeCounter >= FadeSteps)
			{
				if (UWidget* Parent = WeakSkull->GetParent())
				{
					if (UPanelWidget* Panel = Cast<UPanelWidget>(Parent))
					{
						Panel->RemoveChild(WeakSkull.Get());
					}
				}
			}
		}, FadeInterval, true);

	}, FadeDelay, false);
}

void UKillLogWidget::AddScoreEntry(const FString& Reason, int32 Value)
{
	if (!ScoreBox) return;

	FString EntryText = FString::Printf(TEXT("%s +%d"), *Reason, Value);

	UTextBlock* ScoreText = NewObject<UTextBlock>(this);
	ScoreText->SetText(FText::FromString(EntryText));
	ScoreText->SetColorAndOpacity(FSlateColor(FLinearColor::Yellow));
	//ScoreText->SetFont(FSlateFontInfo(FCoreStyle::GetDefaultFont(), 18));

	// ScoreBox에 추가 (맨 아래로)
	ScoreBox->AddChildToVerticalBox(ScoreText);

	// 최대 3개 유지
	if (ScoreBox->GetChildrenCount() > 3)
	{
		ScoreBox->RemoveChildAt(0); // 가장 오래된 항목 제거
	}

	// ✅ 일정 시간 뒤에 제거
	FTimerHandle FadeTimer;
	GetWorld()->GetTimerManager().SetTimer(FadeTimer, [this, ScoreText]()
	{
		ScoreText->SetRenderOpacity(0.f);
		ScoreBox->RemoveChild(ScoreText);
	}, 1.5f, false);
}

void UKillLogWidget::UpdateTotalScore(int32 AddedScore)
{
	TotalScore += AddedScore;

	if (TotalScoreText)
	{
		FString ScoreText = FString::Printf(TEXT("Total Score: %d"), TotalScore);
		TotalScoreText->SetText(FText::FromString(ScoreText));
	}
}

void UKillLogWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UTexture2D* LoadedTexture = Cast<UTexture2D>(
	StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/UI/KillLog/img/skull.skull"))
);

	if (LoadedTexture)
	{
		SkullTexture = LoadedTexture;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("스컬 텍스처 로딩 실패"));
	}

}
