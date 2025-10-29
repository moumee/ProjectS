// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AN/AN_Footstep.h"

#include "Characters/PawnBAsePlayer/SuraPawnPlayer.h"
#include "Characters/PawnBasePlayer/SuraPlayerMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void UAN_Footstep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                          const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* PlayerActor = MeshComp->GetOwner())
	{
		if (ASuraPawnPlayer* PawnPlayer = Cast<ASuraPawnPlayer>(PlayerActor))
		{
			if (PawnPlayer->GetPlayerMovementComponent()->GetMovementState() == EMovementState::EMS_Move)
			{
				UGameplayStatics::PlaySoundAtLocation(MeshComp, FootstepSound, PawnPlayer->GetActorLocation(), FRotator::ZeroRotator,
					VolumeMultiplier, PitchMultiplier);
			}
		}
	}
}
