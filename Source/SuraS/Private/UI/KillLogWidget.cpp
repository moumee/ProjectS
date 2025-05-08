// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KillLogWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void UKillLogWidget::AddKillLogUI(const FString& Killer, const FString& Victim)
{
	if (!KillLogBox) return;

	FString LogText = FString::Printf(TEXT("%s killed %s"), *Killer, *Victim);

	UTextBlock* KillLogEntry = NewObject<UTextBlock>(this);
	if (KillLogEntry)
	{
		KillLogEntry->SetText(FText::FromString(LogText));
		KillLogEntry->SetColorAndOpacity(FSlateColor(FLinearColor::White));
		KillLogEntry->SetJustification(ETextJustify::Center);

		KillLogBox->AddChildToVerticalBox(KillLogEntry);

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, KillLogEntry]()
		{
			if (KillLogBox && KillLogEntry)
			{
				KillLogBox->RemoveChild(KillLogEntry);
			}
		}, 5.0f, false);
	}
}

void UKillLogWidget::SetKillLogManager(UACKillLogManager* InManager)
{
	KillLogManager = InManager;
}

void UKillLogWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
