// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MissionTriggerBox.h"
#include "TutorialTriggerBox.generated.h"

/**
 * 
 */

class UTutorialWidget;

UCLASS()
class SURAS_API ATutorialTriggerBox : public AMissionTriggerBox
{
	GENERATED_BODY()

public:
	ATutorialTriggerBox();

	void CloseTutorialWidget();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tutorial")
	TSubclassOf<UUserWidget> TutorialWidgetClass;
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void ShowTutorialWidget();

private:
	UPROPERTY(Transient) 
	TObjectPtr<UTutorialWidget> ActiveTutorialWidget;
	
	UPROPERTY(Transient)
	TObjectPtr<APlayerController> TriggeringPlayerController;
	
	FTimerHandle ShowWidgetTimerHandle;
	
	UPROPERTY(Transient)
	TObjectPtr<APlayerController> PendingPlayerController;
	
	
};
