
#include "Weapon.h"
#include "CombatPractice/CombatDamageType.h"
#include "CombatPractice/Characters/CombatCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ImpactSoundEffect = nullptr; 
	CurrentDamageType = nullptr; 

	// Create scene component and set as root
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// Create static mesh component and set default values
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Create box component and set default values
	Hitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
	Hitbox->SetupAttachment(Mesh);
	Hitbox->SetCollisionProfileName(TEXT("NoCollision"), true);
	Hitbox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::BeginOverlap);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Turn the hitbox on or off, and update the type of damage
void AWeapon::UpdateHitbox(FName UpdatedCollisionProfile, TSubclassOf<class UCombatDamageType> UpdatedDamageType)
{
	Hitbox->SetCollisionProfileName(UpdatedCollisionProfile);
	CurrentDamageType = UpdatedDamageType; 
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Apply damage to the overlapped actor 
void AWeapon::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp && CurrentDamageType)
	{
		UGameplayStatics::ApplyDamage(OtherActor, CurrentDamageType.GetDefaultObject()->Amount, nullptr, this, CurrentDamageType);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSoundEffect, GetActorLocation());
		Hitbox->SetCollisionProfileName(TEXT("NoCollision"), true);	
	}
}



