// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PawnBasePlayer/SuraPawnPlayerAnimInstance.h"

#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"

void USuraPawnPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	SuraPlayer = Cast<ASuraPawnPlayer>(TryGetPawnOwner());

	if (SuraPlayer)
	{
		MovementState = SuraPlayer->GetPlayerMovementComponent()->GetMovementState();
		bIsCrouching = SuraPlayer->GetPlayerMovementComponent()->IsCrouching();
		bIsDashing = SuraPlayer->GetPlayerMovementComponent()->IsDashing();
		Velocity = SuraPlayer->GetPlayerMovementComponent()->Velocity;
		MovementInputVector = SuraPlayer->GetPlayerMovementComponent()->GetMovementInputVector();
	}

	

	
}

void USuraPawnPlayerAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (SuraPlayer)
	{
		MovementState = SuraPlayer->GetPlayerMovementComponent()->GetMovementState();
		bIsCrouching = SuraPlayer->GetPlayerMovementComponent()->IsCrouching();
		bIsDashing = SuraPlayer->GetPlayerMovementComponent()->IsDashing();
		Velocity = SuraPlayer->GetPlayerMovementComponent()->Velocity;
		MovementInputVector = SuraPlayer->GetPlayerMovementComponent()->GetMovementInputVector();
	}
	
}
