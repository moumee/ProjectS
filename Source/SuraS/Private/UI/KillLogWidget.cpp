// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KillLogWidget.h"

#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
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

	// [1] 기존 해골 이미지들을 오른쪽으로 밀기
	TArray<UWidget*> OldChildren;
	const int32 NumChildren = SkullBox->GetChildrenCount();

	for (int32 i = 0; i < NumChildren; ++i)
	{
		if (UWidget* Child = SkullBox->GetChildAt(i))
		{
			OldChildren.Add(Child);
		}
	}
	SkullBox->ClearChildren(); // 전부 제거 후 다시 삽입할 예정

	// [2] SkullImage 생성
	UImage* SkullImage = NewObject<UImage>(this);
	SkullImage->SetBrushFromTexture(SkullTexture);
	SkullImage->SetRenderOpacity(0.f); // 초기 투명도
	SkullImage->SetRenderTransform(FWidgetTransform(FVector2D(0.f, 0.f), FVector2D(2.f, 2.f), FVector2D(0.5f, 0.5f), 0.f)); // 초기 2배 크기

	// [3] SizeBox로 감싸서 64x64 크기 고정
	USizeBox* SizeWrapper = NewObject<USizeBox>(this);
	SizeWrapper->SetWidthOverride(64.f);
	SizeWrapper->SetHeightOverride(64.f);
	SizeWrapper->AddChild(SkullImage);

	// [4] 안전한 포인터로 관리
	TWeakObjectPtr<UImage> WeakSkull = SkullImage;
	TWeakObjectPtr<USizeBox> WeakWrapper = SizeWrapper;

	// [5] 새 SkullImage를 왼쪽(맨 앞)에 삽입
	SkullBox->AddChild(SizeWrapper);

	// [6] 기존 해골들을 다시 오른쪽으로 추가
	for (UWidget* Child : OldChildren)
	{
		SkullBox->AddChild(Child);
	}

	// [7] 빠른 Fade-In + Scale-In (0.2초)
	const float AppearDuration = 0.2f;
	const float AppearInterval = 0.02f;
	const int32 AppearSteps = AppearDuration / AppearInterval;

	FTimerHandle AppearTimer;
	GetWorld()->GetTimerManager().SetTimer(AppearTimer, [WeakSkull, Counter = 0, AppearSteps]() mutable
	{
		if (!WeakSkull.IsValid()) return;

		float Progress = FMath::Clamp(static_cast<float>(Counter) / AppearSteps, 0.f, 1.f);
		float Scale = FMath::Lerp(2.f, 1.f, Progress);
		float Opacity = FMath::Lerp(0.f, 1.f, Progress);

		WeakSkull->SetRenderOpacity(Opacity);
		WeakSkull->SetRenderTransform(FWidgetTransform(FVector2D(0.f, 0.f), FVector2D(Scale, Scale), FVector2D(0.5f, 0.5f), 0.f));

		Counter++;
	}, AppearInterval, true);

	// skull 삭제 fade out
	// [8] 5초 뒤 Fade-Out 및 삭제
	const float DelayBeforeFadeOut = 5.f;
	const float FadeOutDuration = 0.5f;
	const float FadeOutInterval = 0.02f;
	const int32 FadeOutSteps = FadeOutDuration / FadeOutInterval;

	FTimerHandle FadeOutTimer;
	GetWorld()->GetTimerManager().SetTimer(FadeOutTimer, [WeakWrapper, Counter = 0, FadeOutSteps, this, &FadeOutTimer]() mutable // FadeOutTimer를 참조로 캡처
	{
		if (!WeakWrapper.IsValid())
		{
			return;
		}

		if (USizeBox* ValidWrapper = WeakWrapper.Get())
		{
			if (ValidWrapper->GetChildrenCount() > 0)
			{
				if (UImage* SkullToRemove = Cast<UImage>(ValidWrapper->GetChildAt(0)))
				{
					float Progress = FMath::Clamp(static_cast<float>(Counter) / FadeOutSteps, 0.f, 1.f);
					float Opacity = FMath::Lerp(1.f, 0.f, Progress);
					SkullToRemove->SetRenderOpacity(Opacity);

					Counter++;

					if (Counter > FadeOutSteps)
					{
						SkullBox->RemoveChild(ValidWrapper);
						// ValidWrapper->MarkPendingKill(); // GC를 위해 마킹 (선택 사항)
						GetWorld()->GetTimerManager().ClearTimer(FadeOutTimer);
					}
				}
				else
				{
					// Child가 UImage가 아닌 경우 타이머 종료
					GetWorld()->GetTimerManager().ClearTimer(FadeOutTimer);
				}
			}
			else
			{
				// 더 이상 자식이 없으면 타이머 종료
				GetWorld()->GetTimerManager().ClearTimer(FadeOutTimer);
			}
		}
		else
		{
			// WeakWrapper가 유효하지 않으면 타이머 종료
			GetWorld()->GetTimerManager().ClearTimer(FadeOutTimer);
		}
	}, FadeOutInterval, true, DelayBeforeFadeOut);
}

void UKillLogWidget::AddScoreEntry(const FString& Reason, int32 Value)
{
	if (!ScoreBox) return;

	// [1] 텍스트 생성
	FString EntryText = FString::Printf(TEXT("%s +%d"), *Reason, Value);
	UTextBlock* ScoreText = NewObject<UTextBlock>(this);
	ScoreText->SetText(FText::FromString(EntryText));
	ScoreText->SetColorAndOpacity(FSlateColor(FLinearColor::Yellow));
	ScoreText->SetRenderOpacity(1.f); // 최근 항목은 항상 1.0

	// [2] 생성 시 Y 오프셋 적용 (위에서 아래로 떨어지는 느낌)
	ScoreText->SetRenderTransform(FWidgetTransform(FVector2D(0, -20), FVector2D(1, 1), FVector2D(0.5f, 0.5f), 0.f));

	// [3] ScoreBox에 추가 (맨 아래)
	ScoreBox->AddChildToVerticalBox(ScoreText);

	// [4] 기존 항목들 투명도 조정 (최대 3개)
	const int32 Count = ScoreBox->GetChildrenCount();
	for (int32 i = 0; i < Count; ++i)
	{
		if (UTextBlock* ChildText = Cast<UTextBlock>(ScoreBox->GetChildAt(i)))
		{
			float Alpha = FMath::Clamp(1.0f - 0.4f * i, 0.3f, 1.0f); // 예: 1.0, 0.6, 0.3
			ChildText->SetRenderOpacity(Alpha);
		}
	}

	// [5] 내려오는 애니메이션 (0.2초 동안 Y 오프셋 -20 → 0)
	const float SlideDuration = 0.2f;
	const float SlideInterval = 0.02f;
	const int32 SlideSteps = SlideDuration / SlideInterval;

	TWeakObjectPtr<UTextBlock> WeakScoreText = ScoreText;

	FTimerHandle SlideTimer;
	GetWorld()->GetTimerManager().SetTimer(SlideTimer, [WeakScoreText, StepCounter = 0, SlideSteps]() mutable
	{
		if (!WeakScoreText.IsValid()) return;

		float Progress = FMath::Clamp((float)StepCounter / SlideSteps, 0.f, 1.f);
		float YOffset = FMath::Lerp(-20.f, 0.f, Progress);
		WeakScoreText->SetRenderTransform(FWidgetTransform(FVector2D(0.f, YOffset), FVector2D(1.f, 1.f), FVector2D(0.5f, 0.5f), 0.f));

		StepCounter++;
	}, SlideInterval, true);

	// [6] 최대 3개 유지
	if (ScoreBox->GetChildrenCount() > 3)
	{
		ScoreBox->RemoveChildAt(0); // 가장 오래된 항목 제거
	}

	// [7] 1.5초 뒤 → 자연스럽게 제거
	FTimerHandle FadeTimer;
	GetWorld()->GetTimerManager().SetTimer(FadeTimer, [this, WeakScoreText]()
	{
		if (!WeakScoreText.IsValid()) return;

		WeakScoreText->SetRenderOpacity(0.f);
		if (ScoreBox->HasChild(WeakScoreText.Get()))
		{
			ScoreBox->RemoveChild(WeakScoreText.Get());
		}
	}, 5.0f, false);
	
}


void UKillLogWidget::UpdateTotalScore(int32 AddedScore)
{
	TotalScore += AddedScore;

	if (TotalScoreText)
	{
		FString ScoreText = FString::Printf(TEXT("%d"), TotalScore);

		TotalScoreText->SetText(FText::FromString(ScoreText));
	}
}

void UKillLogWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UTexture2D* LoadedTexture = Cast<UTexture2D>(
	StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/UI/KillLog/img/skull2"))
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


