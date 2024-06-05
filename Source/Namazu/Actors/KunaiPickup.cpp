
#include "KunaiPickup.h"
#include "Namazu/Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

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

	if (APlayerCharacter::KunaiCount < APlayerCharacter::MaxKunai)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), CollectSoundEffect, GetActorLocation());
		APlayerCharacter::KunaiCount++;
		Destroy(); 
	}
}