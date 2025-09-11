// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Boss/SuraCharacterBossProto.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/Boss/SuraBossAttackArea.h"
#include "Characters/Enemies/Boss/SuraBossStates.h"


ASuraCharacterBossProto::ASuraCharacterBossProto()
{
	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetupAttachment(GetMesh());
	HeadMesh->SetLeaderPoseComponent(GetMesh());
	
	LeftArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftArmMesh"));
	LeftArmMesh->SetupAttachment(GetMesh());
	LeftArmMesh->SetLeaderPoseComponent(GetMesh());
	
	RightArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightArmMesh"));
	RightArmMesh->SetupAttachment(GetMesh());
	RightArmMesh->SetLeaderPoseComponent(GetMesh());

	HeadHitColorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("HeadHitColorTimeline"));
	BodyHitColorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("BodyHitColorTimeline"));
	LeftArmHitColorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("LeftArmHitColorTimeline"));
	RightArmHitColorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("RightArmHitColorTimeline"));
	
}

void ASuraCharacterBossProto::BeginPlay()
{
	Super::BeginPlay();

	InitializeHitColorTimelines();

	BlackboardComp = GetController<AAIController>()->GetBlackboardComponent();
	
}

bool ASuraCharacterBossProto::TakeDamage(const FDamageData& DamageData, AActor* DamageCauser)
{
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
			PlayHitMontage("Head");	
		}
		break;
	case SURFACE_LEFT_ARM:
		{
			LeftArmHitColorTimeline->PlayFromStart();
			PlayHitMontage("LeftArm");
		}
		break;
	case SURFACE_RIGHT_ARM:
		{
			RightArmHitColorTimeline->PlayFromStart();
			PlayHitMontage("RightArm");
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
	BlackboardComp->SetValueAsEnum("CurrentState",static_cast<uint8>(NewState));
}

void ASuraCharacterBossProto::PlayHitMontage(FName SectionName)
{
	if (CurrentState == EBossState::Attack) return;
	
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		if (!HitMontage) return;
		if (AnimInstance->Montage_IsPlaying(HitMontage)) return;
		AnimInstance->Montage_Play(HitMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitMontage);
	}
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

