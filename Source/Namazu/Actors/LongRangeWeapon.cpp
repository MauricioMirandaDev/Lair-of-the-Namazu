
#include "LongRangeWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Namazu/Characters/CombatCharacter.h"
#include "Namazu/Actors/Projectile.h"

// Sets default values
ALongRangeWeapon::ALongRangeWeapon()
{
	
}

// Called when the game starts or when spawned
void ALongRangeWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALongRangeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALongRangeWeapon::FireWeapon()
{
	GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Mesh->GetSocketLocation(TEXT("SpawnSocket")), OwningCharacter->GetActorForwardVector().Rotation());
}