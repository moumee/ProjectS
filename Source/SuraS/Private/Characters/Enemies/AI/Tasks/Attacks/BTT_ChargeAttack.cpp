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
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTT_ChargeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASuraCharacterEnemyCharger* const Charger = Cast<ASuraCharacterEnemyCharger>(OwnerComp.GetAIOwner()->GetCharacter()))
	{
		CachedWeakCharger = Charger;
		CachedWeakOwnerComp = &OwnerComp;

		if (!CachedWeakCharger.Get() || !CachedWeakOwnerComp.Get()) return EBTNodeResult::Failed;

		CachedWeakCharger->GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &UBTT_ChargeAttack::OnHit);
		CachedWeakCharger->OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &UBTT_ChargeAttack::OnOverlapBegin);
		
		UAnimMontage* AttackReadyAnimation = CachedWeakCharger->GetChargeReadyAnimation();
		float AttackReadyAnimDuration = CachedWeakCharger->PlayAnimMontage(AttackReadyAnimation);
		
		TWeakObjectPtr<UBTT_ChargeAttack> WeakThis = this;
		FTimerHandle AnimCompleteHandle;

		GetWorld()->GetTimerManager().SetTimer(AnimCompleteHandle, [WeakThis]()
		{
			if (auto Task = WeakThis.Get())
			{
				if (auto Charger = Task->CachedWeakCharger.Get())
				{
					Task->OnAttackReadyEnded(Charger);
				}
				else if (auto OwnerComp = Task->CachedWeakOwnerComp.Get())
				{
					Task->FinishLatentTask(*OwnerComp, EBTNodeResult::Failed);
				}
			}
		}, AttackReadyAnimDuration, false);

		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UBTT_ChargeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	if (bIsAttacking) 
	{
		if (auto Charger = CachedWeakCharger.Get())
		{
			ElapsedChargeTime += DeltaSeconds;
		
			if (ElapsedChargeTime > ChargeDuration)
			{
				EndTask();
			}
			else
			{
				Charger->AddMovementInput(Charger->GetActorForwardVector());
			}
		}
		else
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}
	}
}

void UBTT_ChargeAttack::OnAttackReadyEnded(ASuraCharacterEnemyCharger* Charger)
{
	bIsAttacking = true;
	
	OriginalMaxWalkSpeed = Charger->GetCharacterMovement()->MaxWalkSpeed;
	Charger->GetCharacterMovement()->MaxWalkSpeed = ChargeMaxWalkSpeed;
	Charger->GetCharacterMovement()->MaxAcceleration = ChargeMaxWalkSpeed;
	Charger->GetCharacterMovement()->bRequestedMoveUseAcceleration = false;

	Charger->GetAIController()->ClearFocus(EAIFocusPriority::Gameplay); // to face only the front
	Charger->ActivateDashEffect();
	
	UAnimMontage* AttackAnimation = Charger->ChooseRandomAttackMontage();
	Charger->PlayAnimMontage(AttackAnimation);
}

void UBTT_ChargeAttack::EndTask()
{
	auto OwnerComp = CachedWeakOwnerComp.Get();
	auto Charger = CachedWeakCharger.Get();

	if (!OwnerComp || !Charger)
	{
		if (OwnerComp) FinishLatentTask(*OwnerComp, EBTNodeResult::Failed);

		return;
	}
	
	// reset values
	ElapsedChargeTime = 0.f;
	bIsAttacking = false;
	Charger->GetCharacterMovement()->MaxWalkSpeed = OriginalMaxWalkSpeed;
	Charger->GetCharacterMovement()->bRequestedMoveUseAcceleration = true;
	bWasAttackSuccessful = false;

	if (ASuraPawnPlayer* const Player = Cast<ASuraPawnPlayer>(OwnerComp->GetBlackboardComponent()->GetValueAsObject("AttackTarget")))
	{
		Charger->GetAIController()->SetFocus(Player);
	}

	// end the task
	/*UAnimInstance* const EnemyAnimInstance = CachedCharger->GetMesh()->GetAnimInstance();
	EnemyAnimInstance->Montage_Stop(0.2f);*/
	Charger->StopAnimMontage();

	UAnimMontage* RoarAnimation = Charger->ChooseRandomRoarMontage();
	float RoarAnimDuration = Charger->PlayAnimMontage(RoarAnimation);

	TWeakObjectPtr<UBTT_ChargeAttack> WeakThis = this;
	FTimerHandle AnimCompleteHandle;
	GetWorld()->GetTimerManager().SetTimer(
		AnimCompleteHandle,
		[WeakThis]()
		{
			if (auto Task = WeakThis.Get())
			{
				Task->OnRoarEnded();
			}
		},
		RoarAnimDuration,
		false
	);
}

// Always the last function to be called before the task ends
void UBTT_ChargeAttack::OnRoarEnded() const
{
	auto OwnerComp = CachedWeakOwnerComp.Get();
	if (!OwnerComp) return;
	
	if (auto Charger = CachedWeakCharger.Get())
	{
		Charger->GetCapsuleComponent()->OnComponentHit.RemoveDynamic(this, &UBTT_ChargeAttack::OnHit);
		Charger->OverlapBox->OnComponentBeginOverlap.RemoveDynamic(this, &UBTT_ChargeAttack::OnOverlapBegin);
	}
	
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}

void UBTT_ChargeAttack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              FVector NormalImpulse, const FHitResult& Hit)
{
	if (bIsAttacking)
	{
		UPrimitiveComponent* ActorHitComp = OtherActor->FindComponentByClass<UPrimitiveComponent>();

		auto Charger = CachedWeakCharger.Get();
		if (!Charger) return;

		ASuraPawnPlayer* Player = Cast<ASuraPawnPlayer>(OtherActor);
		
		if (IsValid(Player))
		{
			// UE_LOG(LogTemp, Warning, TEXT("Charge Attacked Player"));
			
			if (!bWasAttackSuccessful)
			{
				
				FVector DirectionToOther = (OtherActor->GetActorLocation() - Charger->GetActorLocation()).GetSafeNormal2D();
				FVector ChargerRightVector = Charger->GetActorRightVector().GetSafeNormal2D();

				float SideSign = FMath::Sign(FVector::DotProduct(ChargerRightVector, DirectionToOther));

				FVector PerpendicularDirection = ChargerRightVector * SideSign;
				
				FDamageData DamageData;
				DamageData.DamageAmount = Charger->GetAttackDamageAmount();
				DamageData.DamageType = EDamageType::Charge;
				DamageData.ImpulseDirection = PerpendicularDirection;
				DamageData.ImpulseMagnitude = 1000.f;
			
				Player->TakeDamage(DamageData, Charger);

				Charger->ActivateCollisionEffect();
				bWasAttackSuccessful = true;
			}
		}
		else if (ActorHitComp && ActorHitComp->GetCollisionProfileName() == FName("Wall")) // stun when collided with a wall
		{
			// UE_LOG(LogTemp, Error, TEXT("Charger Hit with %s"), *ActorHitComp->GetCollisionProfileName().ToString());
			
			bIsAttacking = false;
			Charger->ActivateCollisionEffect();
			
			UAnimMontage* StunAnimation = Charger->GetStunAnimation();

			float StunAnimDuration = Charger->PlayAnimMontage(StunAnimation);

			FTimerHandle AnimCompleteHandle;
			GetWorld()->GetTimerManager().SetTimer(
				AnimCompleteHandle,
				FTimerDelegate::CreateWeakLambda(this, [this]() { EndTask(); }),
			StunAnimDuration,
			false
			);
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
	if (ASuraCharacterEnemyBase* OtherEnemy = Cast<ASuraCharacterEnemyBase>(OtherActor))
	{
		// UE_LOG(LogTemp, Error, TEXT("Charger Overlapped with OtherEnemy"))
		OtherEnemy->LaunchCharacter(OtherEnemy->GetActorUpVector() * 1000.f, true, true);
	}
}
