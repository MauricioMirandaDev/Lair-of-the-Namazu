
#include "LongRangeWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Namazu/NamazuGameModeBase.h"
#include "Namazu/Characters/CombatCharacter.h"
#include "Namazu/Characters/PlayerCharacter.h"
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
	FVector FireDirection = ANamazuGameModeBase::PlayerRef->GetActorLocation() - OwningCharacter->GetActorLocation();
	FireDirection.Normalize();

	FVector FireLocation = OwningCharacter->GetActorLocation() + (FireDirection * 100.0f);

	GetWorld()->SpawnActor<AProjectile>(ProjectileClass, FireLocation, FireDirection.Rotation());
}