// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/EnemySequentialJumpComponent.h"

#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Kismet/GameplayStatics.h" 
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


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
}

void UEnemySequentialJumpComponent::StartJumpSequence()
{
    // 이미 진행 중이거나 경로가 없으면 시작하지 않음
    if (bIsSequenceActive || PathPoints.IsEmpty() || !OwnerCharacter)
    {
        return;
    }

    bIsMoving = true;
    CurrentTargetIndex = 0;
    bIsSequenceActive = true;
    JumpInitialize();
    SetComponentTickEnabled(true);
}

void UEnemySequentialJumpComponent::JumpAnimationSet(UAnimMontage* Montage)
{
    if (Montage == JumpStartMontage)
    {
        // '시작' 몽타주가 끝났으므로, '루프' 몽타주를 재생합니다.
        EnemyAnimInstance->Montage_Play(JumpLoopMontage);
    }
    else if (Montage == JumpLoopMontage)
    {
        // '루프' 몽타주가 한 번 끝났습니다.
        // 타이머에 남은 시간을 확인합니다.
        //float TimeRemaining = GetWorld()->GetTimerManager().GetTimerRemaining(AttackTimerHandle);

        if (TotalDuration > ElapsedTime)
        {
            // 남은 시간이 충분하면, '루프' 몽타주를 다시 재생합니다.
            EnemyAnimInstance->Montage_Play(JumpLoopMontage);
        }
        else
        {
            // 남은 시간이 부족하면, '마무리' 몽타주를 재생합니다.
            EnemyAnimInstance->Montage_Play(JumpEndMontage);
        }
    }
    else if (Montage == JumpEndMontage)
    {
        if (EnemyAnimInstance)
        {
            // 현재 재생 중인 몽타주를 모두 중지합니다.
            EnemyAnimInstance->Montage_Stop(0.1f);
            // 매우 중요: 다른 몽타주에 영향을 주지 않도록 바인딩했던 델리게이트를 반드시 해제합니다.
            //EnemyAnimInstance->OnMontageEnded.RemoveDynamic(this, &AMyCharacter::OnAttackMontageEnded);
        }
    
        // 타이머를 확실히 정리합니다.
        //GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
    }
    
}



void UEnemySequentialJumpComponent::JumpInitialize()
{
    StartPosition = OwnerCharacter->GetActorLocation();
    EndPosition = PathPoints[CurrentTargetIndex];
    TotalDuration = 0.5f;
    ArcHeight = 100;
    ElapsedTime = 0.f;

    
    //EnemyAnimInstance->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);

    // 2. 전체 공격 시간을 관리할 타이머를 설정합니다. (시간이 다 되면 공격 강제 종료)
    //GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &AMyCharacter::StopAttackSequence, TotalDuration, false);
    EnemyAnimInstance->Montage_Play(JumpStartMontage);
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

    // 5. 이동이 완료되었는지 확인합니다.
    if (Alpha >= 1.f)
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
    }
    
    OnSequenceCompleted.Broadcast();
    UE_LOG(LogTemp, Log, TEXT("점프 시퀀스 완료."));
}

