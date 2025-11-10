// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OptionMenuWidget.h"

#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "UI/CustomGameInstance.h"

void UOptionMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// GameInstance 참조 저장
	GameInstanceRef = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GameInstanceRef)
	{
		UE_LOG(LogTemp, Error, TEXT("UOptionMenuWidget: GameInstanceRef is NULL!"));
	}

	// --- 버튼 델리게이트 바인딩 ---
	if (Btn_Close)
	{
		Btn_Close->OnClicked.AddDynamic(this, &UOptionMenuWidget::OnCloseClicked);
	}
	if (Btn_Apply)
	{
		Btn_Apply->OnClicked.AddDynamic(this, &UOptionMenuWidget::OnApplyClicked);
    }

	// --- 슬라이더 바인딩 및 초기화 ---
	if (SensitivitySlider)
	{
		// 슬라이더 값이 바뀔 때마다 OnSliderValueChanged 함수 호출
		SensitivitySlider->OnValueChanged.AddDynamic(this, &UOptionMenuWidget::OnSliderValueChanged);

		SensitivitySlider->OnMouseCaptureEnd.AddDynamic(this, &UOptionMenuWidget::OnSliderCaptureEnded);

		if (GameInstanceRef)
		{
			float InitialSensitivity = GameInstanceRef->MouseSensitivity;
			SensitivitySlider->SetValue(InitialSensitivity);
			
			UpdateSensitivityText(InitialSensitivity);
		}
	}
}

FReply UOptionMenuWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FKey Key = InKeyEvent.GetKey();
	
	if (Key == EKeys::Enter)
	{
		OnApplyClicked();
		return FReply::Handled();
	}
	
	if (Key == EKeys::Escape)
	{
		OnCloseClicked();
		return FReply::Handled(); 
	}
	
	return FReply::Unhandled();
}

void UOptionMenuWidget::SetParentMenu(UUserWidget* InParentMenu)
{
	ParentMenu = InParentMenu;
}

/** 적용(Apply) 버튼 / Enter 키 처리 */
void UOptionMenuWidget::OnApplyClicked()
{
	if (!GameInstanceRef || !SensitivitySlider)
	{
		UE_LOG(LogTemp, Error, TEXT("UOptionMenuWidget: ApplyClicked failed. GameInstance or Slider is NULL."));
		return;
	}
	
	GameInstanceRef->MouseSensitivity = SensitivitySlider->GetValue();
	
	GameInstanceRef->ApplyAndSaveSettings();
	
	// if (GEngine)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("감도 설정이 적용되었습니다"));
	// }
    
	// 4. 적용 후 메뉴 닫기
	OnCloseClicked();
}

/** 닫기(Close) 버튼 / Esc 키 처리 (메인 메뉴로 포커스 복원) */
void UOptionMenuWidget::OnCloseClicked()
{
	RemoveFromParent();

	// 메인 메뉴로 포커스 복원
	APlayerController* PC = GetOwningPlayer();
	// ParentMenu가 MainMenuWidget에서 SetParentMenu()를 통해 설정되었는지 확인
	if (PC && ParentMenu)
	{
		// 입력 모드를 UIOnly로 다시 설정
		FInputModeUIOnly InputMode;
		// 포커스를 부모(메인 메뉴)로 지정
		InputMode.SetWidgetToFocus(ParentMenu->TakeWidget());
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;

		// 메인 메뉴가 즉시 키 입력을 받을 수 있도록 강제로 포커스를 설정
		ParentMenu->SetKeyboardFocus();
	}
}

/** 슬라이더 값이 변경될 때 텍스트 업데이트 (BP의 On Value Changed 로직) */
void UOptionMenuWidget::OnSliderValueChanged(float NewValue)
{
	UpdateSensitivityText(NewValue);
}

/** (헬퍼 함수) 슬라이더 값(float)을 텍스트(int)로 변환 */
void UOptionMenuWidget::UpdateSensitivityText(float SliderValue)
{
	if (SensitivityText)
	{
		float DisplayValue = SliderValue * 100.0f;
		int32 RoundedValue = FMath::RoundToInt(DisplayValue);
		FText NewText = FText::AsNumber(RoundedValue);
        
		SensitivityText->SetText(NewText);
	}
}

void UOptionMenuWidget::OnSliderCaptureEnded()
{
	SetKeyboardFocus();
}