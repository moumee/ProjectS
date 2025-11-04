// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "UI/OptionMenuWidget.h"

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Btn_Play)
    {
        Btn_Play->OnClicked.AddDynamic(this, &UMainMenuWidget::OnPlayClicked);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Btn_Play is NULL in UMainMenuWidget!"));
    }

    if (Btn_Options)
    {
        Btn_Options->OnClicked.AddDynamic(this, &UMainMenuWidget::OnOptionsClicked);
    }

    if (Btn_Quit)
    {
        Btn_Quit->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClicked);
    }
    
    SetKeyboardFocus();
}

void UMainMenuWidget::OnPlayClicked()
{
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("EnemiesDev"));
    
    APlayerController* PC = GetOwningPlayer();
    if (PC)
    {
        PC->SetInputMode(FInputModeGameOnly());
        PC->bShowMouseCursor = false;
    }
}

void UMainMenuWidget::OnOptionsClicked()
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
            InputMode.SetWidgetToFocus(OptionsMenuInstance->TakeWidget()); 
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = true;
            
            UOptionMenuWidget* OptionsWidget = Cast<UOptionMenuWidget>(OptionsMenuInstance);
            if (OptionsWidget)
            {
                OptionsWidget->SetParentMenu(this); 
            }
        }
    }
    else if (!OptionsWidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("OptionsWidgetClass is not set on MainMenuWidget Blueprint!"));
    }
}

void UMainMenuWidget::OnQuitClicked()
{
    APlayerController* PC = GetOwningPlayer();
    if (PC)
    {
        PC->ConsoleCommand(TEXT("quit"));
    }
}