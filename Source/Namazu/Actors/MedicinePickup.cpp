
#include "MedicinePickup.h"
#include "Kismet/GameplayStatics.h"
#include "Namazu/Characters/PlayerCharacter.h"

// Sets default values
AMedicinePickup::AMedicinePickup()
{
	
}

// Called when the game starts or when spawned
void AMedicinePickup::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMedicinePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Have the player collect this item when the collider is overlapped
void AMedicinePickup::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::BeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult); 

	if (APlayerCharacter::MedicineCount < APlayerCharacter::MaxMedicine)
	{
		APlayerCharacter::MedicineCount++;
		APlayerCharacter::CollectedPickups.Add(PickupTag, this);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), CollectSoundEffect, GetActorLocation());
		Destroy(); 
	}
}
