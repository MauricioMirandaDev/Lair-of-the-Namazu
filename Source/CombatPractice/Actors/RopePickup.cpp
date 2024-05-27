
#include "RopePickup.h"
#include "CombatPractice/Characters/PlayerCharacter.h"

ARopePickup::ARopePickup()
{
}

void ARopePickup::BeginPlay()
{
	Super::BeginPlay();
}

void ARopePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Add another rope to the player's inventory
void ARopePickup::CollectPickup()
{
	Super::CollectPickup();

	APlayerCharacter::RopeCount++;
}