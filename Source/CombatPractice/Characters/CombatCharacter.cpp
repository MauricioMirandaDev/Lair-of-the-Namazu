
#include "CombatCharacter.h"
#include "CombatPractice/Actors/Weapon.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ACombatCharacter::ACombatCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	SetCanBeDamaged(true);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	CombatState = ECombatState::COMBAT_Neutral; 
	WeaponClass = nullptr;
	HitAnimation = nullptr;
	ThrustStrength = 100.0f; 
	MaxHealth = 100.0f;

	OnTakeAnyDamage.AddDynamic(this, &ACombatCharacter::ReceiveDamage);
}

// Called when the game starts or when spawned
void ACombatCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;

	// Spawn a weapon in the world and attach it to character
	if (WeaponClass)
	{
		Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		Weapon->SetOwner(this);
	}
}

// Called every frame
void ACombatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Play hit animation and deduct amount from health
void ACombatCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (HitAnimation)
	{
		CurrentHealth -= Damage;
		PlayAnimMontage(HitAnimation, 1.0f, TEXT("None"));
	}

	if (IsDead())
		OnDeath();
}

// Determines if the character has lost all health
bool ACombatCharacter::IsDead()
{
	return CurrentHealth <= 0.0f;
}

// Called when character runs out of health
void ACombatCharacter::OnDeath()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"), true);
}

void ACombatCharacter::ForwardThrust()
{
	// Variables used for trace
	FVector StartLocation = GetActorLocation() + (GetActorForwardVector() * 100.0f);
	FVector EndLocation = StartLocation + (GetActorUpVector() * -500.0f);

	TArray<AActor*> ActorsToIngore;
	ActorsToIngore.Add(this);

	FHitResult TraceResult; 

	UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(), StartLocation, EndLocation, GetCapsuleComponent()->GetUnscaledCapsuleRadius(), 
											 GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight(), UEngineTypes::ConvertToTraceType(ECC_Visibility), 
											 true, ActorsToIngore, EDrawDebugTrace::None, OUT TraceResult, true);

	// Perform a forward thrust if it won't launch the character off an edge
	if (TraceResult.bBlockingHit)
		LaunchCharacter(GetActorForwardVector() * ThrustStrength, true, true);
}
