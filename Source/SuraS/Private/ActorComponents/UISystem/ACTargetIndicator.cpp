// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/UISystem/ACTargetIndicator.h"

#include "Components/WidgetComponent.h"
#include "UI/TargetIndicatorWidget.h"

// Sets default values for this component's properties
UACTargetIndicator::UACTargetIndicator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UACTargetIndicator::BeginPlay()
{
	Super::BeginPlay();

	// 컴포넌트가 부착된 액터(GetOwner())에 WidgetComponent를 동적으로 생성하고 부착
	// BeginPlay 시점에 미리 생성해두어, 필요할 때마다 생성/파괴하는 비용(성능 저하) x
	IndicatorWidgetComponent = NewObject<UWidgetComponent>(GetOwner(), TEXT("IndicatorWidget"));
	if (IndicatorWidgetComponent)
	{
		IndicatorWidgetComponent->RegisterComponent();
		IndicatorWidgetComponent->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		IndicatorWidgetComponent->SetWidgetClass(IndicatorWidgetClass);
		IndicatorWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

		IndicatorWidgetComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
        
		UTargetIndicatorWidget* IndicatorWidget = Cast<UTargetIndicatorWidget>(IndicatorWidgetComponent->GetUserWidgetObject());
		if (IndicatorWidget)
		{
			IndicatorWidget->SetOwningActor(GetOwner());
		}

		// 처음에는 항상 숨겨진 상태로 시작
		HideIndicator();
	}
	
	
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




