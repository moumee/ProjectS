// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUIWidget.h"
#include "KillLogWidget.generated.h"

class UVerticalBox;
class UACKillLogManager;
class UHorizontalBox;
class UTextBlock;
class UTexture2D;

/**
 * 
 */
UCLASS()
class SURAS_API UKillLogWidget : public UBaseUIWidget
{
	GENERATED_BODY()

public:
	// void AddKillLogUI(const FString& Killer, const FString& Victim);
	void SetKillLogManager(UACKillLogManager* InManager);

	void AddSkull();
	void AddScoreEntry(const FString& Reason, int32 Value);
	void UpdateTotalScore(int32 AddedScore);
	void SetSkullTexture(UTexture2D* InTexture);

protected:
	virtual void NativeConstruct() override;
	

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* SkullBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TotalScoreText;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ScoreBox;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* KillLogBox;

	FTimerHandle AutoHideTimer;
	
	FTimerHandle TotalScoreFadeOutTimer;
	
	FTimerHandle FadeTotalScoreTimerHandle;


	
private:
	UPROPERTY()
	UACKillLogManager* KillLogManager;

	UPROPERTY()
	UTexture2D* SkullTexture;

	int32 TotalScore = 0;
	
	
};
