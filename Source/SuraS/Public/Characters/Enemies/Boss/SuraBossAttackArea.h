// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SuraBossAttackArea.generated.h"

class ASuraCharacterBossBase;
enum class EDamageType : uint8;
class UBoxComponent;
class UBillboardComponent;

UCLASS()
class SURAS_API ASuraBossAttackArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASuraBossAttackArea();

	UBoxComponent* GetAttackBox() const { return AttackBox; }


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> AttackBox;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBillboardComponent> IconBillboard;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
