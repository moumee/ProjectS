// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PauseMenuWidget.h"

#include "ActorComponents/UISystem/ACUIMangerComponent.h"

FReply UPauseMenuWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (!GEngine) return FReply::Unhandled();

	if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("NativeOnKeyDown: ESC Key Detected!"));
		
		APawn* MyPawn = GetOwningPlayerPawn();
       
		if (MyPawn)
		{
			UACUIMangerComponent* UIManager = MyPawn->GetComponentByClass<UACUIMangerComponent>();
          
			if (UIManager)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("NativeOnKeyDown: UIManager Found on PAWN! Calling TogglePauseMenu..."));
             
				UIManager->TogglePauseMenu();
				return FReply::Handled();
			}
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("NativeOnKeyDown: FAILED to find UACUIMangerComponent on PAWN!"));
			}
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("NativeOnKeyDown: FAILED to get Owning Player Pawn!"));
		}

		return FReply::Handled(); 
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
	
}
