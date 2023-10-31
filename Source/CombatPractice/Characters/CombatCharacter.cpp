
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
		Weapon->SetOwningCharacter(this);
	}
}

// Called every frame
void ACombatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Getter function to access this character's weapon
AWeapon* ACombatCharacter::GetWeapon()
{
	return Weapon;
}

// Getter function to access the attack animation this character is currently performing
FAttackAnimation ACombatCharacter::GetCurrentAttackAnim()
{
	return CurrentAttackAnimation;
}

// Setter function to update this character's combat state
void ACombatCharacter::SetCombatState(ECombatState NewState)
{
	CombatState = NewState;
}

void ACombatCharacter::SetCurrentAttackAnim(FAttackAnimation NewAnim)
{
	CurrentAttackAnimation = NewAnim;
}

// Used to reset variables a character uses during combat
void ACombatCharacter::ResetAttack()
{
	CurrentAttackAnimation = FAttackAnimation();
}

void ACombatCharacter::PlayAttackAnim(FAttackAnimation AttackAnimation)
{
	PlayAnimMontage(AttackAnimation.Animation, 1.0f, TEXT("None"));
	CurrentAttackAnimation = AttackAnimation;
}

// Aply forward velocity when performing attack
void ACombatCharacter::ForwardThrust()
{
	LaunchCharacter(GetActorForwardVector() * CurrentAttackAnimation.ForwardThrustStrength, true, true);
}

// Called when character runs out of health
void ACombatCharacter::OnDeath()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"), true);
	CombatState = ECombatState::COMBAT_Neutral;

	if (DeathAnimation)
		PlayAnimMontage(DeathAnimation, 1.0f, TEXT("None"));
}

void ACombatCharacter::AfterDeath()
{

}

// Deduct damage from health and update gameplay as needed
void ACombatCharacter::TakeDamage(FAttackAnimation AttackAnimation, FVector AttackLocation)
{
	if (CombatState != ECombatState::COMBAT_DamagedNormal && CombatState != ECombatState::COMBAT_DamagedHeavy)
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
	case EAttackType::ATTACK_Stun:
		CombatState = ECombatState::COMBAT_DamagedStun;
		LaunchCharacter(-GetActorForwardVector() * AttackAnimation.KnockbackStrength, true, true);
		break;
	default:
		CombatState = ECombatState::COMBAT_DamagedNormal;
		LaunchCharacter(-GetActorForwardVector() * AttackAnimation.KnockbackStrength, true, true);
		break;
	}

	if (IsDead())
		OnDeath();
}

// Determines if the character has lost all health
bool ACombatCharacter::IsDead()
{
	return CurrentHealth <= 0.0f;
}
