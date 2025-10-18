// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Boss/SuraBossAttackArea.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"
#include "Interfaces/PlayerInterface.h"

#define PLAYER_TRACE_CHANNEL ECC_GameTraceChannel4


// Sets default values
ASuraBossAttackArea::ASuraBossAttackArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AttackBox = CreateDefaultSubobject<UBoxComponent>("AttackBox");
	RootComponent = AttackBox;
	AttackBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackBox->SetCollisionResponseToChannel(PLAYER_TRACE_CHANNEL, ECR_Overlap);
	AttackBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);

	IconBillboard = CreateDefaultSubobject<UBillboardComponent>("IconBillboard");
	IconBillboard->SetupAttachment(GetRootComponent());
	IconBillboard->SetHiddenInGame(true);
}

// Called when the game starts or when spawned
void ASuraBossAttackArea::BeginPlay()
{
	Super::BeginPlay();

	AttackBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBoxBeginOverlap);
	AttackBox->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnBoxEndOverlap);
	
}

void ASuraBossAttackArea::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<IPlayerInterface>(OtherActor) && !Tags.IsEmpty())
	{
		if (UBlackboardComponent* BlackboardComponent = Cast<APawn>(GetOwner())->GetController<AAIController>()->GetBlackboardComponent())
		{
			BlackboardComponent->SetValueAsName("AttackAreaTag", Tags[0]);
		}
	}
}

void ASuraBossAttackArea::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<IPlayerInterface>(OtherActor) && !Tags.IsEmpty())
	{
		if (UBlackboardComponent* BlackboardComponent = Cast<APawn>(GetOwner())->GetController<AAIController>()->GetBlackboardComponent())
		{
			BlackboardComponent->SetValueAsName("AttackAreaTag", NAME_None);
		}
	}
}




