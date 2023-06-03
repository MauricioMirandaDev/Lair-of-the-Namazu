
#include "CombatCharacter.h"
#include "CombatPractice/Actors/Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACombatCharacter::ACombatCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	WeaponClass = nullptr;
	bCanAttack = true; 
}

// Called when the game starts or when spawned
void ACombatCharacter::BeginPlay()
{
	Super::BeginPlay();

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

// Set variables based on start and end of attack animation
void ACombatCharacter::AttackEffects(bool bIsStart)
{
	if (bIsStart)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		GetCharacterMovement()->SetJumpAllowed(false);
		bCanAttack = false;
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		GetCharacterMovement()->SetJumpAllowed(true);
		bCanAttack = true;
	}
}


