
#include "CombatAnimNotify_ResetAttack.h"
#include "CombatPractice/Characters/CombatCharacter.h"

// Sets default values
UCombatAnimNotify_ResetAttack::UCombatAnimNotify_ResetAttack()
{
	NotifyColor = FColor(100, 255, 255);
}

// Reset the corresponding character's variables during combat
void UCombatAnimNotify_ResetAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation); 

	if (OwnerReference)
		OwnerReference->ResetAttack();
}
