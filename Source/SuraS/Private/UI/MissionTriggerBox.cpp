// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MissionTriggerBox.h"

#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Components/BillboardComponent.h"
#include "Components/ShapeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/TargetingSubsystem.h"

AMissionTriggerBox::AMissionTriggerBox()
{
	MarkerAnchor = CreateDefaultSubobject<UBillboardComponent>(TEXT("MarkerAnchor"));
	MarkerAnchor->SetupAttachment(GetCollisionComponent());
	MarkerAnchor->bHiddenInGame = false; 
	
	IndicatorComponent = CreateDefaultSubobject<UACTargetIndicator>(TEXT("IndicatorComponent"));
	
	bHasBeenTriggered = false;
	bClearTargetOnTrigger = false;
	bTriggerOnceOnly = false; 
	MissionTarget = nullptr;
	
#if WITH_EDITORONLY_DATA
	UBillboardComponent* InheritedSprite = Cast<UBillboardComponent>(GetDefaultSubobjectByName(TEXT("Sprite")));
	if (InheritedSprite)
	{
		InheritedSprite->Sprite = nullptr;
		InheritedSprite->SetVisibility(false);
	}
#endif
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
		if (bTriggerOnceOnly)
		{
			bHasBeenTriggered = true;
			if (GetCollisionComponent())
			{
				GetCollisionComponent()->SetGenerateOverlapEvents(false);
			}
		}
		
		UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
		if (!GameInstance) return;

		UTargetingSubsystem* TargetingSystem = GameInstance->GetSubsystem<UTargetingSubsystem>();
		if (!TargetingSystem) return;
        
		
		if (bClearTargetOnTrigger)
		{
			TargetingSystem->ClearCurrentMissionTarget();
			UE_LOG(LogTemp, Warning, TEXT("TriggerBox activated. Mission target CLEARED."));
		}
		else if (MissionTarget) 
		{
			TargetingSystem->SetCurrentMissionTarget(MissionTarget);
			UE_LOG(LogTemp, Warning, TEXT("TriggerBox activated. Set target to: %s"), 
				*MissionTarget->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("TriggerBox activated but no target was set or cleared."));
		}
	}
}
