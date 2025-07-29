// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/TargetingSkillWidget.h"
#include "Components/TextBlock.h"


void UTargetingSkillWidget::InitializeUI(float NewCurrCoolDown, float NewMaxSkillCoolDown, float NewCurrTargetingTime, float NewMaxTargetingTime)
{
	if (CurrentSkillCoolDown)
	{
		FNumberFormattingOptions FormatOptions;
		FormatOptions.SetMaximumFractionalDigits(1);
		CurrentSkillCoolDown->SetText(FText::AsNumber(NewCurrCoolDown, &FormatOptions));
	}
	if (MaxSkillCoolDown)
	{
		FNumberFormattingOptions FormatOptions;
		FormatOptions.SetMaximumFractionalDigits(1);
		MaxSkillCoolDown->SetText(FText::AsNumber(NewMaxSkillCoolDown, &FormatOptions));
	}
	if (MaxTargetingTime)
	{
		FNumberFormattingOptions FormatOptions;
		FormatOptions.SetMaximumFractionalDigits(1);
		MaxTargetingTime->SetText(FText::AsNumber(NewMaxTargetingTime, &FormatOptions));
	}
	if (ElapsedTimeAfterTargetingStarted)
	{
		FNumberFormattingOptions FormatOptions;
		FormatOptions.SetMaximumFractionalDigits(1);
		ElapsedTimeAfterTargetingStarted->SetText(FText::AsNumber(NewCurrTargetingTime, &FormatOptions));
	}
}
void UTargetingSkillWidget::SetTargetingSkillCoolDown(float NewCurrCoolDown)
{
	//CurrentSkillCoolDown->SetText(FText::FromString(FString::Printf(TEXT("%f"), NewCurrCoolDown)));
	if (CurrentSkillCoolDown)
	{
		FNumberFormattingOptions FormatOptions;
		FormatOptions.SetMaximumFractionalDigits(1);
		CurrentSkillCoolDown->SetText(FText::AsNumber(NewCurrCoolDown, &FormatOptions));
	}
}
void UTargetingSkillWidget::SetDetectionTimeUIVisible(bool bVisible)
{
	if (bVisible)
	{
		ElapsedTimeAfterTargetingStarted->SetVisibility(ESlateVisibility::Visible);
		MaxTargetingTime->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ElapsedTimeAfterTargetingStarted->SetVisibility(ESlateVisibility::Hidden);
		MaxTargetingTime->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UTargetingSkillWidget::SetTargetingTimeUI(float NewCurrTargetingTime)
{
	if (ElapsedTimeAfterTargetingStarted)
	{
		FNumberFormattingOptions FormatOptions;
		FormatOptions.SetMaximumFractionalDigits(1);
		ElapsedTimeAfterTargetingStarted->SetText(FText::AsNumber(NewCurrTargetingTime, &FormatOptions));
	}
}
