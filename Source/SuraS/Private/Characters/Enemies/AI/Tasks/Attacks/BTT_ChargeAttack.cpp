// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Attacks/BTT_ChargeAttack.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/SuraCharacterEnemyCharger.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTT_ChargeAttack::UBTT_ChargeAttack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Charge Attack";
	bNotifyTick = true;

	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTT_ChargeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASuraCharacterEnemyCharger* const Charger = Cast<ASuraCharacterEnemyCharger>(OwnerComp.GetAIOwner()->GetCharacter()))
	{
		CachedCharger = Charger;
		CachedOwnerComp = &OwnerComp;

		CachedCharger->GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &UBTT_ChargeAttack::OnHit);
		CachedCharger->OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &UBTT_ChargeAttack::OnOverlapBegin);
		OnAttackReadyMontageEnded.BindUObject(this, &UBTT_ChargeAttack::OnAttackReadyEnded);
		OnRoarMontageEnded.BindUObject(this, &UBTT_ChargeAttack::OnRoarEnded);
		OnStunMontageEnded.BindUObject(this, &UBTT_ChargeAttack::OnStunEnded);

		UAnimInstance* const EnemyAnimInstance = CachedCharger->GetMesh()->GetAnimInstance();
		UAnimMontage* AttackReadyAnimation = CachedCharger->GetChargeReadyAnimation();

		EnemyAnimInstance->Montage_Play(AttackReadyAnimation);

		// EnemyAnimInstance->Montage_SetBlendingOutDelegate(OnAttackReadyMontageEnded); // montage interrupted
		EnemyAnimInstance->Montage_SetEndDelegate(OnAttackReadyMontageEnded);

		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UBTT_ChargeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	if (bIsAttacking) 
	{
		ElapsedChargeTime += DeltaSeconds;
		
		if (ElapsedChargeTime > ChargeDuration)
		{
			EndTask();
		}
		else
		{
			CachedCharger->AddMovementInput(CachedCharger->GetActorForwardVector());
		}
	}
}

void UBTT_ChargeAttack::OnAttackReadyEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	bIsAttacking = true;
	OriginalMaxWalkSpeed = CachedCharger->GetCharacterMovement()->MaxWalkSpeed;
	CachedCharger->GetCharacterMovement()->MaxWalkSpeed = ChargeMaxWalkSpeed;

	CachedCharger->GetAIController()->ClearFocus(EAIFocusPriority::Gameplay); // to face only the front

	UAnimInstance* const EnemyAnimInstance = CachedCharger->GetMesh()->GetAnimInstance();
	UAnimMontage* AttackAnimation = CachedCharger->ChooseRandomAttackMontage();

	EnemyAnimInstance->Montage_Play(AttackAnimation, 1.f);
}

void UBTT_ChargeAttack::EndTask()
{
	// reset values
	ElapsedChargeTime = 0.f;
	bIsAttacking = false;
	CachedCharger->GetCharacterMovement()->MaxWalkSpeed = OriginalMaxWalkSpeed;
	bWasAttackSuccessful = false;

	if (ASuraPawnPlayer* const Player = Cast<ASuraPawnPlayer>(CachedOwnerComp.Get()->GetBlackboardComponent()->GetValueAsObject("AttackTarget")))
	{
		CachedCharger->GetAIController()->SetFocus(Player);
	}

	// end the task
	UAnimInstance* const EnemyAnimInstance = CachedCharger->GetMesh()->GetAnimInstance();
	EnemyAnimInstance->Montage_Stop(0.2f);

	UAnimMontage* RoarAnimation = CachedCharger->ChooseRandomRoarMontage();
	EnemyAnimInstance->Montage_Play(RoarAnimation);
	
	EnemyAnimInstance->Montage_SetEndDelegate(OnRoarMontageEnded);
}

void UBTT_ChargeAttack::OnRoarEnded(UAnimMontage* AnimMontage, bool bInterrupted) const
{
	FinishLatentTask(*CachedOwnerComp.Get(), EBTNodeResult::Succeeded);
}

void UBTT_ChargeAttack::OnStunEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	EndTask();
}

void UBTT_ChargeAttack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              FVector NormalImpulse, const FHitResult& Hit)
{
	if (bIsAttacking)
	{
		UPrimitiveComponent* ActorHitComp = OtherActor->FindComponentByClass<UPrimitiveComponent>();
		
		if (ASuraPawnPlayer* Player = Cast<ASuraPawnPlayer>(OtherActor))
		{
			// UE_LOG(LogTemp, Warning, TEXT("Charge Attacked Player"));

			if (!bWasAttackSuccessful)
			{
				FDamageData DamageData;
				DamageData.DamageAmount = CachedCharger->GetAttackDamageAmount();
				DamageData.DamageType = EDamageType::Charge;
			
				Player->TakeDamage(DamageData, CachedCharger);

				bWasAttackSuccessful = true;
			}
		}
		else if (ActorHitComp && ActorHitComp->GetCollisionProfileName() == FName("Wall")) // stun when collided with a wall
		{
			// UE_LOG(LogTemp, Error, TEXT("Charger Hit with %s"), *ActorHitComp->GetCollisionProfileName().ToString());
			
			bIsAttacking = false;
			
			UAnimInstance* const EnemyAnimInstance = CachedCharger->GetMesh()->GetAnimInstance();
			UAnimMontage* StunAnimation = CachedCharger->GetStunAnimation();
			EnemyAnimInstance->Montage_Play(StunAnimation);

			EnemyAnimInstance->Montage_SetEndDelegate(OnStunMontageEnded);
		}
		/*else
		{
			UE_LOG(LogTemp, Error, TEXT("Charger Hit with %s"), *ActorHitComp->GetCollisionProfileName().ToString());

			if (ASuraCharacterEnemyBase* Enemy = Cast<ASuraCharacterEnemyBase>(OtherActor))
			{
				UE_LOG(LogTemp, Error, TEXT("Enemy Charged"))
			}
		}*/
	}
}

void UBTT_ChargeAttack::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Error, TEXT("Charger Overlapped with %s"), *OtherActor->GetName())
}
