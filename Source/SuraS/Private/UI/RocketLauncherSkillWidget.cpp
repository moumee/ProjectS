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

void URocketLauncherSkillWidget::InitUIDataTable(UDataTable* InTable)
{
    DT_UI = InTable;
}

void URocketLauncherSkillWidget::RocketLauncherZoomAnimation()
{
	//UE_LOG(LogTemp, Warning, TEXT("RocketLauncherZoomAnimation 함수 호출됨!!!"));
	
    if (!IsInViewport())
    {
       AddToViewport();
    }

	if (!SlowScreen)
	{
		UE_LOG(LogTemp, Error, TEXT("SlowScreen이 바인딩되지 않았습니다!"));
	}
    
    // ZoomIn 애니메이션 시작 전 상태 초기화
    SetVisibility(ESlateVisibility::Visible);
    
    if (!DT_UI || !SlowScreen)
    {
       return;
    }

    static const FName RowName(TEXT("RocketLauncherSkill"));
    if (FUIData* Row = DT_UI->FindRow<FUIData>(RowName, TEXT("")))
    {
       ZoomDuration = Row->AnimDuration;
       StartScale = FVector2D(1.5f, 1.5f);
       
       // 애니메이션 시작 상태 초기화
       SlowScreen->SetRenderScale(StartScale);
    	

        // play anim
        //UE_LOG(LogTemp, Warning, TEXT("FadeInAnimation호출됨"));
        PlayAnimation(FadeInAnimation);
        
    }
}

void URocketLauncherSkillWidget::SetSKillManager(UACSkillManager* IM)
{
    SkillManager = IM;
}

void URocketLauncherSkillWidget::RocketLauncherSkillFadeOut()
{
	// play anim
    PlayAnimation(FadeOutAnimation);
}