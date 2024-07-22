
#include "CombatAnimNotify_FireWeapon.h"
#include "Namazu/Actors/Weapon.h"
#include "Namazu/Actors/LongRangeWeapon.h"
#include "Namazu/Characters/CombatCharacter.h"

UCombatAnimNotify_FireWeapon::UCombatAnimNotify_FireWeapon()
{
	NotifyColor = FColor(180, 150, 20);
}

void UCombatAnimNotify_FireWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (OwnerReference)
	{
		ALongRangeWeapon* RangeWeapon = Cast<ALongRangeWeapon>(OwnerReference->GetWeapon());

		if (RangeWeapon)
			RangeWeapon->FireWeapon();
	}
}
