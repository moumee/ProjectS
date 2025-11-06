// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Boss/Tasks/BTT_BossRangedTargeting.h"

#include "AIController.h"
#include "NiagaraComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/Boss/SuraCharacterBossProto.h"
#include "Kismet/KismetSystemLibrary.h"

UBTT_BossRangedTargeting::UBTT_BossRangedTargeting()
{
	NodeName = "Boss Ranged Targeting";

	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTT_BossRangedTargeting::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBossRangedTargetingMemory* Memory = CastInstanceNodeMemory<FBossRangedTargetingMemory>(NodeMemory);
	check(Memory);
	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!OwningPawn) return EBTNodeResult::Failed;
	Memory->Boss = Cast<ASuraCharacterBossProto>(OwningPawn);
	ASuraCharacterBossProto* Boss = Memory->Boss.Get();
	if (!Boss) return EBTNodeResult::Failed;

	UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName);
	if (!TargetObject) return EBTNodeResult::Failed;
	Memory->TargetActor = Cast<AActor>(TargetObject);
	AActor* TargetActor = Memory->TargetActor.Get();
	if (!TargetActor) return EBTNodeResult::Failed;

	Memory->TargetingDuration = Boss->GetRangedAttackTargetingDuration();
	Memory->RangedAttackStartTime = OwnerComp.GetWorld()->GetTimeSeconds();
	
	Boss->SetCurrentState(EBossState::Attack);
	
	UNiagaraComponent* LaserComp = Boss->GetLaserNiagaraComponent();
	if (!LaserComp) return EBTNodeResult::Failed;
	LaserComp->Activate();
	
	return EBTNodeResult::InProgress;
}

void UBTT_BossRangedTargeting::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FBossRangedTargetingMemory* Memory = CastInstanceNodeMemory<FBossRangedTargetingMemory>(NodeMemory);
	AActor* PlayerActor = Memory->TargetActor.Get();
	ASuraCharacterBossProto* Boss = Memory->Boss.Get();
	if (!PlayerActor || !Boss) FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	UNiagaraComponent* LaserComp = Boss->GetLaserNiagaraComponent();
	if (!IsValid(LaserComp)) FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	FHitResult HitResult;
	bool bHit = UKismetSystemLibrary::LineTraceSingle(Boss, Boss->GetMesh()->GetSocketLocation("Muzzle"), PlayerActor->GetActorLocation(),
		UEngineTypes::ConvertToTraceType(ECC_WorldStatic), false, { Boss }, EDrawDebugTrace::ForOneFrame,
		HitResult, false);

	FVector LaserEndLocation = PlayerActor->GetActorLocation();
	if (HitResult.IsValidBlockingHit() && bHit)
	{
		LaserEndLocation = HitResult.ImpactPoint;
	}
	
	LaserComp->SetVectorParameter("User.BeamEnd", LaserEndLocation);

	float CurrentTime = Boss->GetWorld()->GetTimeSeconds();
	if (CurrentTime - Memory->RangedAttackStartTime >= Memory->TargetingDuration)
	{
		Boss->SetLaserFireEnd(LaserEndLocation);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	
}

uint16 UBTT_BossRangedTargeting::GetInstanceMemorySize() const
{
	return sizeof(FBossRangedTargetingMemory);
}

void UBTT_BossRangedTargeting::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		TargetKey.ResolveSelectedKey(*BBAsset);
	}
}

void UBTT_BossRangedTargeting::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	FBossRangedTargetingMemory* Memory = CastInstanceNodeMemory<FBossRangedTargetingMemory>(NodeMemory);
	
	if (ASuraCharacterBossProto* Boss = Memory->Boss.Get())
	{
		UNiagaraComponent* LaserNiagara = Boss->GetLaserNiagaraComponent();
		if (ensureAlwaysMsgf(LaserNiagara, TEXT("Laser niagara is not assigned in the boss blueprint!!")))
		{
			if (TaskResult != EBTNodeResult::Succeeded)	
				LaserNiagara->DeactivateImmediate();
			Boss->LastSavedMuzzlePositionBeforeFire = Boss->GetMesh()->GetSocketLocation("Muzzle");
			LaserNiagara->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		}
			
	}
}


