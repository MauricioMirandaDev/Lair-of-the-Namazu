
#include "CombatAnimNotify_RotateWeapon.h"
#include "CombatPractice/Actors/Weapon.h"
#include "CombatPractice/Characters/CombatCharacter.h"

// Set default values
UCombatAnimNotify_RotateWeapon::UCombatAnimNotify_RotateWeapon()
{
	NotifyColor = FColor(255, 100, 255);
	NewRotation = FRotator(0.0f, 0.0f, 0.0f);
}

// Update the rotation of the owner's weapon when playing attack animation
void UCombatAnimNotify_RotateWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (OwnerReference)
		OwnerReference->GetWeapon()->SetActorRelativeRotation(NewRotation, true);
}
