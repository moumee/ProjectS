// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Boss/SuraCharacterBossProto.h"

#include "AIController.h"
#include "ActorComponents/DamageComponent/ACBossDamageSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/Animations/SuraBossAnimInstanceProto.h"
#include "Characters/Enemies/Animations/SuraBossPartAnimInstance.h"
#include "Characters/Enemies/Boss/SuraBossAttackArea.h"
#include "Characters/Enemies/Boss/SuraBossStates.h"
#include "Kismet/GameplayStatics.h"

#define SURFACE_HEAD SurfaceType6
#define SURFACE_BODY SurfaceType7
#define SURFACE_LEFT_ARM SurfaceType8
#define SURFACE_RIGHT_ARM SurfaceType9

ASuraCharacterBossProto::ASuraCharacterBossProto()
{
	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetupAttachment(GetMesh());
	HeadMesh->SetLeaderPoseComponent(GetMesh());
	
	LeftArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftArmMesh"));
	LeftArmMesh->SetupAttachment(GetMesh());
	
	RightArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightArmMesh"));
	RightArmMesh->SetupAttachment(GetMesh());

	HeadHitColorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("HeadHitColorTimeline"));
	BodyHitColorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("BodyHitColorTimeline"));
	LeftArmHitColorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("LeftArmHitColorTimeline"));
	RightArmHitColorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("RightArmHitColorTimeline"));
	
}

void ASuraCharacterBossProto::BeginPlay()
{
	Super::BeginPlay();

	GetDamageSystemComp()->OnBodyPartDestroyed.AddUniqueDynamic(this, &ThisClass::OnBossPartDestroyed);

	GetDamageSystemComp()->OnDeath.AddUObject(this, &ThisClass::OnBossDeath);

	InitializeHitColorTimelines();

	BlackboardComp = GetController<AAIController>()->GetBlackboardComponent();

	
	UGameplayStatics::GetAllActorsOfClass(this, ASuraBossAttackArea::StaticClass(), AttackAreas);
	
}

bool ASuraCharacterBossProto::TakeDamage(const FDamageData& DamageData, AActor* DamageCauser)
{
	if (CurrentState == EBossState::Dead) return false;
	
	switch (DamageData.SurfaceType)
	{
		case SURFACE_HEAD:
			{
				HeadHitColorTimeline->PlayFromStart();
				PlayHitMontage("Head");	
			}
			break;
		case SURFACE_BODY:
			{
				BodyHitColorTimeline->PlayFromStart();
				PlayHitMontage("Body"); 
			}
			break;
		case SURFACE_LEFT_ARM:
			{
				if (GetDamageSystemComp()->GetLeftArmHealth() > 0)
				{
					LeftArmHitColorTimeline->PlayFromStart();
					PlayHitMontage("LeftArm");
				}
			}
			break;
		case SURFACE_RIGHT_ARM:
			{
				if (GetDamageSystemComp()->GetRightArmHealth() > 0)
				{
					RightArmHitColorTimeline->PlayFromStart();
					PlayHitMontage("RightArm");
				}
			}
			break;
		default:
			break;
	}
	
	return Super::TakeDamage(DamageData, DamageCauser);
}

void ASuraCharacterBossProto::SetCurrentState(EBossState NewState)
{
	CurrentState = NewState;
	BlackboardComp->SetValueAsEnum("CurrentState", static_cast<uint8>(NewState));
}

ASuraBossAttackArea* ASuraCharacterBossProto::GetAttackAreaByTag(FName Tag)
{
	for (AActor* AttackArea : AttackAreas)
	{
		if (AttackArea->ActorHasTag(Tag))
		{
			if (ASuraBossAttackArea* CastedAttackArea = Cast<ASuraBossAttackArea>(AttackArea))
			{
				return CastedAttackArea;
			}
			return nullptr;
		}
	}
	return nullptr;
}

void ASuraCharacterBossProto::OnArmDismemberMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted && CurrentState != EBossState::Dead)
	{
		SetCurrentState(EBossState::Idle);
	}
}

void ASuraCharacterBossProto::PlayHitMontage(FName SectionName)
{
	if (CurrentState == EBossState::Attack || CurrentState == EBossState::ArmDismember)
	{
		return;
	}
	
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		if (!HitMontage) return;
		if (AnimInstance->Montage_IsPlaying(HitMontage)) return;
		AnimInstance->Montage_Play(HitMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitMontage);
	}
}

void ASuraCharacterBossProto::PlayArmDismemberMontage(FName SectionName)
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		if (!ArmDismemberMontage) return;
		if (AnimInstance->Montage_IsPlaying(ArmDismemberMontage)) return;
		AnimInstance->Montage_Play(ArmDismemberMontage);
		AnimInstance->Montage_JumpToSection(SectionName, ArmDismemberMontage);
		FOnMontageEnded OnMontageEnded;
		OnMontageEnded.BindUObject(this, &ThisClass::OnArmDismemberMontageEnded);
		AnimInstance->Montage_SetEndDelegate(OnMontageEnded, ArmDismemberMontage);
	}
}

void ASuraCharacterBossProto::DismemberArm(USkeletalMeshComponent* PartMesh, USkeletalMeshComponent* LeaderMesh,
                                           FName HideBoneName)
{
	if (!PartMesh || !LeaderMesh) return;
	
	if (USuraBossPartAnimInstance* PartAnimInstance = Cast<USuraBossPartAnimInstance>(PartMesh->GetAnimInstance()))
	{
		FPoseSnapshot LeaderPoseSnapshot;
		LeaderMesh->SnapshotPose(LeaderPoseSnapshot);
		PartAnimInstance->LeaderPoseSnapshot = LeaderPoseSnapshot;
		PartAnimInstance->bIsDead = true;
	}
	
	LeaderMesh->HideBoneByName(HideBoneName, PBO_Term);
	
	PartMesh->SetSimulatePhysics(true);
	PartMesh->WakeAllRigidBodies();
}

void ASuraCharacterBossProto::OnBossPartDestroyed(TEnumAsByte<EPhysicalSurface> PhysicalSurface)
{
	switch (PhysicalSurface)
	{
		case SURFACE_LEFT_ARM:
			SetCurrentState(EBossState::ArmDismember);
			PlayArmDismemberMontage("LeftArm");
			DismemberArm(LeftArmMesh, GetMesh(), "upperarm_l");
			break;
		case SURFACE_RIGHT_ARM:
			SetCurrentState(EBossState::ArmDismember);
			PlayArmDismemberMontage("RightArm");
			DismemberArm(RightArmMesh, GetMesh(), "upperarm_r");
			break;
		default:
			break;
	}
	
}

void ASuraCharacterBossProto::OnBossDeath()
{
	SetCurrentState(EBossState::Dead);
	USuraBossAnimInstanceProto* AnimInstance = Cast<USuraBossAnimInstanceProto>(GetMesh()->GetAnimInstance());
	if (!AnimInstance) return;

	AnimInstance->Montage_Play(DeathMontage);
	AnimInstance->bIsDead = true;
	
}

void ASuraCharacterBossProto::UpdateHeadHitColor(float Alpha)
{
	if (HeadMesh)
	{
		HeadMesh->SetScalarParameterValueOnMaterials("HitColorAlpha", Alpha);
	}
}

void ASuraCharacterBossProto::UpdateBodyHitColor(float Alpha)
{
	if (USkeletalMeshComponent* BodyMesh = GetMesh())
	{
		BodyMesh->SetScalarParameterValueOnMaterials("HitColorAlpha", Alpha);
	}
}

void ASuraCharacterBossProto::UpdateLeftArmHitColor(float Alpha)
{
	if (LeftArmMesh)
	{
		LeftArmMesh->SetScalarParameterValueOnMaterials("HitColorAlpha", Alpha);
	}
}

void ASuraCharacterBossProto::UpdateRightArmHitColor(float Alpha)
{
	if (RightArmMesh)
	{
		RightArmMesh->SetScalarParameterValueOnMaterials("HitColorAlpha", Alpha);
	}
}

void ASuraCharacterBossProto::InitializeHitColorTimelines()
{
	if (HitColorCurve)
	{
		OnHeadHitColorTimelineFloat.BindUFunction(this, FName("UpdateHeadHitColor"));
		HeadHitColorTimeline->AddInterpFloat(HitColorCurve, OnHeadHitColorTimelineFloat);

		OnBodyHitColorTimelineFloat.BindUFunction(this, FName("UpdateBodyHitColor"));
		BodyHitColorTimeline->AddInterpFloat(HitColorCurve, OnBodyHitColorTimelineFloat);

		OnLeftArmHitColorTimelineFloat.BindUFunction(this, FName("UpdateLeftArmHitColor"));
		LeftArmHitColorTimeline->AddInterpFloat(HitColorCurve, OnLeftArmHitColorTimelineFloat);

		OnRightArmHitColorTimelineFloat.BindUFunction(this, FName("UpdateRightArmHitColor"));
		RightArmHitColorTimeline->AddInterpFloat(HitColorCurve, OnRightArmHitColorTimelineFloat);
	}
}

