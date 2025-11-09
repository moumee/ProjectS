// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/SuraCharacterEnemyTurret.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ASuraCharacterEnemyTurret::ASuraCharacterEnemyTurret()
{
	EnemyType = "Turret";
}

// Called when the game starts or when spawned
void ASuraCharacterEnemyTurret::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
}

void ASuraCharacterEnemyTurret::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetAIController()->GetCurrentState() == EEnemyStates::Attacking)
	{
		ASuraPawnPlayer* Player = Cast<ASuraPawnPlayer>(GetAIController()->GetBlackboardComponent()->GetValueAsObject("AttackTarget"));

		if (IsValid(Player))
		{
			// 1. Get the target location in world space
			const FVector TargetLocation = Player->GetActorLocation();

			// 2. Get the BASE'S world transform (the parent of the mesh)
			//    This is the most important fix.
			const FTransform BaseWorldTransform = GetMesh()->GetAttachParent()->GetComponentTransform();

			// 3. Convert the target's location into the BASE'S local space
			const FVector LocalTargetLocation = BaseWorldTransform.InverseTransformPosition(TargetLocation);

			// 4. Get the HEAD'S location, also in the BASE'S local space
			const FVector LocalHeadLocation = GetMesh()->GetRelativeLocation();

			// 5. Find the look-at rotation. Both vectors are now in the same coordinate space.
			const FRotator LocalLookAtRotator = UKismetMathLibrary::FindLookAtRotation(
				LocalHeadLocation, 
				LocalTargetLocation
			);

			// 6. Apply your -90 degree offset for the mesh asset
			//    We use FMath::UnwindDegrees to keep the angle in the -180 to 180 range
			//    This prevents wrap-around issues during interpolation.
			FRotator TargetRelativeRotation = FRotator(LocalLookAtRotator.Pitch, LocalLookAtRotator.Yaw, 0.0f);
			
			// if (abs(GetActorRotation().Pitch) > 10 || abs(GetActorRotation().Roll > 10))
			// {
				TargetRelativeRotation.Yaw -= 90.0f;
			// }

			// 8. Unwind degrees for clean interpolation (solves -180/180 wrapping)
			TargetRelativeRotation.Yaw = FMath::UnwindDegrees(TargetRelativeRotation.Yaw);
			TargetRelativeRotation.Pitch = FMath::UnwindDegrees(TargetRelativeRotation.Pitch);

			// 9. (Recommended) Clamp the pitch angle
			TargetRelativeRotation.Pitch = FMath::Clamp(TargetRelativeRotation.Pitch, -1.f, 1.f);

			// 10. Get the mesh's current relative rotation
			const FRotator CurrentRelativeRotation = GetMesh()->GetRelativeRotation();

			// 11. Interpolate
			const FRotator NewRelativeRotation = FMath::RInterpTo(
				CurrentRelativeRotation,
				TargetRelativeRotation,
				DeltaSeconds,
				RotationSpeed
			);

			// 12. Set the mesh's (the head's) relative rotation
			GetMesh()->SetRelativeRotation(NewRelativeRotation);
		}
	}
}
