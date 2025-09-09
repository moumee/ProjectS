// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SuraBossAttackArea.generated.h"

class UBoxComponent;

UCLASS()
class SURAS_API ASuraBossAttackArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASuraBossAttackArea();

	void SetAttackAreaCollision(ECollisionEnabled::Type CollisionEnabled);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> AttackBox;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBillboardComponent> IconBillboard;

};
