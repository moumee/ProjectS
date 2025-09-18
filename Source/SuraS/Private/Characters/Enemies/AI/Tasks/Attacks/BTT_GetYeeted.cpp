// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Attacks/BTT_GetYeeted.h"

#include "AIController.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTT_GetYeeted::UBTT_GetYeeted(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Get Yeeted";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_GetYeeted::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CachedEnemy = Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter());

	if (CachedEnemy)
	{
		CachedEnemy->GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &UBTT_GetYeeted::OnHit);
	}
	
	return EBTNodeResult::InProgress;
}

void UBTT_GetYeeted::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (CachedEnemy)
	{
		if (CachedEnemy->GetCharacterMovement()->IsFalling())
		{
			// UE_LOG(LogTemp, Log, TEXT("Getting Yeeted"));

			if (!bIsFalling)
			{
				bIsFalling = true;
				UAnimInstance* const EnemyAnimInstance = CachedEnemy->GetMesh()->GetAnimInstance();
				UAnimMontage* FallingMontage = CachedEnemy->GetFallingMontage();

				EnemyAnimInstance->Montage_Play(FallingMontage);
			}
		}
		else
		{
			if (bIsFalling)
			{
				bIsFalling = false;

				UAnimInstance* const EnemyAnimInstance = CachedEnemy->GetMesh()->GetAnimInstance();
				EnemyAnimInstance->Montage_Stop(0.2f);
				CachedEnemy->GetCapsuleComponent()->OnComponentHit.RemoveDynamic(this, &UBTT_GetYeeted::OnHit);

				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
	}
}

void UBTT_GetYeeted::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (ASuraPawnPlayer* Player = Cast<ASuraPawnPlayer>(OtherActor))
	{
		// UE_LOG(LogTemp, Log, TEXT("OnHit %s"), *Player->GetName());

		FVector DirectionToOther = (OtherActor->GetActorLocation() - CachedEnemy->GetActorLocation()).GetSafeNormal2D();
		FVector ChargerRightVector = CachedEnemy->GetActorRightVector().GetSafeNormal2D();

		float SideSign = FMath::Sign(FVector::DotProduct(ChargerRightVector, DirectionToOther));

		FVector PerpendicularDirection = ChargerRightVector * SideSign;
				
		FDamageData DamageData;
		DamageData.DamageAmount = CachedEnemy->GetAttackDamageAmount();
		DamageData.DamageType = EDamageType::Charge;
		DamageData.ImpulseDirection = PerpendicularDirection;
		DamageData.ImpulseMagnitude = 1000.f;
			
		Player->TakeDamage(DamageData, CachedEnemy);
	}
}