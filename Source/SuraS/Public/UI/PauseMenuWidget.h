// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

/**
 * 
 */

class UButton;
class UOptionMenuWidget;
class UACUIMangerComponent;


UCLASS()
class SURAS_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_Play; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_Options;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_Quit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> OptionsWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TSoftObjectPtr<UWorld> MainMenuLevel;

	UFUNCTION()
	void OnResumeClicked();
	
	UFUNCTION()
	void OnOptionsClicked();
	
	UFUNCTION()
	void OnQuitClicked();

private:
	UPROPERTY()
	TObjectPtr<UUserWidget> OptionsMenuInstance;
	
	UPROPERTY()
	TObjectPtr<UACUIMangerComponent> UIManager;
	
};
