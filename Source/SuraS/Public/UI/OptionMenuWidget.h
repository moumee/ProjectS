// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionMenuWidget.generated.h"


class UTextBlock;
class USlider;
class UCustomGameInstance;
class UButton;
/**
 * 
 */
UCLASS()
class SURAS_API UOptionMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetParentMenu(UUserWidget* InParentMenu);

protected:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Close;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Apply;

	UPROPERTY(meta = (BindWidget))
	USlider* SensitivitySlider; 
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SensitivityText;

	UFUNCTION()
	void OnApplyClicked();
	
	UFUNCTION()
	void OnCloseClicked();
	
	UFUNCTION()
	void OnSliderValueChanged(float NewValue);

	UFUNCTION()
	void OnSliderCaptureEnded();

private:
	void UpdateSensitivityText(float SliderValue);
	
	// 메인 메뉴 위젯의 참조를 저장할 변수
	UPROPERTY()
	TObjectPtr<UUserWidget> ParentMenu;
	
	UPROPERTY()
	TObjectPtr<UCustomGameInstance> GameInstanceRef;
	
};
