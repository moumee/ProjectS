// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/EnemySequentialJumpComponent.h"
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
    OwnerCharacter = Cast<ACharacter>(GetOwner());
    SetComponentTickEnabled(false);
}

void UEnemySequentialJumpComponent::StartJumpSequence(float InJumpSpeed)
{
    // 이미 진행 중이거나 경로가 없으면 시작하지 않음
    if (bIsSequenceActive || PathPoints.IsEmpty() || !OwnerCharacter)
    {
        return;
    }
    
    JumpSpeed = InJumpSpeed;
    CurrentTargetIndex = 0;
    bIsSequenceActive = true;

    // 첫 번째 점프 시작
    //JumpToNextPoint();
}

void UEnemySequentialJumpComponent::JumpToNextPoint()
{
    // 유효한 다음 목표 지점이 있는지 확인
    if (!PathPoints.IsValidIndex(CurrentTargetIndex))
    {
        // 모든 경로를 완료했으므로 시퀀스 종료
        FinishSequence();
        return;
    }

    const FVector StartLocation = OwnerCharacter->GetActorLocation();
    const FVector EndLocation = PathPoints[CurrentTargetIndex];
    FVector TossVelocity;
    
    // 다음 지점까지의 발사 속도 계산
    if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, TossVelocity, StartLocation, EndLocation))
    {
        // 점프 실행
        OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Flying );
        FVector tempVector = EndLocation - StartLocation;
        //OwnerCharacter->GetCharacterMovement()->Velocity = tempVector.GetSafeNormal() * JumpSpeed;
        OwnerCharacter->GetCharacterMovement()->Velocity = TossVelocity;
        
        // 도착 감지를 위해 Tick 활성화
        SetComponentTickEnabled(true);
    }
    else
    {
        // 계산 실패 시 시퀀스 바로 종료
        UE_LOG(LogTemp, Warning, TEXT("경로 계산 실패: 인덱스 %d (%s) -> 인덱스 %d (%s)"),
        CurrentTargetIndex - 1, // 이전 지점 (만약 있다면)
        *StartLocation.ToString(),
        CurrentTargetIndex,
        *EndLocation.ToString());
        FinishSequence();
    }
}

void UEnemySequentialJumpComponent::FinishSequence()
{
    bIsSequenceActive = false;
    PathPoints.Empty();
    
    if (OwnerCharacter)
    {
        OwnerCharacter->GetCharacterMovement()->Velocity = FVector::ZeroVector;
        OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    }
    
    OnSequenceCompleted.Broadcast();
    UE_LOG(LogTemp, Log, TEXT("점프 시퀀스 완료."));
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
        bIsMoving = false;
        SetComponentTickEnabled(false); // Tick 비활성화
        OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking); // 원래 상태로 복귀
    }
}



