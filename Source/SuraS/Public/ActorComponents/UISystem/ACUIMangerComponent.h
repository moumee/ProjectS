// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ACUIMangerComponent.generated.h"

class UWeaponSystemComponent;
class UPlayerHUD;
class UACPlayerHUDManager;
class UInputAction;
class UEnhancedInputComponent;
class UBaseUIWidget;
class UACInventoryManager;
class UACCrosshairManager;
class UACKillLogManager;
class UACSkillManager;
class UACPlayerHUDManager;
class UDamageIndicatorWidget;

UENUM(BlueprintType)
enum class EUIType : uint8
{
	None UMETA(DisplayName = "None"),
	Inventory UMETA(DisplayName = "Inventory"),
	KillLog UMETA(DisplayName = "KillLog"),
	PlayerHUD UMETA(DisplayName = "PlayerHUD"),
	Skill UMETA(DisplayName = "Skill"),
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURAS_API UACUIMangerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UACUIMangerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SetupInput();

	/** UI Manager들 **/
	UPROPERTY()
	UACInventoryManager* InventoryManager;

	UPROPERTY()
	UACKillLogManager* KillLogManager;

	UPROPERTY()
	UACPlayerHUDManager* PlayerHUDManager;

	UPROPERTY()
	UACSkillManager* SkillManager;
	
	UPROPERTY()
	UWeaponSystemComponent* WeaponSystemComponent;

	

public:	
	// Called every frame
	// virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	

	/** 특정 UI 열기/ 닫기 **/
	void OpenUI(EUIType UIType);

	UBaseUIWidget* GetWidget(EUIType UIType);
	void InitializeWidgets();
	void InitializeManagers();

	UFUNCTION(BlueprintCallable)
	UACKillLogManager* GetKillLogManager() const { return KillLogManager; }

	UFUNCTION(BlueprintCallable)
	UACPlayerHUDManager* GetPlayerHudManger() const { return PlayerHUDManager; }

	UDataTable* GetWeaponDataTable() const
	{
		return DTWeapon;	
	}

	UDataTable* GetUIDataTable() const
	{
		return DTUISetting;
	}
	
	UWeaponSystemComponent* GetWeaponSystemComponent() const {return WeaponSystemComponent;}

	UFUNCTION(BlueprintCallable)
	void ShowDamageIndicator(AActor* DamageCauser);

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* OpenInventoryAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* OpenPauseMenuAction;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDataTable* DTWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDataTable* DTUISetting;

	UPROPERTY(EditAnywhere, Category = "UI")
	TMap<EUIType, TSubclassOf<UBaseUIWidget>> UIWidgetClasses; // 위젯 블루프린트 클래스

	/** 생성된 UI 위젯 관리 **/
	UPROPERTY()
	TMap<EUIType, UBaseUIWidget*> UIWidgets;

	// damage indicator widget pool
	UPROPERTY()
	TArray<UDamageIndicatorWidget*> DamageIndicatorPool;

	UPROPERTY(EditAnywhere, Category = "UI Settings")
	int32 PoolSize = 5; // default

	// damage indicator widget class
	UPROPERTY(EditDefaultsOnly, Category = "UI Widget Classes")
	TSubclassOf<UDamageIndicatorWidget> DamageIndicatorWidgetClass;

	// get available widget from pool
	UDamageIndicatorWidget* GetAvailableDamageIndicatorFromPool();
};
