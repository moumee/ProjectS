// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Attacks/BTT_GetYeeted.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTT_GetYeeted::UBTT_GetYeeted(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Get Yeeted";
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTT_GetYeeted::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTTGetYeetedTaskMemory* Mem = CastInstanceNodeMemory<FBTTGetYeetedTaskMemory>(NodeMemory);
	check(Mem);
	
	Mem->CachedEnemy = Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter());

	if (Mem->CachedEnemy.IsValid())
	{
		Mem->CachedEnemy.Get()->GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &UBTT_GetYeeted::OnHit);
		Mem->CachedEnemy.Get()->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &UBTT_GetYeeted::OnOverlapBegin);
	}

	Mem->bIsDoneGettingYeeted = false;
	
	return EBTNodeResult::InProgress;
}

void UBTT_GetYeeted::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FBTTGetYeetedTaskMemory* Mem = CastInstanceNodeMemory<FBTTGetYeetedTaskMemory>(NodeMemory);

	if (Mem->CachedEnemy.IsValid())
	{
		if (Mem->CachedEnemy.Get()->GetCharacterMovement()->IsFalling())
		{
			// UE_LOG(LogTemp, Log, TEXT("Getting Yeeted"));

			if (!Mem->bIsFalling)
			{
				Mem->bIsFalling = true;
				UAnimInstance* const EnemyAnimInstance = Mem->CachedEnemy.Get()->GetMesh()->GetAnimInstance();
				UAnimMontage* FallingMontage = Mem->CachedEnemy.Get()->GetFallingMontage();

				EnemyAnimInstance->Montage_Play(FallingMontage);
			}
		}
		else
		{
			if (Mem->bIsFalling)
			{
				Mem->bIsFalling = false;
				Mem->bIsDoneGettingYeeted = true;
			}
		}

		if (Mem->bIsDoneGettingYeeted)
		{
			// UE_LOG(LogTemp, Error, TEXT("Done Getting Yeeted"));

			UAnimInstance* const EnemyAnimInstance = Mem->CachedEnemy.Get()->GetMesh()->GetAnimInstance();
			EnemyAnimInstance->Montage_Stop(0.2f);
			Mem->CachedEnemy.Get()->GetCapsuleComponent()->OnComponentHit.RemoveDynamic(this, &UBTT_GetYeeted::OnHit);
			FRotator TargetRotation = Mem->CachedEnemy.Get()->GetAIController()->GetBlackboardComponent()->GetValueAsRotator("TargetRotation");
			Mem->CachedEnemy.Get()->SetActorRotation(FRotator(0, TargetRotation.Yaw, 0));

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}

void UBTT_GetYeeted::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = HitComponent->GetOwner();
	if (!MyOwner) return;

	AAIController* AIController = nullptr;
	
	if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(MyOwner))
	{
		AIController = Cast<AAIController>(Enemy->GetAIController());
	}
	
	if (!AIController) return;
	
	UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent());
	FBTTGetYeetedTaskMemory* Mem = CastInstanceNodeMemory<FBTTGetYeetedTaskMemory>(OwnerComp->GetNodeMemory(this, OwnerComp->FindInstanceContainingNode(this)));
	
	if (ASuraPawnPlayer* Player = Cast<ASuraPawnPlayer>(OtherActor))
	{
		// UE_LOG(LogTemp, Log, TEXT("OnHit %s"), *Player->GetName());

		FVector DirectionToOther = (OtherActor->GetActorLocation() - Mem->CachedEnemy.Get()->GetActorLocation()).GetSafeNormal2D();
		FVector ChargerRightVector = Mem->CachedEnemy.Get()->GetActorRightVector().GetSafeNormal2D();

		float SideSign = FMath::Sign(FVector::DotProduct(ChargerRightVector, DirectionToOther));

		FVector PerpendicularDirection = ChargerRightVector * SideSign;
				
		FDamageData DamageData;
		DamageData.DamageAmount = Mem->CachedEnemy.Get()->GetAttackDamageAmount();
		DamageData.DamageType = EDamageType::Charge;
		DamageData.ImpulseDirection = PerpendicularDirection;
		DamageData.ImpulseMagnitude = 1000.f;
			
		Player->TakeDamage(DamageData, Mem->CachedEnemy.Get());

		Mem->CachedEnemy.Get()->GetCapsuleComponent()->OnComponentHit.RemoveDynamic(this, &UBTT_GetYeeted::OnHit);

		Mem->bIsDoneGettingYeeted = true;
	}
	else if (OtherComp && OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_WorldStatic)
	{
		Mem->CachedEnemy.Get()->GetCapsuleComponent()->OnComponentHit.RemoveDynamic(this, &UBTT_GetYeeted::OnHit);
		
		Mem->bIsDoneGettingYeeted = true;

		if (Mem->CachedEnemy.Get()->GetAIController()->GetBrainComponent()->IsPaused())
		{
			// UE_LOG(LogTemp, Error, TEXT("AI Paused"));
			Mem->CachedEnemy.Get()->GetAIController()->GetBrainComponent()->RestartLogic();
			Mem->CachedEnemy.Get()->GetAIController()->SetStateToChaseOrPursue(Mem->CachedEnemy.Get());
		}
	}
}

void UBTT_GetYeeted::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* MyOwner = OverlappedComp->GetOwner();
	if (!MyOwner) return;

	AAIController* AIController = nullptr;
	
	if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(MyOwner))
	{
		AIController = Cast<AAIController>(Enemy->GetAIController());
	}
	
	if (!AIController) return;
	
	UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent());
	FBTTGetYeetedTaskMemory* Mem = CastInstanceNodeMemory<FBTTGetYeetedTaskMemory>(OwnerComp->GetNodeMemory(this, OwnerComp->FindInstanceContainingNode(this)));
	
	if (OtherComp && OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_WorldStatic)
	{
		// UE_LOG(LogTemp, Error, TEXT("%s"), *OtherActor->GetName());

		Mem->CachedEnemy.Get()->GetCapsuleComponent()->OnComponentHit.RemoveDynamic(this, &UBTT_GetYeeted::OnHit);
	
		Mem->bIsDoneGettingYeeted = true;

		if (Mem->CachedEnemy.Get()->GetAIController()->GetBrainComponent()->IsPaused())
		{
			// UE_LOG(LogTemp, Error, TEXT("AI Paused"));
			Mem->CachedEnemy.Get()->GetAIController()->GetBrainComponent()->RestartLogic();
			Mem->CachedEnemy.Get()->GetAIController()->SetStateToChaseOrPursue(Mem->CachedEnemy.Get());
		}
	}
}

uint16 UBTT_GetYeeted::GetInstanceMemorySize() const
{
	return sizeof(FBTTGetYeetedTaskMemory);
}
