
#include "CombatAnimNotify_ActivateHitbox.h"
#include "CombatPractice/CombatDamageType.h"
#include "CombatPractice/Actors/Weapon.h"
#include "CombatPractice/Characters/CombatCharacter.h"
#include "CombatPractice/Characters/EnemyCharacter.h"
#include "CombatPractice/Characters/PlayerCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
UCombatAnimNotify_ActivateHitbox::UCombatAnimNotify_ActivateHitbox()
{
	NotifyColor = FColor(100, 100, 255);
	bActivateHitbox = false; 
	DamageClass = nullptr;
}

// Update the collsion preset of the weapon's hitbox and it's damage type
void UCombatAnimNotify_ActivateHitbox::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (bActivateHitbox)
	{
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(OwnerReference))
			Player->Weapon->UpdateHitbox(TEXT("PlayerWeapon"), DamageClass); 
		else if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OwnerReference))
			Enemy->Weapon->UpdateHitbox(TEXT("EnemyWeapon"), DamageClass); 
	}
	else if (OwnerReference)
		OwnerReference->Weapon->UpdateHitbox(TEXT("NoCollision"), DamageClass);
}
