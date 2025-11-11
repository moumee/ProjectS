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
		UE_LOG(LogTemp, Log, TEXT("[ACSkillManager] BeginPlay: WeaponSystem->OnSkillWeaponEquipped에 바인딩 성공."));

		AWeapon* ExistingSkillWeapon = WeaponSystem->GetCurrentSkillWeapon();
		if (ExistingSkillWeapon)
		{
			UE_LOG(LogTemp, Warning, TEXT("[ACSkillManager] BeginPlay: 이미 존재하는 스킬 무기(%s)를 발견. 즉시 바인딩을 시도합니다."), *ExistingSkillWeapon->GetName());
			// 이미 무기를 가지고 있으므로 바인딩 함수를 수동으로 호출
			BindWeaponSkillDelegate(ExistingSkillWeapon);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("[ACSkillManager] BeginPlay: 현재 장착된 스킬 무기가 없음. 획득 대기 중..."));
		}
	}
	else if (!WeaponSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ACSkillManager] BeginPlay: WeaponSystem이 nullptr임"));
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
	if (RocketLauncherSkillWidget)
	{
		UE_LOG(LogTemp, Log, TEXT("[ACSkillManager] SetRocketLauncherSkillWidget: RocketLauncherSkillWidget이 성공적으로 설정됨."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ACSkillManager] SetRocketLauncherSkillWidget: RocketLauncherSkillWidget이 nullptr로 설정됨!"));
	}
}

void UACSkillManager::SetUIManager(UACUIMangerComponent* InManager)
{
	UIManager = InManager;
}

void UACSkillManager::BindWeaponSkillDelegate(AWeapon* NewWeapon)
{
	UE_LOG(LogTemp, Warning, TEXT("--- [ACSkillManager] BindWeaponSkillDelegate가 호출됨 ---"));

	if (!NewWeapon)
	{
		UE_LOG(LogTemp, Error, TEXT("[ACSkillManager] NewWeapon이 nullptr입니다. 바인딩 실패."));
		return;
	}

	if (!RocketLauncherSkillWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("[ACSkillManager] RocketLauncherSkillWidget이 nullptr입니다. 바인딩 실패. (SetRocketLauncherSkillWidget이 먼저 호출되었는지 확인하세요)"));
		return;
	}

	if (NewWeapon && RocketLauncherSkillWidget)
	{
		// 스킬 활성화 델리게이트에 함수 바인딩
		NewWeapon->OnRocketLauncherSkillActivated.AddDynamic(
		   RocketLauncherSkillWidget,
		   &URocketLauncherSkillWidget::RocketLauncherZoomAnimation
		);
		UE_LOG(LogTemp, Log, TEXT("[ACSkillManager] '%s'의 OnRocketLauncherSkillActivated 델리게이트에 RocketLauncherZoomAnimation 등록 성공"), *NewWeapon->GetName());

		// 스킬 종료 델리게이트에 함수 바인딩
		NewWeapon->OnRocketLauncherSkillOvered.AddDynamic(
		   RocketLauncherSkillWidget, 
		   &URocketLauncherSkillWidget::RocketLauncherSkillFadeOut
		);
		UE_LOG(LogTemp, Log, TEXT("[ACSkillManager] '%s'의 OnRocketLauncherSkillOvered 델리게이트에 RocketLauncherSkillFadeOut 등록 성공"), *NewWeapon->GetName());
	}
	
}

