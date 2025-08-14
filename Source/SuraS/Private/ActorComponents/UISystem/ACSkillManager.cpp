// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/UISystem/ACSkillManager.h"

#include "ActorComponents/WeaponSystem/ACWeapon.h"
#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
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
		//  BeginPlay 시점에 처음 장착된 무기에 델리게이트 연결
		//UE_LOG(LogTemp, Warning, TEXT("BindWeaponSKillDelegate 함수 호출됨"));
		BindWeaponSkillDelegate();
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

void UACSkillManager::BindWeaponSkillDelegate()
{
	ASuraPawnPlayer* PlayerPawn = Cast<ASuraPawnPlayer>(GetOwner());
	if (PlayerPawn && RocketLauncherSkillWidget)
	{
		//UE_LOG(LogTemp, Warning, TEXT("RocketLauncherSkillWidget: %s"), *GetNameSafe(RocketLauncherSkillWidget));

		UWeaponSystemComponent* WeaponSystem = PlayerPawn->GetWeaponSystemComponent();
		if (WeaponSystem)
		{
			AWeapon* CurrentWeapon = WeaponSystem->GetCurrentWeapon();
			if (CurrentWeapon)
			{
				// 델리게이트에 함수 바인딩
				// 이 코드를 통해 AWeapon의 OnRocketLauncherSkillActivated가 호출될 때마다 URocketLauncherSkillWidget의
				// RocketLauncherZoomAnimation 함수가 실행됩니다.
				CurrentWeapon->OnRocketLauncherSkillActivated.AddDynamic(
				   RocketLauncherSkillWidget,
				   &URocketLauncherSkillWidget::RocketLauncherZoomAnimation
				);
				//UE_LOG(LogTemp, Warning, TEXT("RocketLauncherZoomAnimation 델리게이트로서 등록"));

				CurrentWeapon->OnRocketLauncherSkillOvered.AddDynamic(RocketLauncherSkillWidget, &URocketLauncherSkillWidget::RocketLauncherSkillFadeOut);
				//UE_LOG(LogTemp, Warning, TEXT("RocketLauncherSkillCanceled 델리게이트로서 등록"));
			}
		}
	}
}

