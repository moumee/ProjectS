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

	// Skull Image 생성
	UImage* SkullImage = NewObject<UImage>(this);
	SkullImage->SetBrushFromTexture(SkullTexture);
	SkullImage->SetDesiredSizeOverride(FVector2D(32.f, 32.f));
	SkullImage->SetOpacity(0.9f);

	// 최대 6개 유지
	if (SkullBox->GetChildrenCount() >= 6)
	{
		SkullBox->RemoveChildAt(0);
	}

	SkullBox->AddChild(SkullImage);
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
