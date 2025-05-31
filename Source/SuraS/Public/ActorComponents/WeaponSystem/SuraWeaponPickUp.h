

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ActorComponents/WeaponSystem/WeaponName.h"
#include "InteractInterface.h"

#include "SuraWeaponPickUp.generated.h"

class AWeapon;
class USuraPickUpComponent;
class ASuraCharacterBase;
//class ASuraCharacterPlayerWeapon;
class ASuraPawnPlayer;

UCLASS()
class SURAS_API ASuraWeaponPickUp : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASuraWeaponPickUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UFUNCTION()
	void AttachToCharacter(ASuraPawnPlayer* Character);

	UFUNCTION()
	AWeapon* SpawnWeapon(ASuraPawnPlayer* Character);

	void DestroyWeaponPickUp();

	//---------------------------------------------------------------------------------
	// <Interface>
public:

	virtual void Interact(ACharacter* Character) override;


protected:
	/** Weapon class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponName WeaponName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	USuraPickUpComponent* PickUpComponent;
protected:
	UPROPERTY(EditAnywhere)
	bool bIsMagazine = false;

	UPROPERTY(EditAnywhere)
	int32 NumAmmo = 10;

public:
	EWeaponName GetWeaponName() const { return WeaponName; }
	bool IsMagazine() const { return bIsMagazine; }
	int32 GetAmmo() const { return NumAmmo; }

	// suhyeon
	// 무기 클래스 설정
	void SetWeaponClass(TSubclassOf<AWeapon> InClass)
	{
		WeaponClass = InClass;
	}

	// 무기 이름 설정
	void SetWeaponName(EWeaponName InName)
	{
		WeaponName = InName;
	}
	
};
