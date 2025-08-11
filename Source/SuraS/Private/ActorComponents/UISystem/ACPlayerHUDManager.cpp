// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/UISystem/ACPlayerHudManager.h"
#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h"
#include "Characters/PawnBAsePlayer/SuraPawnPlayer.h"
#include "UI/PlayerHUD.h"

// Sets default values for this component's properties
UACPlayerHUDManager::UACPlayerHUDManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UACPlayerHUDManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	ASuraPawnPlayer* SuraPawnPlayer = Cast<ASuraPawnPlayer>(GetOwner());  // GetOwningPlayerPawn()은 ActorComponent에선 사용 불가
	if (SuraPawnPlayer)
	{
		WeaponSystemComponent = SuraPawnPlayer->GetWeaponSystemComponent();
	}

	PlayerHUDWidget->InitializeHUD();
}


// Called every frame
void UACPlayerHUDManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UACPlayerHUDManager::SetPlayerHUDWidget(UPlayerHUD* InWidget)
{
	PlayerHUDWidget = InWidget;	
}

void UACPlayerHUDManager::SetUIManager(UACUIMangerComponent* UIMangerComponent)
{
	UIManager = UIMangerComponent;
}


