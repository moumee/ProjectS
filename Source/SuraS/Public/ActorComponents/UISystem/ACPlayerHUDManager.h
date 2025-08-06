// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ACPlayerHUDManager.generated.h"


class UWeaponSystemComponent;
class UPlayerHUD;
class UACUIMangerComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURAS_API UACPlayerHUDManager : public UActorComponent
{
	GENERATED_BODY()

	UACUIMangerComponent* UIManager;

	//Inventory 위젯을 저장할 변수
	UPROPERTY()
	UPlayerHUD* PlayerHUDWidget;

public:	
	// Sets default values for this component's properties
	UACPlayerHUDManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UWeaponSystemComponent* WeaponSystemComponent = nullptr;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//inventorywidget 참조
	void SetPlayerHUDWidget(UPlayerHUD* InWidget);
	UPlayerHUD* GetPlayerHudWidget() const {return PlayerHUDWidget; }

	// UIManagerComponent 할당
	void SetUIManager(UACUIMangerComponent* UIMangerComponent);
	
};
