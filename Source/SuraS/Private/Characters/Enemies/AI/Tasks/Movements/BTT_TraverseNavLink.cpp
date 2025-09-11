// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Movements/BTT_TraverseNavLink.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/Character.h"
#include "Characters/Enemies/AI/EnemySequentialJumpComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h" 

UBTT_TraverseNavLink::UBTT_TraverseNavLink()
{
    NodeName = "Traverse NavLink";
}

EBTNodeResult::Type UBTT_TraverseNavLink::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    
    //AI, navlink, 캐릭터 가져오기
    const AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;
    
    UPathFollowingComponent* PathFollowingComp = AIController->GetPathFollowingComponent();
    if (!PathFollowingComp) return EBTNodeResult::Failed;

    //FNavigationLink* CurrentNavLink = PathFollowingComp->GetCurrentNavLink();
    
    const ACharacter* Character = AIController->GetCharacter();
    if (!Character) return EBTNodeResult::Failed;

    UE_LOG(LogTemp, Error, TEXT("Traverse Task"));
    if (UEnemySequentialJumpComponent* MovementComp = Character->FindComponentByClass<UEnemySequentialJumpComponent>())
    {
        // 이동 완료 시 호출될 함수를 바인딩합니다.
        // FTimerDelegate를 사용하거나 AddUObject/AddDynamic을 사용해 콜백을 설정할 수 있습니다.
        // 여기서는 간단하게 타이머를 사용해 보겠습니다.
        //FTimerHandle TimerHandle;
        //FTimerDelegate CompletionDelegate = FTimerDelegate::CreateUObject(this, &UBTT_TraverseNavLink::OnTraversalCompleted, &OwnerComp);
            
        // 이동이 끝났다는 신호를 기다리기 위해 델리게이트에 함수 바인딩
        // NOTE: 실제로는 이동 컴포넌트의 델리게이트에 직접 바인딩하는 것이 더 정확합니다.
        // MovementComp->OnSequenceCompleted.AddLambda([this, &OwnerComp]()
        // {
        //     FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        // });
    
    

    
        // 이동을 시작시킵니다.
        MovementComp->StartJumpSequence();
            
        // 즉시 종료되지 않고, 이동이 완료될 때까지 태스크를 '진행 중' 상태로 유지합니다.
        return EBTNodeResult::InProgress;
    }
    //현재 AI가 NavLink 구간을 따라가고 있는지 확인
    // if (PathFollowingComp->IsCurrentSegmentNavigationLink())
    // {
    //     // 캐릭터에 부착된 이동 컴포넌트를 가져오기
    //     
    //     
    //     
    // }

    //NavLink를 따라가는 상황이 아니면 즉시 실패를 반환합니다.
    return EBTNodeResult::Failed;
}

// 이 함수는 이동 컴포넌트의 OnSequenceCompleted 델리게이트에 의해 호출됩니다.
void UBTT_TraverseNavLink::OnTraversalCompleted(UBehaviorTreeComponent* BehaviorTreeComponent)
{
    if (BehaviorTreeComponent)
    {
        // Latent Task를 성공으로 종료시킵니다.
        FinishLatentTask(*BehaviorTreeComponent, EBTNodeResult::Succeeded);
    }
}