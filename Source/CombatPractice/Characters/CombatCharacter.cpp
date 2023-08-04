
#include "CombatCharacter.h"
#include "CombatPractice/Actors/Weapon.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	MaxHealth = 100.0f;
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
		Weapon->OwningCharacter = this;
	}
}

// Called every frame
void ACombatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

// Deduct damage from health and update gameplay as needed
void ACombatCharacter::TakeDamage(FAttackAnimation AttackAnimation, FVector AttackLocation)
{
	CurrentHealth -= AttackAnimation.DamageAmount;

	// Rotate to face attacking character
	FRotator LookAtRotation = (AttackLocation - GetActorLocation()).Rotation();
	SetActorRotation(FRotator(0.0f, LookAtRotation.Yaw, 0.0f), ETeleportType::None);

	// Add knockback force and update combat state
	switch (AttackAnimation.AttackType)
	{
	case EAttackType::ATTACK_Heavy:
		CombatState = ECombatState::COMBAT_DamagedHeavy;
		LaunchCharacter((-GetActorForwardVector() * AttackAnimation.KnockbackStrength) + FVector(0.0f, 0.0f, 500.0f), true, true);
		break;
	default:
		CombatState = ECombatState::COMBAT_DamagedNormal;
		LaunchCharacter(-GetActorForwardVector() * AttackAnimation.KnockbackStrength, true, true);
		break;
	}

	if (IsDead())
		OnDeath();
}

// Aply forward velocity when performing attack
void ACombatCharacter::ForwardThrust()
{
	// Variables used for trace
	FVector StartLocation = GetActorLocation() + (GetActorForwardVector() * (CurrentAttackAnimation.ForwardThrustStrength / 7.5f));
	FVector EndLocation = StartLocation + (GetActorUpVector() * -CurrentAttackAnimation.ForwardThrustStrength);

	TArray<AActor*> ActorsToIngore;
	ActorsToIngore.Add(this);

	FHitResult TraceResult; 

	UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(), StartLocation, EndLocation, GetCapsuleComponent()->GetUnscaledCapsuleRadius(), 
											 GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight(), UEngineTypes::ConvertToTraceType(ECC_Visibility), 
											 true, ActorsToIngore, EDrawDebugTrace::None, OUT TraceResult, true);

	// Perform a forward thrust if it won't launch the character off an edge
	if (TraceResult.bBlockingHit)
		LaunchCharacter(GetActorForwardVector() * CurrentAttackAnimation.ForwardThrustStrength, true, true);
}
