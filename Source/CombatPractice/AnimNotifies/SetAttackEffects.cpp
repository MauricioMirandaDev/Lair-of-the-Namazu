
#include "SetAttackEffects.h"
#include "CombatPractice/Characters/CombatCharacter.h"

// Set default values
USetAttackEffects::USetAttackEffects()
{
	NotifyColor = FColor(255, 100, 100);
	bStartOfAttack = false;
}

// Set attack effects based on whether the animation is starting or ending 
void USetAttackEffects::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (OwnerReference)
		OwnerReference->AttackEffects(bStartOfAttack);
}
