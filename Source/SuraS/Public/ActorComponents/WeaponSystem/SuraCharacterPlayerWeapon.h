// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "SuraCharacterPlayerWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API ASuraCharacterPlayerWeapon : public ASuraCharacterPlayer
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void Landed(const FHitResult& Hit) override;

public:
	ASuraCharacterPlayerWeapon();

//-----------------------------------------------------------------
//TODO: ��ȯ�Բ� �Ʒ� ����� Player Character class�� �ʿ��ϴٰ� �����帮��. 
// �� ASuraCharacterPlayer�� private member�� protected�� ��ȯ ��Ź�帮��

public:
	/** Returns ArmMesh subobject **/
	USkeletalMeshComponent* GetArmMesh() const { return ArmMesh; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return Camera; }

};
