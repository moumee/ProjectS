// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TutorialTriggerBox.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h" 
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h" // SetGamePaused를 위해 필요
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h" 

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

		if (TutorialWidgetClass)
		{
			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (PlayerController)
			{
				TriggeringPlayerController = PlayerController;
				ActiveTutorialWidget = CreateWidget<UUserWidget>(PlayerController, TutorialWidgetClass);

				if (ActiveTutorialWidget)
				{
					ActiveTutorialWidget->AddToViewport();
					
					UGameplayStatics::SetGamePaused(GetWorld(), true);

					EnableInput(PlayerController);
					
					if (InputComponent)
					{
						FInputKeyBinding& Binding = InputComponent->BindKey(EKeys::AnyKey, IE_Pressed, this, &ATutorialTriggerBox::CloseTutorialWidget);
						Binding.bExecuteWhenPaused = true; 
					}

					PlayerController->SetInputMode(FInputModeGameAndUI());
					PlayerController->bShowMouseCursor = true;
				}
			}
		}
	}
}

void ATutorialTriggerBox::CloseTutorialWidget()
{
	if (ActiveTutorialWidget && TriggeringPlayerController)
	{
		ActiveTutorialWidget->RemoveFromParent();
		
		UGameplayStatics::SetGamePaused(GetWorld(), false);

		TriggeringPlayerController->SetInputMode(FInputModeGameOnly());
		TriggeringPlayerController->bShowMouseCursor = false;

		DisableInput(TriggeringPlayerController);

		ActiveTutorialWidget = nullptr;
		TriggeringPlayerController = nullptr;
	}
}