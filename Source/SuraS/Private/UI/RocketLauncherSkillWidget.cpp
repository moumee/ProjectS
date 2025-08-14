#include "UI/RocketLauncherSkillWidget.h"
#include "Components/Image.h"
#include "TimerManager.h"
#include "UI/UIData.h"
#include "HAL/PlatformTime.h"
#include "Engine/DataTable.h"

// Fill out your copyright notice in the Description page of Project Settings.
#include "TimerManager.h"


void URocketLauncherSkillWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void URocketLauncherSkillWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    // ZoomIn 애니메이션
    if (bIsZooming && SlowScreen)
    {
       ElapsedTime += FApp::GetDeltaTime();
       float Alpha = FMath::Clamp(ElapsedTime / ZoomDuration, 0.f, 1.f);
       FVector2D CurrentScale = FMath::Lerp(StartScale, TargetScale, Alpha);
       SlowScreen->SetRenderScale(CurrentScale);
        
       if (Alpha >= 1.f)
       {
          bIsZooming = false;
       }
    }
    // FadeOut 애니메이션
    else if (bIsFadingOut && SlowScreen)
    {
       FadeElapsedTime += FApp::GetDeltaTime();
       float Alpha = FMath::Clamp(FadeElapsedTime / FadeDuration, 0.f, 1.f);
       
       float CurrentOpacity = FMath::Lerp(1.0f, 0.0f, Alpha);
       FVector2D CurrentScale = FMath::Lerp(FadeStartScale, FadeTargetScale, Alpha);

       SlowScreen->SetRenderOpacity(CurrentOpacity);
       SlowScreen->SetRenderScale(CurrentScale);
       
       if (Alpha >= 1.0f)
       {
          bIsFadingOut = false;
          SetVisibility(ESlateVisibility::Hidden);
       }
    }
}

void URocketLauncherSkillWidget::InitUIDataTable(UDataTable* InTable)
{
    DT_UI = InTable;
}

void URocketLauncherSkillWidget::RocketLauncherZoomAnimation()
{
    if (!IsInViewport())
    {
       AddToViewport();
    }
    
    // ZoomIn 애니메이션 시작 전 상태 초기화
    bIsFadingOut = false;
    SetVisibility(ESlateVisibility::Visible);
    
    if (!DT_UI || !SlowScreen)
    {
       return;
    }

    static const FName RowName(TEXT("RocketLauncherSkill"));
    if (FUIData* Row = DT_UI->FindRow<FUIData>(RowName, TEXT("")))
    {
       ZoomDuration = Row->AnimDuration;
       ElapsedTime = 0.f;
       StartScale = FVector2D(1.5f, 1.5f);
       TargetScale = FVector2D(1.f, 1.f);
       
       // 애니메이션 시작 상태 초기화
       SlowScreen->SetRenderOpacity(1.0f);
       SlowScreen->SetRenderScale(StartScale);
       
       bIsZooming = true;
    }
}

void URocketLauncherSkillWidget::SetSKillManager(UACSkillManager* IM)
{
    SkillManager = IM;
}

void URocketLauncherSkillWidget::RocketLauncherSkillFadeOut()
{
    // FadeOut 애니메이션 시작 전 상태 초기화
    bIsZooming = false;
    bIsFadingOut = true;
    FadeElapsedTime = 0.0f;

	FadeStartScale = FVector2D(1.0f, 1.0f);
    
    SetVisibility(ESlateVisibility::Visible);
}