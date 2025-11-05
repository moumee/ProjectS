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

struct FInputActionValue;

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
	UACInventoryManager* InventoryManager = nullptr;

	UPROPERTY()
	UACKillLogManager* KillLogManager = nullptr;

	UPROPERTY()
	UACPlayerHUDManager* PlayerHUDManager = nullptr;

	UPROPERTY()
	UACSkillManager* SkillManager = nullptr;
	
	UPROPERTY()
	UWeaponSystemComponent* WeaponSystemComponent = nullptr;

	/** Pause 메뉴 위젯 블루프린트 클래스 (UUserWidget 또는 UBaseUIWidget일 수 있음) */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuWidgetClass; // UUserWidget으로 선언하는 것이 안전

	/** 현재 뷰포트에 표시 중인 Pause 메뉴 인스턴스 */
	UPROPERTY()
	TObjectPtr<UUserWidget> PauseMenuInstance;

	
	

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

	// UFUNCTION(BlueprintCallable)
	// UACPlayerHUDManager* GetPlayerHudManger() const { return PlayerHUDManager; }

	UDataTable* GetWeaponDataTable() const
	{
		return DTWeapon;	
	}

	//UDataTable* GetWSCDataTable() const // <JaeHyeong>
	//{
	//	return DTWeaponSystemComponent;
	//}

	UDataTable* GetUIDataTable() const
	{
		return DTUISetting;
	}
	
	UWeaponSystemComponent* GetWeaponSystemComponent() const {return WeaponSystemComponent;}

	UFUNCTION(BlueprintCallable)
	void ShowDamageIndicator(AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "UI|Input")
	void TogglePauseMenu();


private:
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* OpenInventoryAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* OpenPauseMenuAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ShowTabMenuAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDataTable* DTWeapon = nullptr;

	//UPROPERTY(EditDefaultsOnly, Category = "Data") // <JaeHyeong>
	//UDataTable* DTWeaponSystemComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDataTable* DTUISetting = nullptr;

	UPROPERTY(EditAnywhere, Category = "UI")
	TMap<EUIType, TSubclassOf<UBaseUIWidget>> UIWidgetClasses; // 위젯 블루프린트 클래스

	// --- UI ---

	/** 블루프린트에서 설정할 Tab 메뉴 위젯 클래스 */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> TabMenuWidgetClass;

	/** 현재 생성된 Tab 메뉴 위젯 인스턴스 (메모리 관리를 위해 UPROPERTY 사용) */
	UPROPERTY()
	UUserWidget* TabMenuWidgetInstance;

	// --- 입력 핸들러 ---
	
	/** ShowTabMenuAction이 시작되었을 때 (Tab 누름) */
	void OnShowTabMenuStarted(const FInputActionValue& Value);

	/** ShowTabMenuAction이 완료/취소되었을 때 (Tab 뗌) */
	void OnShowTabMenuCompleted(const FInputActionValue& Value);

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


