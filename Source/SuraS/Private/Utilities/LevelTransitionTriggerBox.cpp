// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/LevelTransitionTriggerBox.h"
#include "Characters/PawnBAsePlayer/SuraPawnPlayer.h"
#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

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

	//FLatentActionInfo LatentInfo;
	//LatentInfo.CallbackTarget = this;
	////LatentInfo.ExecutionFunction = FName("TransitToNewLevel");
	//LatentInfo.ExecutionFunction = FName("TransitToNewLevel_Async");
	//LatentInfo.Linkage = 0;
	//LatentInfo.UUID = 0;
	//UKismetSystemLibrary::Delay(this, 2.f, LatentInfo);

	TransitToNewLevel_Async();
}

void ALevelTransitionTriggerBox::TransitToNewLevel()
{
	//UE_LOG(LogTemp, Error, TEXT("ALevelTransitionTriggerBox::TransitToNewLevel()"));

	if (!Level.IsValid()) 
	{	Level.LoadSynchronous();
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), Level);
	}
	else
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), Level);
	}

	ASuraPawnPlayer* const PlayerActor = Cast<ASuraPawnPlayer>(Player);
	if (!PlayerActor) return;
	UWeaponSystemComponent* WSC = PlayerActor->GetWeaponSystemComponent();
	if (WSC)
	{
		WSC->UnlockWeapon(NewWeaponName);
	}


	FadeOut();
	//UE_LOG(LogTemp, Error, TEXT("FadeOut!!!"));
	SetInputEnabled(true);
}

void ALevelTransitionTriggerBox::TransitToNewLevel_Async()
{
	//UE_LOG(LogTemp, Error, TEXT("ALevelTransitionTriggerBox::TransitToNewLevel_Async()"));

	if (bTransitioning) return;
	bTransitioning = true;

	if (Level.IsNull())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Level soft reference is null."));
		bTransitioning = false;
		return;
	}

	FStreamableManager& SM = UAssetManager::GetStreamableManager();
	const FSoftObjectPath Path = Level.ToSoftObjectPath();

	PendingLevelHandle = SM.RequestAsyncLoad(Path, FStreamableDelegate::CreateUObject(this, &ALevelTransitionTriggerBox::OnLevelPreloaded));
}

void ALevelTransitionTriggerBox::OnLevelPreloaded()
{
	//UE_LOG(LogTemp, Error, TEXT("ALevelTransitionTriggerBox::OnLevelPreloaded()"));

	UGameplayStatics::OpenLevelBySoftObjectPtr(this, Level);


	//ASuraPawnPlayer* const PlayerActor = Cast<ASuraPawnPlayer>(Player);
	//if (!PlayerActor) return;
	//UWeaponSystemComponent* WSC = PlayerActor->GetWeaponSystemComponent();
	//if (WSC)
	//{
	//	WSC->UnlockWeapon(NewWeaponName);
	//}


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

