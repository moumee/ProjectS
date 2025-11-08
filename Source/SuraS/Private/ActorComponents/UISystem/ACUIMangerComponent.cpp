// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/UISystem/ACUIMangerComponent.h"
#include "EnhancedInputComponent.h"
#include "ActorComponents/UISystem/ACKillLogManager.h"
#include "ActorComponents/UISystem/ACPlayerHudManager.h"
#include "ActorComponents/UISystem/ACSkillManager.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "UI/DamageIndicatorWidget.h"
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

	
	// 매니저 생성 및 등록
	//InventoryManager = CreateDefaultSubobject<UACInventoryManager>(TEXT("InventoryManager"));
	KillLogManager = CreateDefaultSubobject<UACKillLogManager>(TEXT("KillLog"));
	PlayerHUDManager = CreateDefaultSubobject<UACPlayerHUDManager>(TEXT("PlayerHUD"));
	SkillManager = CreateDefaultSubobject<UACSkillManager>(TEXT("Skill"));
}


// Called when the game starts
void UACUIMangerComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeManagers();
	InitializeWidgets();
	SetupInput();
	
	ASuraPawnPlayer* SuraPawnPlayer = Cast<ASuraPawnPlayer>(GetOwner());  // GetOwningPlayerPawn()은 ActorComponent에선 사용 불가
	if (SuraPawnPlayer)
	{
		WeaponSystemComponent = SuraPawnPlayer->GetWeaponSystemComponent();
	}

	// create damage indicator widget pool
	for (int32 i = 0; i < PoolSize; i++)
	{
		UDamageIndicatorWidget* NewIndicator = CreateWidget<UDamageIndicatorWidget>(GetWorld(), DamageIndicatorWidgetClass);
		if (NewIndicator)
		{
			DamageIndicatorPool.Add(NewIndicator);
			NewIndicator->AddToViewport(); //  풀 생성 시 한 번만 추가
			NewIndicator->SetVisibility(ESlateVisibility::Hidden); // 초기 상태를 Hidden으로 설정
		}
	}

}

void UACUIMangerComponent::SetupInput()
{
	if (APlayerController* PC = Cast<APlayerController>(GetOwner()->GetInstigatorController()))
	{
		if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PC->InputComponent))
		{
			//EnhancedInput->BindAction(OpenInventoryAction, ETriggerEvent::Started, this, &UACUIMangerComponent::OpenUI, EUIType::Inventory);
			EnhancedInput->BindAction(OpenPauseMenuAction, ETriggerEvent::Started, this, &UACUIMangerComponent::TogglePauseMenu);
			// ETriggerEvent::Started -> 키가 눌렸을 때
			EnhancedInput->BindAction(ShowTabMenuAction, ETriggerEvent::Started, this, &UACUIMangerComponent::OnShowTabMenuStarted);
			// ETriggerEvent::Completed -> 키가 떼어졌을 때
			EnhancedInput->BindAction(ShowTabMenuAction, ETriggerEvent::Completed, this, &UACUIMangerComponent::OnShowTabMenuCompleted);
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
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController not valid in InitializeWidgets."));
		return;
	}
	
	for (const auto& Elem : UIWidgetClasses)
	{
		EUIType UIType = Elem.Key;
		TSubclassOf<UBaseUIWidget> WidgetClass = Elem.Value;

		if (!WidgetClass) continue;

		UBaseUIWidget* NewWidget = CreateWidget<UBaseUIWidget>(GetWorld(), WidgetClass);
		if (!NewWidget) continue;
		if (!IsValid(NewWidget)) //  생성 실패 여부 확인
		{
			UE_LOG(LogTemp, Error, TEXT("InitializeWidgets: NewWidget 생성 실패! UIType: %s"), *UEnum::GetValueAsString(UIType));
			continue;
		}


		UIWidgets.Add(UIType, NewWidget);

		// 위젯 타입별로 매니저 연결
		switch (UIType)
		{
		// case EUIType::Inventory:
		// 	{
		// 		if (UInventoryWidget* IW = Cast<UInventoryWidget>(NewWidget))
		// 		{
		// 			IW->SetInventoryManager(InventoryManager); // InventoryWidget에 InventoryManager를 할당
		// 			InventoryManager->SetInventoryWidget(IW);  // InventoryManager에 InventoryWidget을 할당
		// 		}
		// 		break;
		// 	}

		case EUIType::KillLog:
			{
				if (UKillLogWidget* KLW = Cast<UKillLogWidget>(NewWidget))
				{
					KillLogManager->SetKillLogWidget(KLW);
					KLW->SetKillLogManager(KillLogManager);
					KLW->AddToViewport(); 

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
					PW->AddToViewport(-1);

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
					//RLW->AddToViewport(); //스킬 사용 시점에 AddToViewport해야함;

					//UE_LOG(LogTemp, Warning, TEXT("✔ RocketLauncherWidget Viewport에 추가됨"));
				}
				break;
			}
			
		default:
			break;
		}
	}
}

void UACUIMangerComponent::InitializeManagers()
{
	//InventoryManager->SetUIManager(this);
	KillLogManager->SetUIManager(this);
	PlayerHUDManager->SetUIManager(this);
	SkillManager->SetUIManager(this);
}

void UACUIMangerComponent::ShowDamageIndicator(AActor* DamageCauser)
{
	UDamageIndicatorWidget* Indicator = GetAvailableDamageIndicatorFromPool();
	if (Indicator)
	{
		Indicator->InitializeIndicator(DamageCauser);
	}
}

void UACUIMangerComponent::OnShowTabMenuStarted(const FInputActionValue& Value)
{
	// 위젯 클래스가 유효하고, 아직 위젯이 생성되지 않았다면
	if (TabMenuWidgetClass && !TabMenuWidgetInstance)
	{
		// 1. 이 컴포넌트의 소유자(Owner)를 가져옵니다.
		//    (이 컴포넌트가 플레이어 캐릭터에 붙어있다고 가정)
		APawn* OwnerPawn = Cast<APawn>(GetOwner());
		if (!OwnerPawn)
		{
			// 소유자가 Pawn이 아니면 컨트롤러를 가져올 수 없습니다.
			return; 
		}

		// 2. 소유자인 Pawn에서 컨트롤러를 가져옵니다.
		APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController());
		if (!PC) 
		{
			// 컨트롤러가 유효하지 않으면(예: AI에 의해 조종되는 Pawn) 중단합니다.
			return; 
		}

		// 이제 PC 변수가 유효하므로 위젯을 생성할 수 있습니다.
		TabMenuWidgetInstance = CreateWidget<UUserWidget>(PC, TabMenuWidgetClass);

		if (TabMenuWidgetInstance)
		{
			// 뷰포트에 추가
			TabMenuWidgetInstance->AddToViewport();
		}
	}
}

void UACUIMangerComponent::OnShowTabMenuCompleted(const FInputActionValue& Value)
{
	// 위젯 인스턴스가 유효하다면 (즉, 화면에 떠 있다면)
	if (TabMenuWidgetInstance)
	{
		TabMenuWidgetInstance->RemoveFromParent();
		TabMenuWidgetInstance = nullptr; // 참조 제거

		// *** 중요 ***
		// 여기서도 입력 모드를 건드리지 않습니다.
	}
}

UDamageIndicatorWidget* UACUIMangerComponent::GetAvailableDamageIndicatorFromPool()
{
	for (UDamageIndicatorWidget* Indicator : DamageIndicatorPool)
	{
		// return invisible widget
		if (Indicator && !Indicator->IsVisible())
		{
			return Indicator;
		}
	}
	return nullptr;
}

void UACUIMangerComponent::TogglePauseMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("TogglePauseMenu 함수가 호출되었습니다!"));
	
	
	APlayerController* PC = Cast<APlayerController>(GetOwner()->GetInstigatorController());
	if (!PC || !GetWorld()) return;

	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		// --- 닫기 로직 ---
		if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
		{
			PauseMenuInstance->RemoveFromParent(); 
			PauseMenuInstance = nullptr; 
		}
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(false);
	}
	else
	{
		// --- 열기 로직 ---
		if (PauseMenuWidgetClass)
		{
			PauseMenuInstance = CreateWidget<UUserWidget>(PC, PauseMenuWidgetClass);

			if (PauseMenuInstance)
			{
				PauseMenuInstance->AddToViewport(); 
				UGameplayStatics::SetGamePaused(GetWorld(), true);
				
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(PauseMenuInstance->TakeWidget());
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				
				PC->SetInputMode(FInputModeUIOnly());
				PC->SetShowMouseCursor(true);
				
			}
		}
	}
}
