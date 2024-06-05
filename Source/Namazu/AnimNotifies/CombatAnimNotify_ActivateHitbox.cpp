
#include "CombatAnimNotify_ActivateHitbox.h"
#include "Namazu/Actors/Weapon.h"
#include "Namazu/Characters/CombatCharacter.h"

// Sets default values
UCombatAnimNotify_ActivateHitbox::UCombatAnimNotify_ActivateHitbox()
{
	NotifyColor = FColor(100, 100, 255);
	bActivateHitbox = false; 
	NewBoxExtent = FVector(0.0f, 0.0f, 0.0f);
}

// Update the collsion preset of the weapon's hitbox
void UCombatAnimNotify_ActivateHitbox::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (OwnerReference)
		OwnerReference->GetWeapon()->UpdateHitbox(bActivateHitbox, NewBoxExtent);
}
