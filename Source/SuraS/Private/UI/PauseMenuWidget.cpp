// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PauseMenuWidget.h"

#include "ActorComponents/UISystem/ACUIMangerComponent.h"
#include "Components/Button.h"
#include "Instance/SuraCheckpointSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "UI/OptionMenuWidget.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Play)
	{
		Btn_Play->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnResumeClicked);
	}
	if (Btn_Options)
	{
		Btn_Options->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnOptionsClicked);
	}
	if (Btn_Quit)
	{
		Btn_Quit->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnQuitClicked);
	}
	
	APawn* MyPawn = GetOwningPlayerPawn();
	if (MyPawn)
	{
		UIManager = MyPawn->GetComponentByClass<UACUIMangerComponent>();
	}
	SetKeyboardFocus();
}

FReply UPauseMenuWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		if (UIManager)
		{
			UIManager->TogglePauseMenu();
			return FReply::Handled();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("NativeOnKeyDown: UIManager is NULL!"));
		}
		return FReply::Handled(); 
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UPauseMenuWidget::OnResumeClicked()
{
	// UIManager의 TogglePauseMenu를 호출하여 메뉴를 닫습니다.
	if (UIManager)
	{
		UIManager->TogglePauseMenu();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UPauseMenuWidget: UIManager is NULL in OnResumeClicked."));
	}
}

void UPauseMenuWidget::OnOptionsClicked()
{
	if (OptionsMenuInstance && OptionsMenuInstance->IsInViewport())
	{
		OptionsMenuInstance->SetKeyboardFocus(); 
		return;
	}

	APlayerController* PC = GetOwningPlayer();
	
	if (PC && OptionsWidgetClass)
	{
		OptionsMenuInstance = CreateWidget<UUserWidget>(PC, OptionsWidgetClass);
        
		if (OptionsMenuInstance)
		{
			OptionsMenuInstance->AddToViewport();
			
			FInputModeUIOnly InputMode;
            
			// 포커스를 옵션 메뉴로 릴레이
			InputMode.SetWidgetToFocus(OptionsMenuInstance->TakeWidget()); 
			PC->SetInputMode(InputMode);
			PC->bShowMouseCursor = true;
            
			// 옵션 메뉴에 이 PauseMenu를 부모로 등록
			UOptionMenuWidget* OptionsWidget = Cast<UOptionMenuWidget>(OptionsMenuInstance);
			if (OptionsWidget)
			{
				OptionsWidget->SetParentMenu(this); 
			}
		}
	}
	else if (!OptionsWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPauseMenuWidget: OptionsWidgetClass is not set!"));
	}
}

void UPauseMenuWidget::OnQuitClicked()
{
	
	if (MainMenuLevel.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("UPauseMenuWidget: MainMenuLevel이 블루프린트에서 할당되지 않았습니다!"));
		return;
	}
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), MainMenuLevel);
}
