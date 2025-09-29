// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TargetIndicatorWidget.h"

#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UTargetIndicatorWidget::SetOwningActor(AActor* InOwningActor)
{
	OwningActor = InOwningActor;
}

void UTargetIndicatorWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 이제 OwningActor 변수에 값이 제대로 들어 있으므로, 거리 계산이 정상적으로 작동합니다.
	if (!OwningActor.IsValid() || !DistanceText)
	{
		return;
	}
    
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (PlayerPawn)
	{
		const float Distance = FVector::Dist(OwningActor->GetActorLocation(), PlayerPawn->GetActorLocation());
		const int32 DistanceInMeters = FMath::RoundToInt(Distance / 100.f);

		FString DistanceString = FString::Printf(TEXT("%d M"), DistanceInMeters);
		DistanceText->SetText(FText::FromString(DistanceString));
	}
}