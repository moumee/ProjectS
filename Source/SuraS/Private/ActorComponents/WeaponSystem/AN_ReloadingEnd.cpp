// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/AN_ReloadingEnd.h"
#include "ActorComponents/WeaponSystem/WeaponInterface.h"
#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h"
#include "ActorComponents/WeaponSystem/ACWeapon.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"

namespace
{
	void TryCallReloadingEnd(UObject* Object)
	{
		if (!Object) return;
		IWeaponInterface* WeaponInterface = Cast<IWeaponInterface>(Object);
		if (WeaponInterface) { WeaponInterface->ReloadingEnd(); }
	}
}

void UAN_ReloadingEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp) return;

	AActor* Owner = MeshComp->GetOwner();
	TryCallReloadingEnd(Owner);

	ASuraPawnPlayer* Player = Cast<ASuraPawnPlayer>(Owner);
	if (!Player) return;

	UWeaponSystemComponent* WeaponSystem = Player->GetWeaponSystemComponent();
	if (!WeaponSystem) return;

	AActor* CurrentWeapon = WeaponSystem->GetCurrentWeapon();
	TryCallReloadingEnd(CurrentWeapon);
}
