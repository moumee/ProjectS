// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/UISystem/ACKillLogManager.h"

#include "UI/KillLogWidget.h"

// Sets default values for this component's properties
UACKillLogManager::UACKillLogManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}




// Called when the game starts
void UACKillLogManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UACKillLogManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UACKillLogManager::SetKillLogWidget(UKillLogWidget* InWidget)
{
	KillLogWidget = InWidget;
}

void UACKillLogManager::SetUIManager(UACUIMangerComponent* InManager)
{
	UIManager = InManager;
}

void UACKillLogManager::AddKillLog(const FString& Killer, const FString& Victim)
{
	if (!KillLogWidget) return;

	// 1. 해골 이미지 추가
	KillLogWidget->AddSkull();

	// 2. 점수 로그 항목 추가
	KillLogWidget->AddScoreEntry(TEXT("적 처치"), 100);

	// 3. 총 점수 업데이트
	KillLogWidget->UpdateTotalScore(100);

	// (선택) 디버그 메시지
	UE_LOG(LogTemp, Log, TEXT("KillLog: %s killed %s"), *Killer, *Victim);
}
