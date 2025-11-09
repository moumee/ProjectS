// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorComponents/UISystem/ACTargetIndicator.h"
#include "Components/WidgetComponent.h"
#include "UI/TargetIndicatorWidget.h"        // (경로 확인)
#include "UI/MissionTriggerBox.h"          // [추가] GetMarkerAnchor()를 위해
#include "Components/BillboardComponent.h"   // [추가] MarkerAnchor의 타입을 알기 위해
#include "GameFramework/Actor.h"
#include "Engine/CollisionProfile.h"         // [추가] NoCollision 프로파일을 위해

// Sets default values for this component's properties
UACTargetIndicator::UACTargetIndicator()
{
	PrimaryComponentTick.bCanEverTick = false;
	IndicatorWidgetComponent = nullptr; 
}


// Called when the game starts
void UACTargetIndicator::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (!Owner || !IndicatorWidgetClass)
	{
		return;
	}

	IndicatorWidgetComponent = NewObject<UWidgetComponent>(Owner, TEXT("IndicatorWidget"));
	if (!IndicatorWidgetComponent) return;
	
	USceneComponent* AttachTarget = nullptr;
	AMissionTriggerBox* TriggerBox = Cast<AMissionTriggerBox>(Owner); 

	if (TriggerBox && TriggerBox->GetMarkerAnchor())
	{
		AttachTarget = TriggerBox->GetMarkerAnchor();
	}
	else
	{
		AttachTarget = Owner->GetRootComponent();
	}
	
	IndicatorWidgetComponent->RegisterComponent(); 
	IndicatorWidgetComponent->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform);
	
	
	IndicatorWidgetComponent->SetWidgetClass(IndicatorWidgetClass);
	IndicatorWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); 
	IndicatorWidgetComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName); 
	
	UTargetIndicatorWidget* IndicatorWidget = Cast<UTargetIndicatorWidget>(IndicatorWidgetComponent->GetUserWidgetObject());
	if (IndicatorWidget)
	{
		IndicatorWidget->SetOwningActor(GetOwner());
	}
	HideIndicator();
}

void UACTargetIndicator::ShowIndicator() const
{
	if (IndicatorWidgetComponent)
	{
		IndicatorWidgetComponent->SetVisibility(true);
	}
}

void UACTargetIndicator::HideIndicator() const
{
	if (IndicatorWidgetComponent)
	{
		IndicatorWidgetComponent->SetVisibility(false);
	}
}