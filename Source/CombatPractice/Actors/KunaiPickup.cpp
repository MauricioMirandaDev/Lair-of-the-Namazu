
#include "KunaiPickup.h"
#include "CombatPractice/Characters/PlayerCharacter.h"

AKunaiPickup::AKunaiPickup()
{
}

void AKunaiPickup::BeginPlay()
{
	Super::BeginPlay();
}

void AKunaiPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Add another kunai to the player's inventory
void AKunaiPickup::CollectPickup()
{
	Super::CollectPickup();

	APlayerCharacter::KunaiCount++; 
}