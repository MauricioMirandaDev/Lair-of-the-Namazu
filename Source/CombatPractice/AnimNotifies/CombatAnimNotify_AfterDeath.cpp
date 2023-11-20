
#include "CombatAnimNotify_AfterDeath.h"
#include "CombatPractice/Characters/CombatCharacter.h"

UCombatAnimNotify_AfterDeath::UCombatAnimNotify_AfterDeath()
{
	NotifyColor = FColor(255, 255, 255);
}

void UCombatAnimNotify_AfterDeath::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (OwnerReference)
		OwnerReference->AfterDeath(); 
}
