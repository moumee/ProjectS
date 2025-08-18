// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/UISystem/ACUIMangerComponent.h"
#include "EnhancedInputComponent.h"
#include "ActorComponents/UISystem/ACInventoryManager.h"
#include "ActorComponents/UISystem/ACKillLogManager.h"
#include "ActorComponents/UISystem/ACPlayerHudManager.h"
#include "ActorComponents/UISystem/ACSkillManager.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "UI/InventoryWidget.h"
#include "UI/KillLogWidget.h"
#include "UI/PlayerHUD.h"
#include "UI/RocketLauncherSkillWidget.h"


// Sets default values for this component's properties
UACUIMangerComponent::UACUIMangerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	
	InitializeManagers();
}


// Called when the game starts
void UACUIMangerComponent::BeginPlay()
{
	Super::BeginPlay();
	SetupInput();
	
	InitializeWidgets();
	
	ASuraPawnPlayer* SuraPawnPlayer = Cast<ASuraPawnPlayer>(GetOwner());  // GetOwningPlayerPawn()은 ActorComponent에선 사용 불가
	if (SuraPawnPlayer)
	{
		WeaponSystemComponent = SuraPawnPlayer->GetWeaponSystemComponent();
	}
}

void UACUIMangerComponent::SetupInput()
{
	if (APlayerController* PC = Cast<APlayerController>(GetOwner()->GetInstigatorController()))
	{
		if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PC->InputComponent))
		{
			EnhancedInput->BindAction(OpenInventoryAction, ETriggerEvent::Started, this, &UACUIMangerComponent::OpenUI, EUIType::Inventory);
		}
	}
}

void UACUIMangerComponent::OpenUI(EUIType UIType)
{
	
	UBaseUIWidget* TargetWidget = GetWidget(UIType);
	if (!TargetWidget) return; // 위젯 생성에 실패한 경우 처리

	if (!TargetWidget->IsInViewport())
	{
		TargetWidget->OpenUI();
	}
}

UBaseUIWidget* UACUIMangerComponent::GetWidget(EUIType UIType)
{
	if (UIWidgets.Contains(UIType))
	{
		return UIWidgets[UIType];
	}

	UE_LOG(LogTemp, Warning, TEXT("Widget not initialized!"));
	return nullptr;
}

void UACUIMangerComponent::InitializeWidgets()
{
	for (const auto& Elem : UIWidgetClasses)
	{
		EUIType UIType = Elem.Key;
		TSubclassOf<UBaseUIWidget> WidgetClass = Elem.Value;

		if (!WidgetClass) continue;

		UBaseUIWidget* NewWidget = CreateWidget<UBaseUIWidget>(GetWorld(), WidgetClass);
		if (!NewWidget) continue;

		UIWidgets.Add(UIType, NewWidget);

		// 위젯 타입별로 매니저 연결
		switch (UIType)
		{
		case EUIType::Inventory:
			{
				if (UInventoryWidget* IW = Cast<UInventoryWidget>(NewWidget))
				{
					IW->SetInventoryManager(InventoryManager); // InventoryWidget에 InventoryManager를 할당
					InventoryManager->SetInventoryWidget(IW);  // InventoryManager에 InventoryWidget을 할당
				}
				break;
			}

		case EUIType::KillLog:
			{
				if (UKillLogWidget* KLW = Cast<UKillLogWidget>(NewWidget))
				{
					KillLogManager->SetKillLogWidget(KLW);
					KLW->SetKillLogManager(KillLogManager);
					KLW->AddToViewport(); // ✅ 반드시 필요

					//UE_LOG(LogTemp, Warning, TEXT("✔ KillLogWidget Viewport에 추가됨"));
				}
				break;
			}

		case EUIType::PlayerHUD:
			{
				if (UPlayerHUD* PW = Cast<UPlayerHUD>(NewWidget))
				{
					PlayerHUDManager->SetPlayerHUDWidget(PW);
					PW->SetPlayerHUDManager(PlayerHUDManager);
					PW->AddToViewport(); // ✅ 반드시 필요

					//UE_LOG(LogTemp, Warning, TEXT("✔ KillLogWidget Viewport에 추가됨"));
				}
				break;
			}
			
		case EUIType::Skill:
			{
				if (URocketLauncherSkillWidget* RLW = Cast<URocketLauncherSkillWidget>(NewWidget))
				{
					SkillManager->SetRocketLauncherSkillWidget(RLW);
					RLW->SetSKillManager(SkillManager);
					RLW->InitUIDataTable(DTUISetting);
					//RLW->AddToViewport(); //스킬 사용 시점에 AddToViewport해야함.
					

					//UE_LOG(LogTemp, Warning, TEXT("✔ RocketLauncherWidget Viewport에 추가됨"));
				}
								

				default:
					break;
			}
		}
	}
}

void UACUIMangerComponent::InitializeManagers()
{
	// 인벤토리 매니저 생성 및 등록 (생성자에서 호출되므로 문제 없음)
	InventoryManager = CreateDefaultSubobject<UACInventoryManager>(TEXT("InventoryManager"));
	// UIComponentManager에 접근하기 위해 this를 파라미터로 전달
	InventoryManager->SetUIManager(this);

	KillLogManager = CreateDefaultSubobject<UACKillLogManager>(TEXT("KillLog"));
	KillLogManager->SetUIManager(this);
	
	// HUDManager, PauseMenuManager 등도 여기에 추가
	PlayerHUDManager = CreateDefaultSubobject<UACPlayerHUDManager>(TEXT("PlayerHUD"));
	PlayerHUDManager->SetUIManager(this);

	SkillManager = CreateDefaultSubobject<UACSkillManager>(TEXT("Skill"));
	SkillManager->SetUIManager(this);
	
}

void UACUIMangerComponent::TestKillLog()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("🔹 IA_KILLLOG 입력 감지됨!"));
	
	if (!KillLogManager) return;

	KillLogManager->AddKillLog(TEXT("Player"), TEXT("Enemy"));
}