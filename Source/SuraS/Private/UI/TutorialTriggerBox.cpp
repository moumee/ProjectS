// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TutorialTriggerBox.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h" 
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h" // SetGamePaused를 위해 필요
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h" 
#include "UI/TutorialWidget.h"

ATutorialTriggerBox::ATutorialTriggerBox()
{
	ActiveTutorialWidget = nullptr;
	TriggeringPlayerController = nullptr;
}

void ATutorialTriggerBox::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (bTriggerOnceOnly && bHasBeenTriggered)
	{
		return;
	}

	if (ActiveTutorialWidget)
	{
		return;
	}

	if (OtherActor && OtherActor->IsA(ASuraPawnPlayer::StaticClass()))
	{
		Super::NotifyActorBeginOverlap(OtherActor);

		PendingPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
       
		if (PendingPlayerController && TutorialWidgetClass)
		{
			GetWorld()->GetTimerManager().SetTimer(
				ShowWidgetTimerHandle, 
				this, 
				&ATutorialTriggerBox::ShowTutorialWidget, 
				0.01f, 
				false
			);
		}
	}
}

void ATutorialTriggerBox::ShowTutorialWidget()
{

	APlayerController* PlayerController = PendingPlayerController;
	
	if (PlayerController && TutorialWidgetClass && !ActiveTutorialWidget)
	{
		TriggeringPlayerController = PlayerController;
		ActiveTutorialWidget = CreateWidget<UTutorialWidget>(PlayerController, TutorialWidgetClass);

		if (ActiveTutorialWidget)
		{
			ActiveTutorialWidget->OwningTriggerBox = this;
			ActiveTutorialWidget->AddToViewport();
          
			UGameplayStatics::SetGamePaused(GetWorld(), true);
          
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(ActiveTutorialWidget->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
          
			PlayerController->SetInputMode(InputMode);
			PlayerController->bShowMouseCursor = true;
		}
	}

	PendingPlayerController = nullptr;
}

void ATutorialTriggerBox::CloseTutorialWidget()
{
	if (TriggeringPlayerController)
	{
		ActiveTutorialWidget = nullptr;
		TriggeringPlayerController = nullptr;
	}
}
