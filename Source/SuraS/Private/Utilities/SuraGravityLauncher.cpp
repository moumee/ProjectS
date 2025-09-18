// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/SuraGravityLauncher.h"

#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Slate/SGameLayerManager.h"

// Sets default values
ASuraGravityLauncher::ASuraGravityLauncher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>("CollisionBox");
	SetRootComponent(CollisionBox);
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnGravityLauncherOverlap);
	// GameTraceChannel4 is Player
	CollisionBox->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Overlap);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CollisionBox);

	ForceDirectionArrow = CreateDefaultSubobject<UArrowComponent>("ForceDirectionArrow");
	ForceDirectionArrow->SetupAttachment(CollisionBox);

}

// Called when the game starts or when spawned
void ASuraGravityLauncher::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASuraGravityLauncher::OnGravityLauncherOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(OtherActor))
	{
		PlayerInterface->GravityLaunchPlayer(ForceDirectionArrow->GetForwardVector(), ForceAmount);
	}
}



