// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/WeaponAimUIWidget.h"

#include "InterchangeResult.h"
#include "ActorComponents/WeaponSystem/SuraProjectile.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"

void UWeaponAimUIWidget::NativeConstruct()
{
    Super::NativeConstruct();

    //UE_LOG(LogTemp, Error, TEXT("UWeaponAimUIWidget::NativeConstruct()!!!"));

    // 초기 상태: 모두 가득 차 있음
    if (LeftDashCounter) LeftDashCounter->SetPercent(1.0f);
    if (RightDashCounter) RightDashCounter->SetPercent(1.0f);
}

void UWeaponAimUIWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    UpdateHitIndicator(InDeltaTime);
}

#pragma region Spread
void UWeaponAimUIWidget::ResetAimUISize()
{
    if (OutCircle)
    {
        UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(OutCircle->Slot);
        if (CanvasSlot)
        {
            CanvasSlot->SetSize(DefaultOutCircleSize);
        }
    }
}
void UWeaponAimUIWidget::ApplyAimUISpread(float SpreadValue)
{
    if (OutCircle)
    {
        UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(OutCircle->Slot);
        if (CanvasSlot)
        {
            FVector2D NewSize = { DefaultOutCircleSize.X + SpreadValue * OutCircleSpreadSizeOffset, DefaultOutCircleSize.Y + SpreadValue * OutCircleSpreadSizeOffset };
            CanvasSlot->SetSize(NewSize);
        }
    }
}
#pragma endregion

#pragma region HitIndicator
void UWeaponAimUIWidget::SetUpAimUIDelegateBinding(ASuraProjectile* Projectile)
{
    Projectile->OnHeadShot.BindUObject(this, &UWeaponAimUIWidget::HeadShot);
    Projectile->OnBodyShot.BindUObject(this, &UWeaponAimUIWidget::BodyShot);
}

void UWeaponAimUIWidget::HeadShot()
{
    //UE_LOG(LogTemp, Error, TEXT("UWeaponAimUIWidget::HeadShot()!!!"));

    SetNormalOvelayInvisible();

    bIsIndicatingCriticalHit = true;
    CriticalOverlayFadeOutTimer = 0.f;
    CriticalOverlay->SetRenderOpacity(0.f);
    CriticalOverlay->SetVisibility(ESlateVisibility::Visible);
}

void UWeaponAimUIWidget::BodyShot()
{
    //UE_LOG(LogTemp, Error, TEXT("UWeaponAimUIWidget::BodyShot()!!!"));

    SetCriticalOvelayInvisible();

    bIsIndicatingNormalHit = true;
    NormalOverlayFadeOutTimer = 0.f;
    NormalOverlay->SetRenderOpacity(0.f);
    NormalOverlay->SetVisibility(ESlateVisibility::Visible);
}

void UWeaponAimUIWidget::FadeInCriticalOverlay(float DeltaTime)
{
    float InterpOpacity = FMath::FInterpTo(CurrentCriticalOverlayOpacity, TargetCriticalOverlayOpacity, DeltaTime, CriticalOverlayFadeInSpeed);
    CurrentCriticalOverlayOpacity = InterpOpacity;
    CriticalOverlay->SetRenderOpacity(CurrentCriticalOverlayOpacity);

    CriticalOverlayFadeOutTimer += DeltaTime;
}

void UWeaponAimUIWidget::FadeOutCriticalOverlay(float DeltaTime)
{
    float InterpOpacity = FMath::FInterpTo(CurrentCriticalOverlayOpacity, 0.f, DeltaTime, CriticalOverlayFadeOutSpeed);
    CurrentCriticalOverlayOpacity = InterpOpacity;
    CriticalOverlay->SetRenderOpacity(CurrentCriticalOverlayOpacity);

    if (CurrentCriticalOverlayOpacity < 0.1f)
    {
        //UE_LOG(LogTemp, Error, TEXT("CriticalOverlay Fade Out!!!"));

        bIsIndicatingCriticalHit = false;
        CurrentCriticalOverlayOpacity = 0.f;
        CriticalOverlay->SetRenderOpacity(0.f);
    }
}

void UWeaponAimUIWidget::FadeInNormalOverlay(float DeltaTime)
{
    float InterpOpacity = FMath::FInterpTo(CurrentNormalOverlayOpacity, TargetNormalOverlayOpacity, DeltaTime, NormalOverlayFadeInSpeed);
    CurrentNormalOverlayOpacity = InterpOpacity;
    NormalOverlay->SetRenderOpacity(CurrentNormalOverlayOpacity);

    NormalOverlayFadeOutTimer += DeltaTime;
}

void UWeaponAimUIWidget::FadeOutNormalOverlay(float DeltaTime)
{
    float InterpOpacity = FMath::FInterpTo(CurrentNormalOverlayOpacity, 0.f, DeltaTime, NormalOverlayFadeOutSpeed);
    CurrentNormalOverlayOpacity = InterpOpacity;
    NormalOverlay->SetRenderOpacity(CurrentNormalOverlayOpacity);

    if (CurrentNormalOverlayOpacity < 0.1f)
    {
        //UE_LOG(LogTemp, Error, TEXT("NormalOverlay Fade Out!!!"));

        bIsIndicatingNormalHit = false;
        CurrentNormalOverlayOpacity = 0.f;
        NormalOverlay->SetRenderOpacity(0.f);
    }
}

void UWeaponAimUIWidget::SetCriticalOvelayInvisible()
{
    bIsIndicatingCriticalHit = false;
    CurrentCriticalOverlayOpacity = 0.f;
    CriticalOverlay->SetRenderOpacity(0.f);
}

void UWeaponAimUIWidget::SetNormalOvelayInvisible()
{
    bIsIndicatingNormalHit = false;
    CurrentNormalOverlayOpacity = 0.f;
    NormalOverlay->SetRenderOpacity(0.f);
}

void UWeaponAimUIWidget::UpdateHitIndicator(float DeltaTime)
{
    if (bIsIndicatingCriticalHit)
    {
        if (CriticalOverlayFadeOutStartTime > CriticalOverlayFadeOutTimer)
        {
            FadeInCriticalOverlay(DeltaTime);
        }
        else
        {
            FadeOutCriticalOverlay(DeltaTime);
        }
    }

    if (bIsIndicatingNormalHit)
    {
        if (NormalOverlayFadeOutStartTime > NormalOverlayFadeOutTimer)
        {
            FadeInNormalOverlay(DeltaTime);
        }
        else
        {
            FadeOutNormalOverlay(DeltaTime);
        }
    }
}


#pragma endregion

#pragma region DashCounter
bool UWeaponAimUIWidget::TryUseDash(float CooldownTimePer1Gauge)
{
    float Left = LeftDashCounter ? LeftDashCounter->GetPercent() : 0.f;
    float Right = RightDashCounter ? RightDashCounter->GetPercent() : 0.f;
    float Total = Left + Right;

    if (Total < 1.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Dash failed: insufficient gauge %.2f"), Total);
        return false;
    }

    float LeftToConsume = FMath::Min(Left, 1.0f);
    float RightToConsume = 1.0f - LeftToConsume;

    if (LeftToConsume > 0 && LeftDashCounter)
    {
        AnimateProgress(LeftDashCounter, Left, Left - LeftToConsume, 0.5f);
    }
    if (RightToConsume > 0 && RightDashCounter)
    {
        AnimateProgress(RightDashCounter, Right, Right - RightToConsume, 0.5f);
    }

    // 0.5초 소모 끝나고 → 4.5초 회복
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimerForNextTick([this, CooldownTimePer1Gauge]()
        {
            FTimerHandle Handle;
            GetWorld()->GetTimerManager().SetTimer(Handle, [this, CooldownTimePer1Gauge]()
            {
                StartRecovery(1.0f, CooldownTimePer1Gauge - 0.5f);
            }, 0.5f, false);
        });
    }

    return true;
}



void UWeaponAimUIWidget::StartRecovery(float AmountToRecover, float RecoveryDuration)
{
    float Left = LeftDashCounter ? LeftDashCounter->GetPercent() : 0.f;
    float Right = RightDashCounter ? RightDashCounter->GetPercent() : 0.f;

    // 먼저 Right가 비어 있으면 회복
    if (Right < 1.0f)
    {
        AnimateProgress(RightDashCounter, Right, FMath::Min(1.0f, Right + AmountToRecover), RecoveryDuration);
    }
    else if (Left < 1.0f)
    {
        AnimateProgress(LeftDashCounter, Left, FMath::Min(1.0f, Left + AmountToRecover), RecoveryDuration);
    }
}


void UWeaponAimUIWidget::AnimateProgress(UProgressBar* TargetBar, float From, float To, float Duration)
{
    if (!TargetBar || Duration <= 0.f || From == To) return;

    const float TickInterval = 0.01f; // 프레임당 1회 정도
    float Elapsed = 0.f;
    UWorld* World = GetWorld();
    if (!World) return;

    //  ProgressBar별 고유 타이머 저장
    FTimerHandle& TimerHandle = (TargetBar == LeftDashCounter)
        ? LeftDashTimerHandle : RightDashTimerHandle;

    // 이전 애니메이션 타이머가 있으면 제거
    if (World->GetTimerManager().IsTimerActive(TimerHandle))
    {
        World->GetTimerManager().ClearTimer(TimerHandle);
    }

    // 애니메이션 시작 로그
    UE_LOG(LogTemp, Warning, TEXT("Start AnimateProgress: From=%.2f → To=%.2f (%.2fs) on %s"),
        From, To, Duration, *TargetBar->GetName());

    World->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=, this]() mutable
    {
        Elapsed += TickInterval;

        float Alpha = FMath::Clamp(Elapsed / Duration, 0.f, 1.f);
        float CurrentValue = FMath::Lerp(From, To, Alpha);

        if (TargetBar)
        {
            TargetBar->SetPercent(CurrentValue);
        }

        if (Alpha >= 1.0f)
        {
            World->GetTimerManager().ClearTimer(TimerHandle);
        }

    }), TickInterval, true);
}



#pragma endregion