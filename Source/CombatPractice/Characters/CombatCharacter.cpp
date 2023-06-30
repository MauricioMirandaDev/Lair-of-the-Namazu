
#include "CombatCharacter.h"
#include "CombatPractice/Actors/Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/DamageType.h"

// Sets default values
ACombatCharacter::ACombatCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	SetCanBeDamaged(true);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	WeaponClass = nullptr;
	HitAnimation = nullptr;
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

	if (CurrentHealth <= 0.0f)
		OnDeath();
}

// Called when character runs out of health
void ACombatCharacter::OnDeath()
{
	
}