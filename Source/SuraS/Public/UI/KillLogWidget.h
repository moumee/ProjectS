// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUIWidget.h"
#include "KillLogWidget.generated.h"

class UVerticalBox;
class UACKillLogManager;

/**
 * 
 */
UCLASS()
class SURAS_API UKillLogWidget : public UBaseUIWidget
{
	GENERATED_BODY()

public:
	void AddKillLogUI(const FString& Killer, const FString& Victim);
	void SetKillLogManager(UACKillLogManager* InManager);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* KillLogBox;
	
private:
	UPROPERTY()
	UACKillLogManager* KillLogManager;
	
	
};
