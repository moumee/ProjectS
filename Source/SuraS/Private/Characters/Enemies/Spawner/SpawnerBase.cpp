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
		UE_LOG(LogTemp, Log, TEXT("%s"), *PooledObject->GetName());
		GetGameInstance()->GetSubsystem<UObjectPoolManager>()->GetPool(PooledObject, GetWorld())
		->GetPooledObject(GetActorLocation() + FVector(FMath::RandRange(-200, 200), FMath::RandRange(-200, 200), 0), FRotator().ZeroRotator);
	}
}

// Called when the game starts or when spawned
void ASpawnerBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (PooledObject)
	{
		Pool = GetGameInstance()->GetSubsystem<UObjectPoolManager>()->GetPool(PooledObject, GetWorld());
		Pool->BehaviorTree = BehaviorTree;
		GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &ASpawnerBase::SpawnWrapper, interval, true);
	}
}


