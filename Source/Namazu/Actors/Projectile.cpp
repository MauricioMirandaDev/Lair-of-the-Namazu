
#include "Projectile.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Namazu/Characters/PlayerCharacter.h"
#include "Namazu/NamazuGameModeBase.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileDamage = FAttackAnimation();
	ImpactSoundEffect = nullptr; 

	// Create box component and set default values
	Hitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
	SetRootComponent(Hitbox);
	Hitbox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit); 

	// Create static mesh component and set default values
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Hitbox);
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	// Create projectile movement component and set default values
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Apply damage to character on impact
void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
		ANamazuGameModeBase::PlayerRef->TakeDamage(ProjectileDamage, GetActorLocation());

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSoundEffect, GetActorLocation());
	Destroy(); 
}

