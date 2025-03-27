//
//원하는 액터를 소환하는 스포너 예제
// 1. 해당 스포너는 PoolManager에서 pool을 가져와 pool에서 actor를 반환받는 일련의 과정의 예제이다.
// 2. interval을 주기로 액터를 스폰한다
//


#include "Characters/Enemies/Spawner/SpawnerBase.h"
#include "Instance/ObjectPoolManager.h"

// Sets default values
ASpawnerBase::ASpawnerBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASpawnerBase::Tick(float DeltaTime)
{
	
}

void ASpawnerBase::SpawnWrapper()
{
	for (int i = 0; i < spawnCount; i++)
	{
		Pool->GetPooledObject(GetActorLocation(), FRotator().ZeroRotator);
	}
}

// Called when the game starts or when spawned
void ASpawnerBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (PooledObject)
	{
		Pool = GetGameInstance()->GetSubsystem<UObjectPoolManager>()->GetPool(PooledObject, GetWorld());
		GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &ASpawnerBase::SpawnWrapper, interval, true);
	}
}


