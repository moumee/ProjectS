// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DamageIndicatorWidget.h"

#include "VectorTypes.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Components/Image.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Kismet/GameplayStatics.h"

void UDamageIndicatorWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 이 위젯을 소유한 플레이어의 Pawn을 가져와 PlayerPawn 변수에 저장합니다.
    APawn* OwningPawn = GetOwningPlayerPawn();
    if (OwningPawn)
    {
       PlayerPawn = Cast<ASuraPawnPlayer>(OwningPawn);
    }

    // 위젯 블루프린트에서 UPROPERTY(meta=(BindWidget))으로 바인딩된 이미지 위젯들이
    // 유효한지 확인합니다.
    if (!IsValid(MeleeIndicatorArrow) || !IsValid(RangedIndicatorArrow))
    {
       UE_LOG(LogTemp, Error, TEXT("Melee or Ranged IndicatorArrows are not bound in DamageIndicatorWidget."));
    }
    
    // 두 이미지 모두 초기에는 숨겨둡니다.
    if(IsValid(MeleeIndicatorArrow))
    {
       MeleeIndicatorArrow->SetVisibility(ESlateVisibility::Hidden);
    }
    if(IsValid(RangedIndicatorArrow))
    {
       RangedIndicatorArrow->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UDamageIndicatorWidget::HandleDamageTaken(AActor* DamageCauser)
{
    // 피격 유발자가 유효한지 확인합니다.
    if (!IsValid(DamageCauser))
    {
       return;
    }

    // 플레이어 폰(Pawn)이 유효한지 확인합니다.
    if (!IsValid(PlayerPawn))
    {
       return;
    }
   
    SetVisibility(ESlateVisibility::Visible);

    // 피격 당시 적의 위치를 저장합니다.
    LastPlayerHitEnemyPosition = DamageCauser->GetActorLocation();

    // 적의 유형을 확인하여 근거리/원거리 정보를 결정합니다.
    LastPlayerHitEnemyRange = EEnemyRange::ER_None;
    const ASuraCharacterEnemyBase* Enemy = Cast<ASuraCharacterEnemyBase>(DamageCauser);
    if (IsValid(Enemy))
    {
       FName EnemyType = Enemy->GetEnemyType();
       if (EnemyType == "Rifle" || EnemyType == "Bombard")
       {
          LastPlayerHitEnemyRange = EEnemyRange::ER_Ranged;
       }
       else if (EnemyType == "Melee")
       {
          LastPlayerHitEnemyRange = EEnemyRange::ER_Melee;
       }
    }

    // 적 유형에 따라 위젯의 UI를 업데이트하고, 보이게 합니다.
    UpdateEnemyRangeType(LastPlayerHitEnemyRange);

   UpdateIndicatorDirection();
   
    
    // 기존에 실행 중인 타이머들이 있다면 모두 취소
    GetWorld()->GetTimerManager().ClearTimer(IndicatorUpdateTimerHandle);
    GetWorld()->GetTimerManager().ClearTimer(HideTimerHandle);

    // UpdateIndicatorDirection 함수를 0.01초마다 반복 호출하는 타이머를 시작
    GetWorld()->GetTimerManager().SetTimer(
       IndicatorUpdateTimerHandle,
       this,
       &UDamageIndicatorWidget::UpdateIndicatorDirection,
       0.01f,
       true);

    // 2초 후 UI를 숨기는 타이머를 시작
    GetWorld()->GetTimerManager().SetTimer(
       HideTimerHandle,
       this,
       &UDamageIndicatorWidget::HideIndicator,
       2.0f,
       false);
}

void UDamageIndicatorWidget::UpdateEnemyRangeType(EEnemyRange EnemyRangeType)
{
    // 두 이미지 모두 숨긴 후, 조건에 맞는 이미지 하나만 보이게 합니다.
    if (IsValid(MeleeIndicatorArrow))
    {
       MeleeIndicatorArrow->SetVisibility(ESlateVisibility::Hidden);
    }
    if (IsValid(RangedIndicatorArrow))
    {
       RangedIndicatorArrow->SetVisibility(ESlateVisibility::Hidden);
    }

    if (EnemyRangeType == EEnemyRange::ER_Melee)
    {
       if (IsValid(MeleeIndicatorArrow))
       {
          MeleeIndicatorArrow->SetVisibility(ESlateVisibility::Visible);
       }
    }
    else if (EnemyRangeType == EEnemyRange::ER_Ranged)
    {
       if (IsValid(RangedIndicatorArrow))
       {
          RangedIndicatorArrow->SetVisibility(ESlateVisibility::Visible);
       }
    }
    else
    {
       // 데미지 타입이 None이면 모두 숨김
       if (IsValid(MeleeIndicatorArrow)) MeleeIndicatorArrow->SetVisibility(ESlateVisibility::Hidden);
       if (IsValid(RangedIndicatorArrow)) RangedIndicatorArrow->SetVisibility(ESlateVisibility::Hidden);
    }
}


void UDamageIndicatorWidget::UpdateIndicatorDirection()
{
   if (!IsValid(PlayerPawn))
   {
      return;
   }

   // 1. 플레이어 위치를 기준으로 피격 방향 벡터를 계산합니다.
   const FVector CurrentPlayerLocation = PlayerPawn->GetActorLocation();
   const FVector RelativeDirection = (LastPlayerHitEnemyPosition - CurrentPlayerLocation).GetSafeNormal2D();

   // 2. 플레이어가 바라보는 전방 방향 벡터를 가져옵니다.
   const FVector PlayerForwardVector = PlayerPawn->GetActorForwardVector().GetSafeNormal2D();
    
   // 3. 두 벡터 사이의 각도를 FMath::Atan2 함수를 사용해 계산합니다.
   //    플레이어의 시야를 기준으로 피격 방향의 각도를 얻습니다.
   const float Angle = FMath::Atan2(RelativeDirection.Y, RelativeDirection.X) - FMath::Atan2(PlayerForwardVector.Y, PlayerForwardVector.X);
   
   // 4. 계산된 각도를 이용해 UI가 위치할 새로운 좌표를 계산합니다.
   //    Unreal의 UI는 Y축이 아래쪽으로 증가하므로, FMath::Cos에 마이너스를 붙여야
   //    0도가 화면의 위쪽(북쪽)을 가리키게 됩니다.
   const float NewX = IndicatorRadius * FMath::Sin(Angle);
   const float NewY = -IndicatorRadius * FMath::Cos(Angle);
   
   const FVector2D Translation = FVector2D(NewX, NewY);
   const float AngleInDegrees = FMath::RadiansToDegrees(Angle); // 회전 각도도 필요하니 변환해둡니다.

   // 5. 계산된 위치와 각도를 위젯의 렌더 트랜스폼에 적용합니다.
   //    두 화살표 모두 같은 트랜스폼을 적용합니다.
   if (IsValid(MeleeIndicatorArrow))
   {
      MeleeIndicatorArrow->SetRenderTranslation(Translation);
      MeleeIndicatorArrow->SetRenderTransformAngle(AngleInDegrees);
   }
   if (IsValid(RangedIndicatorArrow))
   {
      RangedIndicatorArrow->SetRenderTranslation(Translation);
      RangedIndicatorArrow->SetRenderTransformAngle(AngleInDegrees);
   }
}


void UDamageIndicatorWidget::HideIndicator()
{
   SetVisibility(ESlateVisibility::Hidden);

   // 두 타이머를 모두 정리하여 깔끔하게 마무리
   GetWorld()->GetTimerManager().ClearTimer(IndicatorUpdateTimerHandle);
   GetWorld()->GetTimerManager().ClearTimer(HideTimerHandle);
}