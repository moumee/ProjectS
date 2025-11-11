// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;
class UWorld;
/**
 * 
 */
UCLASS()
class SURAS_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	// --- 위젯 바인딩 변수 ---
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Play;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_NewGame;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Options;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Quit;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> OptionsWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level")
	TSoftObjectPtr<UWorld> LevelToLoad;

	// Continue Game
	UFUNCTION()
	void OnPlayClicked();
	
	UFUNCTION()
	void OnNewGameClicked();

	UFUNCTION()
	void OnOptionsClicked();

	UFUNCTION()
	void OnQuitClicked();

private:
	UPROPERTY()
	UUserWidget* OptionsMenuInstance;
	
};
