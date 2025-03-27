//
//���ϴ� ���͸� ��ȯ�ϴ� ������ ����
// 1. �ش� �����ʴ� PoolManager���� pool�� ������ pool���� actor�� ��ȯ�޴� �Ϸ��� ������ �����̴�.
// 2. interval�� �ֱ�� ���͸� �����Ѵ�
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


