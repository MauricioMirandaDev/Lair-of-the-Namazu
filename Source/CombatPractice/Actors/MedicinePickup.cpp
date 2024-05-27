
#include "MedicinePickup.h"
#include "CombatPractice/Characters/PlayerCharacter.h"

AMedicinePickup::AMedicinePickup()
{
}

void AMedicinePickup::BeginPlay()
{
	Super::BeginPlay();
}

void AMedicinePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Add more medicine to the player's inventory
void AMedicinePickup::CollectPickup()
{
	Super::CollectPickup();

	APlayerCharacter::MedicineCount++;
}