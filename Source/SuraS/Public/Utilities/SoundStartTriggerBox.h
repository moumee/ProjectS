// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "SoundStartTriggerBox.generated.h"

UCLASS()
class SURAS_API ASoundStartTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASoundStartTriggerBox();

	UAudioComponent* GetBackgroundMusicAudioComponent() const { return BackgroundMusicAudioComponent; }

	UAudioComponent* GetSpatialEffectAudioComponent() const { return SpatialEffectAudioComponent; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTriggerBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UPROPERTY()
	TObjectPtr<UAudioComponent> BackgroundMusicAudioComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> BackgroundMusic;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAudioComponent> SpatialEffectAudioComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> SpatialEffectAudio;
	
};
