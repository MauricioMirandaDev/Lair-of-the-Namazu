
#include "KunaiPickup.h"
#include "Kismet/GameplayStatics.h"
#include "Namazu/Characters/PlayerCharacter.h"

// Sets default values
AKunaiPickup::AKunaiPickup()
{
	
}

// Called when the game starts or when spawned
void AKunaiPickup::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AKunaiPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Have the player collect this item when the collider is overlapped
void AKunaiPickup::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::BeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (APlayerCharacter::KunaiCount < APlayerCharacter::MaxKunai)
	{
		APlayerCharacter::KunaiCount++;
		APlayerCharacter::CollectedPickups.Add(PickupTag, this);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), CollectSoundEffect, GetActorLocation());
		Destroy();
	}
}
