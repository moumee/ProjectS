// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Boss/SuraBossAttackArea.h"

#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"


// Sets default values
ASuraBossAttackArea::ASuraBossAttackArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AttackBox = CreateDefaultSubobject<UBoxComponent>("AttackBox");
	RootComponent = AttackBox;
	AttackBox->SetCollisionProfileName("BossAttack");

	IconBillboard = CreateDefaultSubobject<UBillboardComponent>("IconBillboard");
	IconBillboard->SetupAttachment(GetRootComponent());
	IconBillboard->SetHiddenInGame(true);
}

void ASuraBossAttackArea::SetAttackAreaCollision(ECollisionEnabled::Type CollisionEnabled)
{
	AttackBox->SetCollisionEnabled(CollisionEnabled);
}

// Called when the game starts or when spawned
void ASuraBossAttackArea::BeginPlay()
{
	Super::BeginPlay();
	
}



