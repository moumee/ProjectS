// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/EnemySequentialJumpComponent.h"

#include "AIController.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Kismet/GameplayStatics.h" 
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"


UEnemySequentialJumpComponent::UEnemySequentialJumpComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UEnemySequentialJumpComponent::SetPathPoints(const TArray<FVector>& InPathPoints)
{
    PathPoints = InPathPoints;
    for (FVector PathPoint : PathPoints)
    {
        DrawDebugSphere(
        GetWorld(),                 // 현재 월드
        PathPoint,           // 구의 중심 좌표 (점을 찍을 위치)
        25.f,                       // 구의 반지름 (점의 크기)
        12,                         // 구의 디테일(선분 개수)
        FColor::Red,                // 색상
        true,                      // 계속 유지할지 여부
        -1.f,                       // 표시 시간 ( -1.f 이면 한 프레임만)
        0,                          // 뎁스 우선순위 (보통 0)
        2.f                         // 라인 두께
    );
    }
}

void UEnemySequentialJumpComponent::BeginPlay()
{
    Super::BeginPlay();
    OwnerCharacter = Cast<ASuraCharacterEnemyBase>(GetOwner());
    SetComponentTickEnabled(false);
    EnemyAnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
    
    //OnMontageEnded.BindUObject(this, &UEnemySequentialJumpComponent::JumpAnimationEndSet);
    //EnemyAnimInstance->Montage_SetEndDelegate(OnMontageEnded, JumpStartMontage);
    //EnemyAnimInstance->Montage_SetEndDelegate(OnMontageEnded, JumpLoopMontage);
    //EnemyAnimInstance->Montage_SetBlendingOutDelegate(OnMontageEnded, JumpStartMontage);
}

void UEnemySequentialJumpComponent::StartJumpSequence()
{
    // 이미 진행 중이거나 경로가 없으면 시작하지 않음
    if (bIsSequenceActive || PathPoints.IsEmpty() || !OwnerCharacter)
    {
        return;
    }
    OwnerCharacter->GetController<AEnemyBaseAIController>()->IsTraversing = true;
    bIsMoving = true;
    CurrentTargetIndex = 0;
    bIsSequenceActive = true;
    EnemyAnimInstance->OnMontageEnded.AddDynamic(this, &UEnemySequentialJumpComponent::JumpAnimationEndSet);
    JumpInitialize();
    SetComponentTickEnabled(true);
    
}
            
void UEnemySequentialJumpComponent::JumpAnimationEndSet(UAnimMontage* AnimMontage, bool bInterrupted)
{
    UE_LOG(LogTemp, Error, TEXT("Delegate Start"));
    if (ElapsedTime > TotalDuration) EnemyAnimInstance->Montage_Stop(0.1f);
    
    else if (TotalDuration- JumpEndMontage->GetPlayLength() < ElapsedTime)
    {
        UE_LOG(LogTemp, Error, TEXT("Jumploop"));
        // 죵료모션.
        EnemyAnimInstance->Montage_Play(JumpEndMontage, AnimSpeed);
    }
    else if (JumpStartMontage->GetPlayLength() < TotalDuration)
    {
            
        // 루프
        EnemyAnimInstance->Montage_Play(JumpLoopMontage);
    }
    
    
}



void UEnemySequentialJumpComponent::JumpInitialize()
{
    StartPosition = OwnerCharacter->GetActorLocation();
    EndPosition = PathPoints[CurrentTargetIndex];
    ElapsedTime = 0.f;
    EnemyAnimInstance->Montage_Play(JumpStartMontage, AnimSpeed);
}

void UEnemySequentialJumpComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!bIsSequenceActive || !OwnerCharacter || !PathPoints.IsValidIndex(CurrentTargetIndex))
    {
        SetComponentTickEnabled(false);
        return;
    }

    // 1. 경과 시간을 업데이트하고, 이동 진행률(0.0 ~ 1.0)을 계산합니다.
    ElapsedTime += DeltaTime;

    
    float Alpha = FMath::Clamp(ElapsedTime / TotalDuration, 0.f, 1.f);

    // 2. 시작점과 도착점 사이의 직선 위치를 계산합니다 (선형 보간).
    FVector CurrentLinearPosition = FMath::Lerp(StartPosition, EndPosition, Alpha);

    // 3. 포물선 모양을 만들기 위한 Z축(높이) 오프셋을 계산합니다.
    // 이 공식은 Alpha가 0.5일 때 최대값 1을, 0과 1일 때 0을 반환하는 포물선 형태입니다.
    float ZOffset = (4.f * ArcHeight) * (Alpha - (Alpha * Alpha));

    // 4. 직선 위치에 Z축 오프셋을 더하여 최종 위치를 결정합니다.
    FVector NewLocation = FVector(CurrentLinearPosition.X, CurrentLinearPosition.Y, CurrentLinearPosition.Z + ZOffset);
    OwnerCharacter->SetActorLocation(NewLocation);

    
    UAnimMontage* CurrentMontage = EnemyAnimInstance->GetCurrentActiveMontage();
    if (CurrentMontage == JumpLoopMontage && ElapsedTime > TotalDuration - JumpEndMontage->GetPlayLength())
    {
        UE_LOG(LogTemp, Error, TEXT("escape and landing"));
        EnemyAnimInstance->Montage_Play(JumpEndMontage, AnimSpeed);
    }
    //     else if (CurrentMontage != JumpLoopMontage)
    //     {
    //         EnemyAnimInstance->Montage_Play(JumpLoopMontage);
    //     }
    // }
    
    // 5. 이동이 완료되었는지 확인합니다.
    if (Alpha >= 1.f && ElapsedTime > TotalDuration + WaitingTime)
    {
        UE_LOG(LogTemp, Log, TEXT("점프 서브시퀀스 완료."));
        CurrentTargetIndex++;
        if (PathPoints.IsValidIndex(CurrentTargetIndex))
        {
            JumpInitialize();
        }
        else
        {
            FinishSequence();
        }
        
    }
    
}

void UEnemySequentialJumpComponent::FinishSequence()
{
    bIsSequenceActive = false;
    bIsMoving = false;
    PathPoints.Empty();
    
    if (OwnerCharacter)
    {
        OwnerCharacter->GetCharacterMovement()->Velocity = FVector::ZeroVector;
        OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
        AAIController* AIController = OwnerCharacter->GetController<AAIController>();
        AIController->SetFocus(Cast<ASuraPawnPlayer>(AIController->GetBlackboardComponent()->GetValueAsObject("AttackTarget")));
        Cast<AEnemyBaseAIController>(AIController)->UpdateCurrentState(EEnemyStates::Chase);
        OwnerCharacter->GetController<AEnemyBaseAIController>()->IsTraversing = false;
    }
    EnemyAnimInstance->OnMontageEnded.RemoveDynamic(this, &UEnemySequentialJumpComponent::JumpAnimationEndSet);
    OnSequenceCompleted.Broadcast();
    UE_LOG(LogTemp, Log, TEXT("점프 시퀀스 완료."));
}

