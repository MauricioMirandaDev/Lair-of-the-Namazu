
#include "CombatAnimNotify_SetMovement.h"
#include "CombatPractice/Characters/CombatCharacter.h"

// Set default values
UCombatAnimNotify_SetMovement::UCombatAnimNotify_SetMovement()
{
	NotifyColor = FColor(100, 255, 100); 
	NewMovement = EMovementMode::MOVE_Walking; 
}

// Set the owner's movement mode to a new value
void UCombatAnimNotify_SetMovement::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (OwnerReference)
		OwnerReference->GetCharacterMovement()->SetMovementMode(NewMovement);
}
