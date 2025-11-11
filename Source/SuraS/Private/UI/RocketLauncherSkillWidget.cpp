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
	UE_LOG(LogTemp, Warning, TEXT("--- [RocketLauncherSkillWidget] RocketLauncherZoomAnimation 함수 호출됨!!! ---"));
	
    if (!IsInViewport())
    {
       AddToViewport();
    	UE_LOG(LogTemp, Log, TEXT("[RocketLauncherSkillWidget] 위젯을 뷰포트에 추가합니다."));
    }

	if (!SlowScreen)
	{
		UE_LOG(LogTemp, Error, TEXT("[RocketLauncherSkillWidget] SlowScreen이 바인딩되지 않았습니다! (블루프린트에서 Is Variable 체크 확인)"));
	}
    
    // ZoomIn 애니메이션 시작 전 상태 초기화
    SetVisibility(ESlateVisibility::Visible);
    
    if (!DT_UI || !SlowScreen)
    {
    	UE_LOG(LogTemp, Error, TEXT("[RocketLauncherSkillWidget] DT_UI가 null이거나 SlowScreen이 바인딩되지 않아서 애니메이션을 실행할 수 없습니다."));
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
	UE_LOG(LogTemp, Warning, TEXT("--- [RocketLauncherSkillWidget] RocketLauncherSkillFadeOut 함수 호출됨 ---"));
	// play anim
    PlayAnimation(FadeOutAnimation);
}