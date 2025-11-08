// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MissionTriggerBox.h"

#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Components/ShapeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/TargetingSubsystem.h"

AMissionTriggerBox::AMissionTriggerBox()
{
	bHasBeenTriggered = false;
	bClearTargetOnTrigger = false;
}

void AMissionTriggerBox::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (bTriggerOnceOnly && bHasBeenTriggered)
	{
		return;
	}

	if (OtherActor && OtherActor->IsA(ASuraPawnPlayer::StaticClass())) 
	{
		// 트리거 플래그 설정 (bTriggerOnceOnly가 True인 경우)
		if (bTriggerOnceOnly)
		{
			bHasBeenTriggered = true;
			if (GetCollisionComponent())
			{
				GetCollisionComponent()->SetGenerateOverlapEvents(false);
			}
		}
        
		// UTargetingSubsystem 호출
		UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
		if (!GameInstance) return;

		UTargetingSubsystem* TargetingSystem = GameInstance->GetSubsystem<UTargetingSubsystem>();
		if (!TargetingSystem) return;
        
		
		if (bClearTargetOnTrigger)
		{
			// 제거 모드가 켜져 있으면, 타겟 설정 여부와 관계없이 무조건 제거
			TargetingSystem->ClearCurrentMissionTarget();
			UE_LOG(LogTemp, Warning, TEXT("TriggerBox activated. Mission target CLEARED."));
		}
		// 설정 모드 (제거 모드가 아니거나, 제거 모드가 꺼져있을 경우)
		else if (MissionTarget) 
		{
			// MissionTarget이 유효할 경우에만 설정
			TargetingSystem->SetCurrentMissionTarget(MissionTarget);
			UE_LOG(LogTemp, Warning, TEXT("TriggerBox activated. Set target to: %s"), 
				*MissionTarget->GetName());
		}
		else
		{
			// 설정 모드인데 Target이 할당되지 않은 경우 경고
			UE_LOG(LogTemp, Warning, TEXT("TriggerBox activated but no target was set or cleared."));
		}
	}
}
