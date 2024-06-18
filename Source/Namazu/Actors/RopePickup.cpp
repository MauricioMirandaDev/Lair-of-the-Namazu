
#include "RopePickup.h"
#include "Kismet/GameplayStatics.h"
#include "Namazu/Actors/Rope.h"
#include "Namazu/Characters/GrappleComponent.h"
#include "Namazu/Characters/PlayerCharacter.h"

// Sets default values
ARopePickup::ARopePickup()
{
	
}

// Called when the game starts or when spawned
void ARopePickup::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARopePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Have the player collect this item when the collider is overlapped
void ARopePickup::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::BeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (APlayerCharacter::RopeCount < APlayerCharacter::MaxRope)
	{
		APlayerCharacter::RopeCount++;
		APlayerCharacter::CollectedPickups.Add(PickupTag, this);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), CollectSoundEffect, GetActorLocation());

		if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
			Player->GetGrappleComponent()->GetRope()->SetActorTickEnabled(true);

		Destroy();
	}
}
