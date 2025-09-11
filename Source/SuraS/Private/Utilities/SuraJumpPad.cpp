// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/SuraJumpPad.h"

#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ASuraJumpPad::ASuraJumpPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>("CollisionBox");
	SetRootComponent(CollisionBox);
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnJumpPadOverlap);
	// GameTraceChannel4 is Player
	CollisionBox->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Overlap);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CollisionBox);
}

// Called when the game starts or when spawned
void ASuraJumpPad::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASuraJumpPad::OnJumpPadOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ASuraPawnPlayer* Player = Cast<ASuraPawnPlayer>(OtherActor))
	{
		Player->JumpPadLaunchPlayer(ForceAmount);
	}
}


