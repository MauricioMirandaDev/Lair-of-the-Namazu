
#include "FallVolume.h"
#include "Namazu/Characters/CombatCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
AFallVolume::AFallVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Volume = CreateDefaultSubobject<UBoxComponent>(TEXT("Volume"));
	Volume->SetCollisionProfileName(TEXT("FallVolume"), true);
	Volume->OnComponentBeginOverlap.AddDynamic(this, &AFallVolume::BeginOverlap);
	SetRootComponent(Volume);
}

// Called when the game starts or when spawned
void AFallVolume::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFallVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Trigger fall to death if character enters volume
void AFallVolume::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACombatCharacter* Character = Cast <ACombatCharacter>(OtherActor))
	{
		Character->OnFallDeath();
	}
}

