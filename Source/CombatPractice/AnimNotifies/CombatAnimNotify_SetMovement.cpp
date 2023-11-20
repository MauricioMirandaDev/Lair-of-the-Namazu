
#include "CombatAnimNotify_SetMovement.h"
#include "CombatPractice/Characters/CombatCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Set default values
UCombatAnimNotify_SetMovement::UCombatAnimNotify_SetMovement()
{
	NotifyColor = FColor(100, 255, 100); 
	bStopMovement = false; 
}

// Update the walk speed of this character 
void UCombatAnimNotify_SetMovement::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (OwnerReference)
		OwnerReference->UpdateMovement(bStopMovement);
}
