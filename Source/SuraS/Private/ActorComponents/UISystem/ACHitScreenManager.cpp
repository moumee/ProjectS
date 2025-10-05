// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/UISystem/ACHitScreenManager.h"

#include "NiagaraSystemWidget.h"
#include "NiagaraUIComponent.h"
#include "ActorComponents/UISystem/ACPlayerHealthComponent.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "UI/HitScreenWidget.h"


// Sets default values for this component's properties
UACHitScreenManager::UACHitScreenManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UACHitScreenManager::BeginPlay()
{
	Super::BeginPlay();

	ASuraPawnPlayer* OwnerCharacter = Cast<ASuraPawnPlayer>(GetOwner());
	if (OwnerCharacter)
	{
		UACPlayerHealthComponent* HealthComp = OwnerCharacter->GetHealthComponent();
		if (HealthComp)
		{
			HealthComp->OnHealthChanged.AddDynamic(this, &UACHitScreenManager::OnOwnerHealthChanged);
			OnOwnerHealthChanged(HealthComp->GetCurrentHealth(), HealthComp->GetCurrentHealth(), HealthComp->GetMaxHealth());
		}
	}
	
}

void UACHitScreenManager::OnOwnerHealthChanged(float NewHealth, float OldHealth, float MaxHealth)
{
	if (MaxHealth <= 0) return;


	UpdateHitScreen(NewHealth, OldHealth, MaxHealth);
	
	if (NewHealth < OldHealth && CurrentHitWidget)
	{
		TriggerHitFlash();
	}
}


// Called every frame
void UACHitScreenManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UACHitScreenManager::UpdateHitScreen(float NewHealth, float OldHealth, float MaxHealth)
{
	const float HealthRatio = NewHealth / MaxHealth;

    // 1. 표시할 위젯 클래스 결정 (WBP_Damage_1 또는 2, 또는 없음)
    TSubclassOf<UUserWidget> DesiredWidgetClass = nullptr;
    if (HealthRatio < 1.0f && NewHealth > 0)
    {
        DesiredWidgetClass = (HealthRatio < LowHPThreshold) ? LowHPEffectWidgetClass : HighHPEffectWidgetClass;
    }

    // 2. 위젯 교체가 필요한지 확인 및 실행 
    bool bNeedsWidgetChange = false;
    if ((!CurrentHitWidget && DesiredWidgetClass) || (CurrentHitWidget && !DesiredWidgetClass) || (CurrentHitWidget && DesiredWidgetClass && CurrentHitWidget->GetClass() != DesiredWidgetClass))
    {
        bNeedsWidgetChange = true;
    }

    if (bNeedsWidgetChange)
    {
        if (CurrentHitWidget)
        {
            CurrentHitWidget->RemoveFromParent();
            CurrentHitWidget = nullptr;
            CurrentNiagaraComponent = nullptr;
        }

        if (DesiredWidgetClass)
        {
            APlayerController* PC = GetWorld()->GetFirstPlayerController();
            if (PC)
            {
                CurrentHitWidget = CreateWidget<UHitScreenWidget>(PC, DesiredWidgetClass);
                CurrentHitWidget->AddToViewport();
            	StartBreathingEffect();

                UNiagaraSystemWidget* NiagaraWidget = Cast<UNiagaraSystemWidget>(CurrentHitWidget->GetWidgetFromName(TEXT("NiagaraDamageFX")));
                if (NiagaraWidget)
                {
                    CurrentNiagaraComponent = NiagaraWidget->GetNiagaraComponent();
                }
            }
        }
    }

    // 3. WBP_Damage_1이 활성화된 경우, 내부 나이아가라 에셋을 교체
    if (CurrentHitWidget && CurrentHitWidget->GetClass() == HighHPEffectWidgetClass && CurrentNiagaraComponent)
    {
        // 체력 비율에 따라 목표 나이아가라 에셋을 결정
        TObjectPtr<UNiagaraSystem> DesiredNiagaraSystem = nullptr;
        if (HealthRatio >= 0.8f && HealthRatio < 1.0f)
        {
            DesiredNiagaraSystem = NiagaraAsset_99_80;
        	UE_LOG(LogTemp, Warning, TEXT("99_80나이아가라 적용됨"));
        }
        else if (HealthRatio >= 0.5f)
        {
            DesiredNiagaraSystem = NiagaraAsset_79_50;
        	UE_LOG(LogTemp, Warning, TEXT("79_50나이아가라 적용됨"));
        }
        else if (HealthRatio >= LowHPThreshold)
        {
            DesiredNiagaraSystem = NiagaraAsset_49_30;
        	UE_LOG(LogTemp, Warning, TEXT("49_30나이아가라 적용됨"));
        }
        
        // 현재 할당된 에셋과 목표 에셋이 다를 경우에만 교체하여 불필요한 업데이트를 방지
        if (DesiredNiagaraSystem && CurrentNiagaraComponent->GetAsset() != DesiredNiagaraSystem)
        {
            CurrentNiagaraComponent->SetAsset(DesiredNiagaraSystem);
            CurrentNiagaraComponent->Activate(true); 
        }
    }
}

void UACHitScreenManager::TriggerHitFlash()
{
	if (!CurrentHitWidget || !CurrentHitWidget->Anim_HitFlash)
	{
		return;
	}
	// 1. 혹시 이전에 실행 중이던 타이머가 있다면 초기화
	GetWorld()->GetTimerManager().ClearTimer(HitFlashTimerHandle);

	// 2. HitFlash 애니메이션을 1회 재생
	CurrentHitWidget->PlayAnimation(
		CurrentHitWidget->Anim_HitFlash,
		0.0f,
		1,
		EUMGSequencePlayMode::Forward,
		1.0f,
		false
	);

	// 3. 애니메이션이 끝날 시간에 맞춰 타이머를 설정
	GetWorld()->GetTimerManager().SetTimer(
		HitFlashTimerHandle,                               
		this,                                              
		&UACHitScreenManager::OnHitFlashTimerFinished, 
		HitFlashAnimationDuration,                         
		false                                              
	);
}

void UACHitScreenManager::StartBreathingEffect()
{
	if (!CurrentHitWidget || !CurrentHitWidget->Anim_HitFlash)
	{
		return;
	}

	CurrentHitWidget->PlayAnimation(CurrentHitWidget->Anim_Breathing, 0.0f, 0, EUMGSequencePlayMode::Forward, 1);
}

void UACHitScreenManager::OnHitFlashTimerFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("Hit Flash Timer Finished. Restarting Breathing..."));
	StartBreathingEffect();
}
