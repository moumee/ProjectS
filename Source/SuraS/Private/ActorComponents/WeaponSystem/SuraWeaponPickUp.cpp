
#include "ActorComponents/WeaponSystem/SuraWeaponPickUp.h"

#include "ActorComponents/WeaponSystem/ACWeapon.h"
#include "ActorComponents/WeaponSystem/SuraPickUpComponent.h"
#include "ActorComponents/WeaponSystem/WeaponName.h"

#include "Characters/SuraCharacterBase.h"
#include "ActorComponents/WeaponSystem/SuraCharacterPlayerWeapon.h"


// Sets default values
ASuraWeaponPickUp::ASuraWeaponPickUp()
{
	PrimaryActorTick.bCanEverTick = false;

	//Weapon = CreateDefaultSubobject<UACWeapon>(TEXT("Weapon"));
	//Weapon->SetupAttachment(RootComponent);

	WeaponName = EWeaponName::WeaponName_Rifle;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	// TODO: ������ �ʿ� ���� �� ���Ƽ� �����ؾ���
	PickUpComponent = CreateDefaultSubobject<USuraPickUpComponent>(TEXT("PickUpComponent"));
	PickUpComponent->SetupAttachment(WeaponMesh);
}

// Called when the game starts or when spawned
void ASuraWeaponPickUp::BeginPlay()
{
	Super::BeginPlay();
	
	FAttachmentTransformRules AttachRule(EAttachmentRule::SnapToTarget, true);
	//PickUpComponent->AttachToComponent(Weapon, AttachRule);
	PickUpComponent->AttachToComponent(WeaponMesh, AttachRule);

	////TODO: ��� �ٲ�� ������ ������
	//if (PickUpComponent)
	//{
	//	PickUpComponent->OnPickUp.AddDynamic(this, &ASuraWeaponPickUp::AttachToCharacter);
	//}
}

// Called every frame
void ASuraWeaponPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//TODO: �ʿ� ����. ��Ȱ��ȭ ���Ѿ���

}

void ASuraWeaponPickUp::AttachToCharacter(ASuraCharacterPlayerWeapon* Character)
{
	//Weapon->AttachWeaponToPlayer(Character);

	UE_LOG(LogTemp, Warning, TEXT("Weapon has been spwaned"));
}


AWeapon* ASuraWeaponPickUp::SpawnWeapon(ASuraCharacterPlayerWeapon* Character)
{
	AWeapon* NewWeapon = nullptr;
	
	if (WeaponClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr && Character != nullptr)
		{
			//const FVector SpawnLocation = Character->GetActorLocation();
			//const FRotator SpawnRotation = Character->GetActorRotation();

			//FActorSpawnParameters ActorSpawnParams;
			//ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			//NewWeapon = NewObject<AWeapon>(Character, WeaponClass);
			//NewWeapon->RegisterComponent();

			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			NewWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, GetActorTransform(), ActorSpawnParams);
			NewWeapon->InitializeWeapon(Character);
		}
	}

	return NewWeapon;
}

void ASuraWeaponPickUp::DestroyWeaponPickUp()
{
	Destroy();
}

void ASuraWeaponPickUp::Interact(ACharacter* Character)
{
	ASuraCharacterPlayerWeapon* CharacterPickingUp = Cast<ASuraCharacterPlayerWeapon>(Character);
	if (CharacterPickingUp)
	{
		//Weapon->AttachWeaponToPlayer(CharacterPickingUp);
		UE_LOG(LogTemp, Warning, TEXT("Weapon has been spwaned"));
	}
}


void PickUpComponent() //TODO: ???
{

}


