
#include "CombatAnimNotify_ChangeState.h"
#include "CombatPractice/Characters/CombatCharacter.h"

// Sets default values
UCombatAnimNotify_ChangeState::UCombatAnimNotify_ChangeState()
{
	NotifyColor = FColor(100, 255, 255);
	NewState = ECombatState::COMBAT_Neutral; 
}

// Update this character's combat state
void UCombatAnimNotify_ChangeState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (OwnerReference)
		OwnerReference->CombatState = NewState; 
}
