// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DamageIndicatorWidget.h"

#include "Animation/WidgetAnimation.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Components/Image.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"

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

void UDamageIndicatorWidget::OnFadeOutAnimationFinished()
{
   SetVisibility(ESlateVisibility::Hidden);
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

   //플레이어 위치를 기준으로 피격 방향 벡터를 계산
   const FVector CurrentPlayerLocation = PlayerPawn->GetActorLocation();
   const FVector RelativeDirection = (LastPlayerHitEnemyPosition - CurrentPlayerLocation).GetSafeNormal2D();

   // 플레이어가 바라보는 전방 방향 벡터를 가져옴
   const FVector PlayerForwardVector = PlayerPawn->GetActorForwardVector().GetSafeNormal2D();
    
   // 두 벡터 사이의 각도를 FMath::Atan2 함수를 사용해 계산
   // 플레이어의 시야를 기준으로 피격 방향의 각도 획득
   const float Angle = FMath::Atan2(RelativeDirection.Y, RelativeDirection.X) - FMath::Atan2(PlayerForwardVector.Y, PlayerForwardVector.X);
   
   // 계산된 각도를 이용해 UI가 위치할 새로운 좌표를 계산
   //  Unreal의 UI는 Y축이 아래쪽으로 증가하므로, FMath::Cos에 마이너스를 붙임
   //  0도가 화면의 위쪽(북쪽)을 가리킴
   const float NewX = IndicatorRadius * FMath::Sin(Angle);
   const float NewY = -IndicatorRadius * FMath::Cos(Angle);
   
   const FVector2D Translation = FVector2D(NewX, NewY);
   const float AngleInDegrees = FMath::RadiansToDegrees(Angle); 

   // 계산된 위치와 각도를 위젯의 렌더 트랜스폼에 적용
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
   if (IsValid(FadeOut))
   {
      //  애니메이션이 끝났을 때 호출될 델리게이트를 바인딩
      FWidgetAnimationDynamicEvent AnimationDelegate;
      AnimationDelegate.BindDynamic(this, &UDamageIndicatorWidget::OnFadeOutAnimationFinished);
        
      BindToAnimationFinished(FadeOut, AnimationDelegate);

      // 페이드아웃 애니메이션을 재생
      PlayAnimation(FadeOut);
   }
   
   GetWorld()->GetTimerManager().ClearTimer(IndicatorUpdateTimerHandle);
   GetWorld()->GetTimerManager().ClearTimer(HideTimerHandle);
}

void UDamageIndicatorWidget::InitializeIndicator(AActor* DamageCauser)
{
   //  동일한 몬스터 피격인지 확인
    if (LastDamageCauser.IsValid() && LastDamageCauser.Get() == DamageCauser)
    {
        // 타이머만 재설정하고 애니메이션을 다시 시작
        GetWorld()->GetTimerManager().ClearTimer(IndicatorUpdateTimerHandle);
        GetWorld()->GetTimerManager().ClearTimer(HideTimerHandle);
        
        GetWorld()->GetTimerManager().SetTimer(
           IndicatorUpdateTimerHandle,
           this,
           &UDamageIndicatorWidget::UpdateIndicatorDirection,
           0.01f,
           true);

        GetWorld()->GetTimerManager().SetTimer(
           HideTimerHandle,
           this,
           &UDamageIndicatorWidget::HideIndicator,
           IndicatorDuration,
           false);

        SetVisibility(ESlateVisibility::Visible);
        PlayAnimation(FadeIn);
       
        return;
    }
   
    if (!IsValid(DamageCauser))
    {
       return;
    }

    if (!IsValid(PlayerPawn))
    {
       APawn* OwningPawn = GetOwningPlayerPawn();
       if (OwningPawn)
       {
          PlayerPawn = Cast<ASuraPawnPlayer>(OwningPawn);
       }
       if (!IsValid(PlayerPawn))
       {
          return;
       }
    }
    
    //  애니메이션을 재생하기 전에 위젯을 보이게 함
    SetVisibility(ESlateVisibility::Visible);

    // 피격 당시 적의 위치를 저장하고 적 유형을 결정
    LastPlayerHitEnemyPosition = DamageCauser->GetActorLocation();
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
    
    // UI 업데이트 및 초기 위치 설정
    UpdateEnemyRangeType(LastPlayerHitEnemyRange);
    UpdateIndicatorDirection();
    
    // 타이머를 새로 설정
    GetWorld()->GetTimerManager().ClearTimer(IndicatorUpdateTimerHandle);
    GetWorld()->GetTimerManager().ClearTimer(HideTimerHandle);
    
    GetWorld()->GetTimerManager().SetTimer(
       IndicatorUpdateTimerHandle,
       this,
       &UDamageIndicatorWidget::UpdateIndicatorDirection,
       0.01f,
       true);

    GetWorld()->GetTimerManager().SetTimer(
       HideTimerHandle,
       this,
       &UDamageIndicatorWidget::HideIndicator,
       IndicatorDuration,
       false);

    //  LastDamageCauser를 업데이트
    LastDamageCauser = DamageCauser;
   
    PlayAnimation(FadeIn);
}
