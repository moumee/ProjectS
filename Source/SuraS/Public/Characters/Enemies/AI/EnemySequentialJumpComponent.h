#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemySequentialJumpComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSequenceCompleted);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURAS_API UEnemySequentialJumpComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEnemySequentialJumpComponent();
	
	void StartJumpSequence();

	// 시퀀스 완료 시 방송할 델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnSequenceCompleted OnSequenceCompleted;

	void SetPathPoints(const TArray<FVector>& InPathPoints);

	UPROPERTY(EditAnywhere, Category = "Jump Settings")
	UAnimMontage* JumpStartMontage;
	UPROPERTY(EditAnywhere, Category = "Jump Settings")
	UAnimMontage* JumpLoopMontage;
	UPROPERTY(EditAnywhere, Category = "Jump Settings")
	UAnimMontage* JumpEndMontage;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// 점프 로직
	UFUNCTION()
	void JumpAnimationEndSet(UAnimMontage* AnimMontage, bool bInterrupted);
	// 시퀀스 종료 로직
	void FinishSequence();

	void JumpInitialize();

	// -- 설정 변수 --
	UPROPERTY(EditAnywhere, Category = "Jump Settings")
	float JumpSpeed = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Jump Settings")
	float ArcParam = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Jump Settings")
	float ArrivalTolerance = 100.f;

	// -- 상태 변수 --
	UPROPERTY()
	TObjectPtr<ACharacter> OwnerCharacter;

	// 전체 경로
	TArray<FVector> PathPoints;
	// 현재 목표 지점의 인덱스
	int32 CurrentTargetIndex;
	// 시퀀스가 진행 중인지 여부
	bool bIsSequenceActive;

	// -- 상태 변수 --
	bool bIsMoving;
	float ElapsedTime;      // 이동 경과 시간
	UPROPERTY(EditAnywhere, Category = "Jump Settings")
	float TotalDuration = 0.7f;;    // 총 이동 시간
	UPROPERTY(EditAnywhere, Category = "Jump Settings")
	float ArcHeight = 100;;        // 포물선 최고 높이
	float jumpdage;
	FVector StartPosition;    // 시작 월드 좌표
	FVector EndPosition;      // 도착 월드 좌표
	bool bIsWating = false;
	UPROPERTY(EditAnywhere, Category = "Jump Settings")
	float WaitingTime = 0.2f;;  
	UPROPERTY()
	UAnimInstance* EnemyAnimInstance;
	UPROPERTY(EditAnywhere, Category = "Jump Settings")
	float AnimSpeed = 2.f;
};