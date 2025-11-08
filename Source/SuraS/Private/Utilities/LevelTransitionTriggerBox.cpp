// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/LevelTransitionTriggerBox.h"
#include "Characters/PawnBAsePlayer/SuraPawnPlayer.h"
#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

ALevelTransitionTriggerBox::ALevelTransitionTriggerBox()
{
	//PrimaryActorTick.bCanEverTick = true;
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(FName("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetGenerateOverlapEvents(true);

}

void ALevelTransitionTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ALevelTransitionTriggerBox::OnOverlapBegin);
}

//void ALevelTransitionTriggerBox::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void ALevelTransitionTriggerBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SetActorEnableCollision(false);
	if (!OtherActor) return;
	Player = OtherActor;
	SetInputEnabled(false);
	if (!LoadingWidgetClass) return;
	LoadingWidget = CreateWidget<UUserWidget>(GetWorld(), LoadingWidgetClass);
	if (!LoadingWidget) return;
	LoadingWidget->AddToViewport();
	FadeIn();

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("TransitToNewLevel");
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = 0;
	UKismetSystemLibrary::Delay(this, 2.f, LatentInfo);
}

void ALevelTransitionTriggerBox::TransitToNewLevel()
{
	ASuraPawnPlayer* const PlayerActor = Cast<ASuraPawnPlayer>(Player);
	if (!PlayerActor) return;
	UWeaponSystemComponent* WSC = PlayerActor->GetWeaponSystemComponent();
	if (WSC)
	{
		WSC->UnlockWeapon(NewWeaponName);
	}

	if (!Level.IsValid()) 
	{	Level.LoadSynchronous();
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), Level);
	}
	else
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), Level);
	}

	FadeOut();
	//UE_LOG(LogTemp, Error, TEXT("FadeOut!!!"));
	SetInputEnabled(true);
}

void ALevelTransitionTriggerBox::SetInputEnabled(bool bEnable)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		if (bEnable)
		{
			PlayerController->SetInputMode(FInputModeGameOnly());
			PlayerController->SetIgnoreMoveInput(false);
			PlayerController->SetIgnoreLookInput(false);
			PlayerController->bShowMouseCursor = false;
			//UE_LOG(LogTemp, Log, TEXT("Input Disabled"));
		}
		else
		{
			PlayerController->SetInputMode(FInputModeUIOnly());
			PlayerController->bShowMouseCursor = true;
			PlayerController->SetIgnoreMoveInput(true);
			PlayerController->SetIgnoreLookInput(true);
			//UE_LOG(LogTemp, Log, TEXT("Input Enabled (Default)"));
		}
	}
}

