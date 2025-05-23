

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

//TODO: ��� �����ϱ�
#include "WeaponInterface.h"
#include "WeaponSystemComponent.generated.h"

// delegate about inventory widget (writted by suhyeon)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponPickedUp, FName, WeaponName);

class AWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURAS_API UWeaponSystemComponent : public UActorComponent, public IWeaponInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponSystemComponent();

	// 델리게이트 선언 (by suhyeon)
	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FOnWeaponPickedUp OnWeaponPickedUp;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#pragma region PlayerReference
private:
	void InitializePlayerReference();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "WeaponSystem|PlayerReference", Meta = (AllowPrivateAccess = "true"))
	class ASuraCharacterPlayerWeapon* PlayerOwner;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "WeaponSystem|PlayerReference", Meta = (AllowPrivateAccess = "true"))
	class APlayerController* PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* WeaponSystemMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SwitchWeaponUpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SwitchWeaponDownAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SwitchWeapon1Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SwitchWeapon2Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SwitchWeapon3Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LeftMouseButtonAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RightMouseButtonAction;


	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TargetingSystem|PlayerReference", Meta = (AllowPrivateAccess = "true"))
	//class APlayerCameraManager* PlayerCameraManager;
#pragma endregion

#pragma region SearchWeapon

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSystem")
	float SearchWeaponRadius = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSystem")
	float SearchWeaponViewportRatio_Width = 0.7;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSystem")
	float SearchWeaponViewportRatio_Height = 0.7;

	UPROPERTY()
	class ASuraWeaponPickUp* OverlappedWeapon;

public:
	bool SearchWeapon();
	
	TTuple<FVector2D, bool> GetScreenPositionOfWorldLocation(const FVector& SearchLocation) const;

	bool IsInViewport(FVector2D ActorScreenPosition, float ScreenRatio_Width = 0.0f, float ScreenRatio_Height = 0.0f) const;

#pragma endregion

#pragma region Interaction

public:
	void PickUpWeapon();
	bool ObtainNewWeapon(ASuraWeaponPickUp* NewWeaponPickUp);
	bool ObtainAmmo(ASuraWeaponPickUp* MagazinePickUp);
#pragma endregion

#pragma region Zoom

protected:
	bool bIsZoomIn = false;

public:
	bool IsZoomIn() const { return bIsZoomIn; }
	virtual void ZoomIn(bool bZoomIn) override;

	bool IsWeaponModifyingCamFov();

#pragma endregion

#pragma region Aiming

protected:
	FVector ScreenCenterWorldLocation;

	FVector ScreenCenterWorldDirection;

	FVector RightHandToAimSocketOffset;

	FVector TargetRightHandWorldLocation;

public:

	virtual void SetRightHandToAimSocketOffset(FVector offset) override;

	FVector GetScreenCenterWorldPosition() const { return ScreenCenterWorldLocation; }

	FVector GetScreenCenterWorldDirection() const { return ScreenCenterWorldDirection; }

	FVector CalculateScreenCenterWorldPositionAndDirection(FVector& OutWorldPosition, FVector& OutWorldDirection) const;

	FVector GetTargetRightHandWorldLocation() const { return TargetRightHandWorldLocation; }

	FVector CalculateTargetRightHandPosition();

	FTransform GetWeaponAimSocketRelativeTransform();

	//TODO: World Space�� location�� rotation�� bonespace�� ������ ��ȯ�ϴ� �Լ� �ϼ��ϱ�
	//FVector ConvertTargetToBoneSpace(const FVector& TargetWorldLocation, const FName& BoneName) const;

#pragma endregion

#pragma region SwitchWeapon
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
	TArray<AWeapon*> WeaponInventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AWeapon* CurrentWeapon = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 CurrentWeaponIndex = 0;

public:
	AWeapon* GetCurrentWeapon() { return CurrentWeapon; }

	int32 GetWeaponNum() { return WeaponInventory.Num(); }

	void SwitchToPreviousWeapon();
	void SwitchToNextWeapon();
	void SwitchToIndex1();
	void SwitchToIndex2();
	void SwitchToIndex3();

	virtual void SwitchToOtherWeapon() override;

	void ChangeWeapon(int32 WeaponIndex);

	/** suhyeon **/
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	const TArray<AWeapon*>& GetWeaponInventory() const {return WeaponInventory; }

	int32 GetCurrentWeaponIndex() const {return CurrentWeaponIndex; }
	/** suhyeon**/
#pragma endregion
	
};
