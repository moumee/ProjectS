// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Attacks/BTT_GetYeeted.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTT_GetYeeted::UBTT_GetYeeted()
{
	NodeName = "Get Yeeted";
	bCreateNodeInstance = true;
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTT_GetYeeted::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	/*FBTTGetYeetedTaskMemory* Mem = CastInstanceNodeMemory<FBTTGetYeetedTaskMemory>(NodeMemory);
	check(Mem);*/
	
	// Mem->CachedEnemy = Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter());
	CachedEnemy = Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter());

	// Mem->bIsDoneGettingYeeted = false;
	bIsDoneGettingYeeted = false;

	if (ASuraCharacterEnemyBase* Enemy = CachedEnemy.Get())
	{
		Enemy->GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &UBTT_GetYeeted::OnHit);
		// Mem->CachedEnemy.Get()->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &UBTT_GetYeeted::OnOverlapBegin);
				
		UAnimInstance* const EnemyAnimInstance = Enemy->GetMesh()->GetAnimInstance();
		UAnimMontage* FallingMontage = Enemy->GetFallingMontage();

		EnemyAnimInstance->Montage_Play(FallingMontage);
	}
	
	return EBTNodeResult::InProgress;
}

void UBTT_GetYeeted::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	/*FBTTGetYeetedTaskMemory* Mem = CastInstanceNodeMemory<FBTTGetYeetedTaskMemory>(NodeMemory);

	if (Mem->CachedEnemy.IsValid())
	{
		if (Mem->CachedEnemy.Get()->GetCharacterMovement()->IsFalling())
		{
			// UE_LOG(LogTemp, Log, TEXT("Getting Yeeted"));

			if (!Mem->bIsFalling) // one time init
			{
				Mem->bIsFalling = true;

				Mem->CachedEnemy.Get()->GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &UBTT_GetYeeted::OnHit);
				// Mem->CachedEnemy.Get()->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &UBTT_GetYeeted::OnOverlapBegin);
				
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
			Mem->CachedEnemy.Get()->GetCapsuleComponent()->OnComponentHit.RemoveDynamic(this, &UBTT_GetYeeted::OnHit);
			// Mem->CachedEnemy.Get()->GetCapsuleComponent()->OnComponentBeginOverlap.RemoveDynamic(this, &UBTT_GetYeeted::OnOverlapBegin);

			UAnimInstance* const EnemyAnimInstance = Mem->CachedEnemy.Get()->GetMesh()->GetAnimInstance();
			EnemyAnimInstance->Montage_Stop(0.2f);
			FRotator TargetRotation = Mem->CachedEnemy.Get()->GetAIController()->GetBlackboardComponent()->GetValueAsRotator("TargetRotation");
			Mem->CachedEnemy.Get()->SetActorRotation(FRotator(0, TargetRotation.Yaw, 0));

			if (Mem->CachedEnemy.Get()->GetAIController()->GetBrainComponent()->IsPaused())
			{
				Mem->CachedEnemy.Get()->GetAIController()->GetBrainComponent()->RestartLogic();
				Mem->CachedEnemy.Get()->GetAIController()->SetStateToChaseOrPursue(Mem->CachedEnemy.Get());
			}

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}*/

	/*if (ASuraCharacterEnemyBase* Enemy = CachedEnemy.Get())
	{
		if (Enemy->GetCharacterMovement()->IsFalling())
		{
			// UE_LOG(LogTemp, Log, TEXT("Getting Yeeted"));

			if (!bIsFalling) // one time init
			{
				bIsFalling = true;
				
			}
		}
		else
		{
			if (bIsFalling)
			{
				bIsFalling = false;
				bIsDoneGettingYeeted = true;
			}
		}

		if (bIsDoneGettingYeeted)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}*/

	ElapsedTime += DeltaSeconds;

	if (ElapsedTime > MaxElapsedTime)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
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
	
	/*UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent());
	FBTTGetYeetedTaskMemory* Mem = CastInstanceNodeMemory<FBTTGetYeetedTaskMemory>(OwnerComp->GetNodeMemory(this, OwnerComp->FindInstanceContainingNode(this)));
	if (!Mem) return;
	
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

		Mem->bIsDoneGettingYeeted = true;
	}*/
	
	ASuraPawnPlayer* Player = Cast<ASuraPawnPlayer>(OtherActor);

	if (IsValid(Player))
	{
		// UE_LOG(LogTemp, Log, TEXT("OnHit %s"), *Player->GetName());
		if (ASuraCharacterEnemyBase* Enemy = CachedEnemy.Get())
		{
			FVector DirectionToOther = (OtherActor->GetActorLocation() - Enemy->GetActorLocation()).GetSafeNormal2D();
			FVector ChargerRightVector = Enemy->GetActorRightVector().GetSafeNormal2D();

			float SideSign = FMath::Sign(FVector::DotProduct(ChargerRightVector, DirectionToOther));

			FVector PerpendicularDirection = ChargerRightVector * SideSign;
				
			FDamageData DamageData;
			DamageData.DamageAmount = Enemy->GetAttackDamageAmount();
			DamageData.DamageType = EDamageType::Charge;
			DamageData.ImpulseDirection = PerpendicularDirection;
			DamageData.ImpulseMagnitude = 1000.f;
			
			Player->TakeDamage(DamageData, Enemy);

			bIsDoneGettingYeeted = true;
		}
	}
	
	/*else if (OtherComp && OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_WorldStatic)
	{
		UE_LOG(LogTemp, Log, TEXT("OnHit %s"), *OtherComp->GetName());
		
		Mem->bIsDoneGettingYeeted = true;

		if (Mem->CachedEnemy.Get()->GetAIController()->GetBrainComponent()->IsPaused())
		{
			// UE_LOG(LogTemp, Error, TEXT("AI Paused"));
			Mem->CachedEnemy.Get()->GetAIController()->GetBrainComponent()->RestartLogic();
			Mem->CachedEnemy.Get()->GetAIController()->SetStateToChaseOrPursue(Mem->CachedEnemy.Get());
		}
	}*/
}

/*void UBTT_GetYeeted::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
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
	
		Mem->bIsDoneGettingYeeted = true;

		if (Mem->CachedEnemy.Get()->GetAIController()->GetBrainComponent()->IsPaused())
		{
			// UE_LOG(LogTemp, Error, TEXT("AI Paused"));
			Mem->CachedEnemy.Get()->GetAIController()->GetBrainComponent()->RestartLogic();
			Mem->CachedEnemy.Get()->GetAIController()->SetStateToChaseOrPursue(Mem->CachedEnemy.Get());
		}
	}
}*/

/*uint16 UBTT_GetYeeted::GetInstanceMemorySize() const
{
	return sizeof(FBTTGetYeetedTaskMemory);
}*/

void UBTT_GetYeeted::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	auto Enemy = CachedEnemy.Get();
	if (!Enemy) return;
	
	// UE_LOG(LogTemp, Error, TEXT("Done Getting Yeeted"));
	Enemy->GetCapsuleComponent()->OnComponentHit.RemoveDynamic(this, &UBTT_GetYeeted::OnHit);
	// Mem->CachedEnemy.Get()->GetCapsuleComponent()->OnComponentBeginOverlap.RemoveDynamic(this, &UBTT_GetYeeted::OnOverlapBegin);

	UAnimInstance* const EnemyAnimInstance = Enemy->GetMesh()->GetAnimInstance();
	EnemyAnimInstance->Montage_Stop(0.2f);
	FRotator TargetRotation = Enemy->GetAIController()->GetBlackboardComponent()->GetValueAsRotator("TargetRotation");
	Enemy->SetActorRotation(FRotator(0, TargetRotation.Yaw, 0));

	if (Enemy->GetAIController()->GetBrainComponent()->IsPaused())
	{
		Enemy->GetAIController()->GetBrainComponent()->RestartLogic();
		Enemy->GetAIController()->GetBlackboardComponent()->SetValueAsObject("CoopAlly", nullptr);
		Enemy->GetAIController()->GetBlackboardComponent()->SetValueAsBool("IsCoopThrower", false);
		Enemy->GetAIController()->SetStateToChaseOrPursue(Enemy);
	}
}