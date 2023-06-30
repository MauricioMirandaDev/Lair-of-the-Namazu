
#include "CombatAnimNotify_ActivateHitbox.h"
#include "CombatPractice/Actors/Weapon.h"
#include "CombatPractice/Characters/CombatCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
UCombatAnimNotify_ActivateHitbox::UCombatAnimNotify_ActivateHitbox()
{
	NotifyColor = FColor(100, 100, 255);
	CollisionProfile = TEXT("NoCollision");
}

// Update the collsion preset of the weapon's hitbox 
void UCombatAnimNotify_ActivateHitbox::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (OwnerReference)
		OwnerReference->Weapon->Hitbox->SetCollisionProfileName(CollisionProfile);
}
