// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/AN_EjectProjectileShell.h"
#include "ActorComponents/WeaponSystem/WeaponInterface.h"
#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h"
#include "ActorComponents/WeaponSystem/ACWeapon.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"

namespace
{
	void TryCallEjectProjectileShell(UObject* Object)
	{
		if (!Object) return;
		IWeaponInterface* WeaponInterface = Cast<IWeaponInterface>(Object);
		if (WeaponInterface) { WeaponInterface->EjectProjectileShell(); }
	}
}

void UAN_EjectProjectileShell::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp) return;

	AActor* Owner = MeshComp->GetOwner();
	TryCallEjectProjectileShell(Owner);

	ASuraPawnPlayer* Player = Cast<ASuraPawnPlayer>(Owner);
	if (!Player) return;

	UWeaponSystemComponent* WeaponSystem = Player->GetWeaponSystemComponent();
	if (!WeaponSystem) return;

	AActor* CurrentWeapon = WeaponSystem->GetCurrentWeapon();
	TryCallEjectProjectileShell(CurrentWeapon);
}
