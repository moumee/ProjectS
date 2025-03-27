// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PawnBasePlayer/SuraPlayerController.h"

#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "GameFramework/SpectatorPawn.h"
#include "Kismet/GameplayStatics.h"


void ASuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("ToggleDebugPawn", IE_Pressed, this, &ASuraPlayerController::ToggleDebugPossession);
}

void ASuraPlayerController::ToggleDebugPossession()
{
	if (GetPawn()->IsA(ASuraPawnPlayer::StaticClass()))
	{

		if (!SpawnedDebugPawn)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			SpawnedDebugPawn =
				GetWorld()->SpawnActor<ASpectatorPawn>(ASpectatorPawn::StaticClass(), GetPawn()->GetActorLocation(), GetControlRotation(), SpawnParams);
		}
		else
		{
			SpawnedDebugPawn->SetActorLocationAndRotation(GetPawn()->GetActorLocation(), GetControlRotation());
		}

		UnPossess();
		Possess(SpawnedDebugPawn);
	}
	else
	{
		if (ASuraPawnPlayer* PawnPlayer = Cast<ASuraPawnPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), ASuraPawnPlayer::StaticClass())))
		{
			UnPossess();
			Possess(PawnPlayer);
		}
	}
}
