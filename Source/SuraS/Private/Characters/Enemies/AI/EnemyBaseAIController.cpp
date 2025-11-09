// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Enemies/AI/EnemyBaseAIController.h"

#include "ActorComponents/AttackComponents/ACPlayerAttackTokens.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/SuraCharacterEnemyTurret.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Structures/Enemies/EnemyAttributesData.h"

AEnemyBaseAIController::AEnemyBaseAIController(FObjectInitializer const& ObjectInitializer) 
{
	SetupPerceptionSystem();
}

void AEnemyBaseAIController::InitializeBlackBoard(float StrafeRadius, float ChaseStrafeRadius, float AttackRadius, float AttackRate)
{
	GetBlackboardComponent()->SetValueAsFloat("StrafeRadius", StrafeRadius);
	GetBlackboardComponent()->SetValueAsFloat("ChaseStrafeRadius", ChaseStrafeRadius);
	GetBlackboardComponent()->SetValueAsFloat("AttackRadius", AttackRadius);
	GetBlackboardComponent()->SetValueAsFloat("AttackRate", AttackRate);
}

void AEnemyBaseAIController::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);

	if (ASuraCharacterEnemyBase* Enemy = Cast<ASuraCharacterEnemyBase>(PossessedPawn))
	{
		CachedPossessedPawn = Enemy;
		
		if (UBehaviorTree* const BehaviorTree = Enemy->GetBehaviorTree())
		{
			UBlackboardComponent* Bboard;
			UseBlackboard(BehaviorTree->BlackboardAsset, Bboard);
			Blackboard = Bboard; // "Blackboard" is an already existing variable name in AAIController class

			if (const auto EnemyAttributesData = Enemy->EnemyAttributesDT.DataTable->FindRow<FEnemyAttributesData>(Enemy->GetEnemyType(), ""))
			{
				InitializeBlackBoard(EnemyAttributesData->StrafeRadius, EnemyAttributesData->ChaseStrafeRadius, EnemyAttributesData->AttackRadius, EnemyAttributesData->AttackRate);

				SightConfig->SightRadius = EnemyAttributesData->MaxSightRadius;
				SightConfig->LoseSightRadius = SightConfig->SightRadius + 100.f;
				SightConfig->PeripheralVisionAngleDegrees = EnemyAttributesData->SightAngle;
			}

			RunBehaviorTree(BehaviorTree);

			UpdateCurrentState(EEnemyStates::Passive);

			AttachToPawn(Enemy);

			Enemy->SetUpAIController(this);
		}
	}
}

void AEnemyBaseAIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Comp")));

		SightConfig->SightRadius = 1000.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 100.f;
		SightConfig->PeripheralVisionAngleDegrees = 180.f;
		SightConfig->SetMaxAge(5.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 0.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyBaseAIController::OnTargetSighted);
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}
}

void AEnemyBaseAIController::OnTargetSighted(AActor* SeenTarget, FAIStimulus const Stimulus)
{
	if (Cast<ASuraCharacterEnemyTurret>(CachedPossessedPawn.Get()))
	{
		if (ASuraPawnPlayer* const Player = Cast<ASuraPawnPlayer>(SeenTarget))
		{
			GetBlackboardComponent()->SetValueAsObject("AttackTarget", Player);
			UpdateCurrentState(EEnemyStates::Attacking);
		}
	}
	else
	{
		if (GetCurrentState() != EEnemyStates::Climbing && GetCurrentState() != EEnemyStates::CoopAttacking)
			SetStateToChaseOrPursue(SeenTarget);
	}
}

void AEnemyBaseAIController::UpdateCurrentState(EEnemyStates NewState)
{
	if (GetCurrentState() != NewState)
	{
		_CurrentState = NewState;
		GetBlackboardComponent()->SetValueAsEnum("State", static_cast<uint8>(_CurrentState));
	}
}

void AEnemyBaseAIController::SetStateToChaseOrPursue(AActor* TargetActor)
{
	if (GetCurrentState() != EEnemyStates::Pursue && GetCurrentState() != EEnemyStates::Attacking)
	{
		if (ASuraPawnPlayer* const Player = Cast<ASuraPawnPlayer>(TargetActor))
		{
			GetBlackboardComponent()->SetValueAsObject("AttackTarget", Player);

			int PursuitIndex = Player->GetAttackTokensComponent()->ReservePursuitToken(1);
		
			if (PursuitIndex != -1)
			{
				PursuitIndex = Player->GetAttackTokensComponent()->GetMaxEnemyPursuitTokens() - PursuitIndex - 1;
				
				GetBlackboardComponent()->SetValueAsInt("PursuitIndex", PursuitIndex);
				UpdateCurrentState(EEnemyStates::Pursue);
			}
			else
			{
				UpdateCurrentState(EEnemyStates::Chase);
			}
		}
	}
}

void AEnemyBaseAIController::SetStateToCoopAttack(AActor* Ally, bool bIsPitcher)
{
	UpdateCurrentState(EEnemyStates::CoopAttacking);
	GetBlackboardComponent()->SetValueAsObject("CoopAlly", Ally);
	GetBlackboardComponent()->SetValueAsBool("IsCoopThrower", bIsPitcher);
}

void AEnemyBaseAIController::EndPursueState()
{
	if (GetCurrentState() == EEnemyStates::Pursue || GetCurrentState() == EEnemyStates::Attacking)
	{
		if (ASuraPawnPlayer* const Player = Cast<ASuraPawnPlayer>(GetBlackboardComponent()->GetValueAsObject("AttackTarget")))
		{
			Player->GetAttackTokensComponent()->ReturnPursuitToken(1);
		}
	}
}

ASuraPawnPlayer* AEnemyBaseAIController::GetAttackTarget()
{
	ASuraPawnPlayer* const Player = Cast<ASuraPawnPlayer>(GetBlackboardComponent()->GetValueAsObject("AttackTarget"));
	
	return Player;
}
