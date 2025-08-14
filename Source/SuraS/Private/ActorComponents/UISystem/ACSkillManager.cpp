// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/UISystem/ACSkillManager.h"

#include "ActorComponents/WeaponSystem/ACWeapon.h"
#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h"
#include "UI/RocketLauncherSkillWidget.h"

// Sets default values for this component's properties
UACSkillManager::UACSkillManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UACSkillManager::BeginPlay()
{
	Super::BeginPlay();

	UWeaponSystemComponent* WeaponSystem = UIManager->GetWeaponSystemComponent();
	if (WeaponSystem)
	{
		// 스킬 무기 장착 델리게이트에 바인딩
		WeaponSystem->OnSkillWeaponEquipped.AddDynamic(this, &UACSkillManager::BindWeaponSkillDelegate);
	}
	else if (!WeaponSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponSystem이 nullptr임"));
	}
	
}


// Called every frame
void UACSkillManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UACSkillManager::SetRocketLauncherSkillWidget(URocketLauncherSkillWidget* IW)
{
	RocketLauncherSkillWidget = IW;
}

void UACSkillManager::SetUIManager(UACUIMangerComponent* InManager)
{
	UIManager = InManager;
}

void UACSkillManager::BindWeaponSkillDelegate(AWeapon* NewWeapon)
{

	if (NewWeapon && RocketLauncherSkillWidget)
	{
		// 스킬 활성화 델리게이트에 함수 바인딩
		NewWeapon->OnRocketLauncherSkillActivated.AddDynamic(
		   RocketLauncherSkillWidget,
		   &URocketLauncherSkillWidget::RocketLauncherZoomAnimation
		);
		//UE_LOG(LogTemp, Warning, TEXT("RocketLauncherZoomAnimation 델리게이트로서 등록"));

		// 스킬 종료 델리게이트에 함수 바인딩
		NewWeapon->OnRocketLauncherSkillOvered.AddDynamic(
		   RocketLauncherSkillWidget, 
		   &URocketLauncherSkillWidget::RocketLauncherSkillFadeOut
		);
		//UE_LOG(LogTemp, Warning, TEXT("RocketLauncherSkillOvered 델리게이트로서 등록"));
	}
	
}

