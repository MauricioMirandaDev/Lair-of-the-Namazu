
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
	CurrentAttackAnimation = FAttackAnimation(); 
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

// Getter function to access this character's current state 
ECombatState ACombatCharacter::GetCombatState()
{
	return CombatState;
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

// Setter function to update this character's current attack animation 
void ACombatCharacter::SetCurrentAttackAnim(FAttackAnimation NewAnim)
{
	CurrentAttackAnimation = NewAnim;
}

// Set this character to be able to move or not 
void ACombatCharacter::UpdateMovement(bool bPauseMovement)
{

}

// Used to reset variables a character uses during combat
void ACombatCharacter::ResetAttack()
{
	CurrentAttackAnimation = FAttackAnimation();
}

// Aply forward velocity when performing attack
void ACombatCharacter::ForwardThrust()
{
	LaunchCharacter(GetActorForwardVector() * CurrentAttackAnimation.ForwardThrustStrength, true, true);
}

// Deduct damage from health and update gameplay as needed
void ACombatCharacter::TakeDamage(FAttackAnimation AttackAnimation, FVector AttackLocation)
{
	CurrentHealth -= AttackAnimation.DamageAmount;
	StopAnimMontage(CurrentAttackAnimation.Animation);

	// Deactivate the weapon's hitbox 
	Weapon->UpdateHitbox(false, FVector(0.0f, 0.0f, 0.0f));

	// Rotate to face attacking character
	if (AttackAnimation.AttackType != EAttackType::ATTACK_Behind && AttackAnimation.AttackType != EAttackType::ATTACK_Ground)
	{
		FRotator LookAtRotation = (AttackLocation - GetActorLocation()).Rotation();
		SetActorRotation(FRotator(0.0f, LookAtRotation.Yaw, 0.0f), ETeleportType::None);
	}

	if (IsDead())
		OnDeath();
	else
	{

		// Update combat state
		switch (AttackAnimation.AttackType)
		{
			case EAttackType::ATTACK_Heavy:
				CombatState = ECombatState::COMBAT_DamagedHeavy;
				LaunchCharacter((-GetActorForwardVector() * AttackAnimation.ForwardKnockbackStrength) + FVector(0.0f, 0.0f, AttackAnimation.VerticalKnockbackStrength), true, true);
				break;
			case EAttackType::ATTACK_Stun:
				CombatState = ECombatState::COMBAT_DamagedStun;
				LaunchCharacter(-GetActorForwardVector() * AttackAnimation.ForwardKnockbackStrength, true, true);
				break;
			case EAttackType::ATTACK_Trip:
				CombatState = ECombatState::COMBAT_DamagedTrip;
				break;
			case EAttackType::ATTACK_Behind:
				CombatState = ECombatState::COMBAT_DeadBehind;
				break;
			case EAttackType::ATTACK_Ground:
				CombatState = ECombatState::COMBAT_DeadGround;
				break;
			default:
				CombatState = ECombatState::COMBAT_DamagedNormal;
				LaunchCharacter(-GetActorForwardVector() * AttackAnimation.ForwardKnockbackStrength, true, true);
				break;
		}
	}
}

// Apply effects after the character has finished their death animation 
void ACombatCharacter::AfterDeath()
{

}

// Determines if the character has lost all health
bool ACombatCharacter::IsDead()
{
	return CurrentHealth <= 0.0f;
}

// Play the following attack animation 
void ACombatCharacter::PlayAttackAnim(FAttackAnimation AttackAnimation)
{
	PlayAnimMontage(AttackAnimation.Animation, 1.0f, TEXT("None"));
	CurrentAttackAnimation = AttackAnimation;
}

// Called when character runs out of health
void ACombatCharacter::OnDeath()
{
	CombatState = ECombatState::COMBAT_Dead;
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"), true);
	GetCharacterMovement()->SetActive(false, true); 
}