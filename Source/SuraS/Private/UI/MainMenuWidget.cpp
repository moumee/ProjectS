// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuWidget.h"
#include "Components/Button.h"
#include "Instance/SuraCheckpointSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/SuraSaveGame.h"
#include "UI/OptionMenuWidget.h"

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Btn_Play)
    {
        Btn_Play->OnClicked.AddDynamic(this, &UMainMenuWidget::OnPlayClicked);
    }

    if (Btn_NewGame)
    {
        Btn_NewGame->OnClicked.AddDynamic(this, &UMainMenuWidget::OnNewGameClicked);
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

    if (USuraCheckpointSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USuraCheckpointSubsystem>())
    {
        bool bLevelStartedLoading = false; 

        if (Subsystem->HasSavedCheckpoint())
        {
            if (USuraSaveGame* CurrentSave = Subsystem->GetCurrentSave())
            {
                FName SavedMapName = CurrentSave->MapName;
                
                if (SavedMapName != NAME_None)
                {
                    UGameplayStatics::OpenLevel(GetWorld(), SavedMapName);
                    bLevelStartedLoading = true;
                }
                else
                {
                    // 세이브 파일은 있으나 맵 이름이 비어있는 경우 경고
                    UE_LOG(LogTemp, Warning, TEXT("OnPlayClicked: Save file exists, but SavedMapName is None."));
                }
            }   
        }

        // 레벨 로드가 시작되지 않았다면 (저장 파일이 없거나, 맵 이름이 유효하지 않은 경우)
        if (!bLevelStartedLoading)
        {
            if (!LevelToLoad.IsNull())
            {
                // NewGame과 동일하게 LevelToLoad에 지정된 기본 레벨 open
                UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), LevelToLoad);
                bLevelStartedLoading = true;
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("OnPlayClicked: Cannot load level. SavedMapName is invalid AND LevelToLoad is not set."));
            }
        }
        
        // 레벨 로딩이 시작된 경우에만 입력 모드를 변경
        if (bLevelStartedLoading)
        {
            APlayerController* PC = GetOwningPlayer();
            if (PC)
            {
                PC->SetInputMode(FInputModeGameOnly());
                PC->bShowMouseCursor = false;
            }
        }
    }
}

void UMainMenuWidget::OnNewGameClicked()
{
    USuraCheckpointSubsystem* CheckpointSubsystem = GetGameInstance()->GetSubsystem<USuraCheckpointSubsystem>();
    
    if (CheckpointSubsystem)
    {
        CheckpointSubsystem->ClearSavedCheckpoint(); //
        UE_LOG(LogTemp, Log, TEXT("New Game Started: Checkpoint data cleared."));
    }
    
    if (!LevelToLoad.IsNull())
    {
        UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), LevelToLoad);
        
        APlayerController* PC = GetOwningPlayer();
        if (PC)
        {
            PC->SetInputMode(FInputModeGameOnly());
            PC->bShowMouseCursor = false;
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("MainMenuWidget::OnNewGameClicked - 'LevelToLoad'가 설정되지 않았습니다."));
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