
#include "BaseEnemy.h"
#include "CombatPractice/Actors/BaseWeapon.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponClass)
	{
		Weapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass);
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		Weapon->SetOwner(this);
	}
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
