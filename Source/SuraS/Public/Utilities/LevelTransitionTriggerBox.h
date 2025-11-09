// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "ActorComponents/WeaponSystem/WeaponName.h"
#include "LevelTransitionTriggerBox.generated.h"

struct FStreamableHandle;

UCLASS()
class SURAS_API ALevelTransitionTriggerBox : public AActor
{
	GENERATED_BODY()
public:	
	ALevelTransitionTriggerBox();
protected:
	virtual void BeginPlay() override;
public:	
	//virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerBox")
	UBoxComponent* TriggerBox = nullptr;
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintImplementableEvent)
	void FadeIn();
	UFUNCTION(BlueprintImplementableEvent)
	void FadeOut();
	UFUNCTION()
	void TransitToNewLevel();
	void TransitToNewLevel_Async();
	void OnLevelPreloaded();
protected:
	void SetInputEnabled(bool bEnable);
public:
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "LoadingWidgetClass")
	TSubclassOf<UUserWidget> LoadingWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	const TSoftObjectPtr<UWorld> Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NewWeaponName")
	EWeaponName NewWeaponName = EWeaponName::WeaponName_Rifle;
protected:
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "LoadingWidget")
	UUserWidget* LoadingWidget = nullptr;

	UPROPERTY(VisibleAnywhere)
	AActor* Player = nullptr;

	TSharedPtr<FStreamableHandle> PendingLevelHandle;
	UPROPERTY(VisibleAnywhere)
	bool bTransitioning = false;

};
