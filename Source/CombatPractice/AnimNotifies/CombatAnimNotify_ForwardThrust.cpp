
#include "CombatAnimNotify_ForwardThrust.h"
#include "CombatPractice/Characters/CombatCharacter.h"

// Sets default values
UCombatAnimNotify_ForwardThrust::UCombatAnimNotify_ForwardThrust()
{
	NotifyColor = FColor(255, 100, 255);
}

// Peform a forward thrust during attack animation 
void UCombatAnimNotify_ForwardThrust::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation); 

	if (OwnerReference)
		OwnerReference->ForwardThrust(); 
}
