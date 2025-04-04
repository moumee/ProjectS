// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/UISystem/ACUIMangerComponent.h"
#include "EnhancedInputComponent.h"
#include "ActorComponents/UISystem/ACInventoryManager.h"
#include "UI/InventoryWidget.h"



// Sets default values for this component's properties
UACUIMangerComponent::UACUIMangerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	
	InitializeMangers();
}


// Called when the game starts
void UACUIMangerComponent::BeginPlay()
{
	Super::BeginPlay();
	SetupInput();
	
	InitializeWidgets();
}

void UACUIMangerComponent::SetupInput()
{
	if (APlayerController* PC = Cast<APlayerController>(GetOwner()->GetInstigatorController()))
	{
		if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PC->InputComponent))
		{
			EnhancedInput->BindAction(OpenInventoryAction, ETriggerEvent::Started, this, &UACUIMangerComponent::OpenUI, EUIType::Inventory);
			//EnhancedInput->BindAction(OpenPauseMenuAction, ETriggerEvent::Started, this, &UACBaseUIComponent::ToggleUI, EUIType::PauseMenu);
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

		//case EUIType::HUD:
			// HUD 위젯 초기화 및 매니저 연결
				//break;

		default:
			break;
		}
	}
}

void UACUIMangerComponent::InitializeMangers()
{
	// 인벤토리 매니저 생성 및 등록 (생성자에서 호출되므로 문제 없음)
	InventoryManager = CreateDefaultSubobject<UACInventoryManager>(TEXT("InventoryManager"));

	// UIComponentManager에 접근하기 위해 this를 파라미터로 전달
	InventoryManager->SetUIManager(this);

	
	// HUDManager, PauseMenuManager 등도 여기에 추가

}