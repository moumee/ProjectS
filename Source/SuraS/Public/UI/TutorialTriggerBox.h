// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MissionTriggerBox.h"
#include "TutorialTriggerBox.generated.h"

/**
 * 
 */
class UUserWidget;

UCLASS()
class SURAS_API ATutorialTriggerBox : public AMissionTriggerBox
{
	GENERATED_BODY()

public:
	ATutorialTriggerBox();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tutorial")
	TSubclassOf<UUserWidget> TutorialWidgetClass;
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	UPROPERTY(Transient) 
	TObjectPtr<UUserWidget> ActiveTutorialWidget;
	
	UPROPERTY(Transient)
	TObjectPtr<APlayerController> TriggeringPlayerController;
	
	UFUNCTION()
	void CloseTutorialWidget();
	
};
