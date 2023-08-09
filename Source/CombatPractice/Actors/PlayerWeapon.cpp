
#include "PlayerWeapon.h"
#include "Components/BoxComponent.h"

APlayerWeapon::APlayerWeapon()
{
}

void APlayerWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerWeapon::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::BeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void APlayerWeapon::UpdateHitbox(bool bActivate)
{
	Super::UpdateHitbox(bActivate);

	if (bActivate)
		Hitbox->SetCollisionProfileName(TEXT("PlayerWeapon"), true);
}