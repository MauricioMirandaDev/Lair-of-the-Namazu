
#include "CombatAnimNotify_AttachRope.h"
#include "CombatPractice/Characters/CombatCharacter.h"
#include "CombatPractice/Characters/PlayerCharacter.h"

// Sets default values
UCombatAnimNotify_AttachRope::UCombatAnimNotify_AttachRope()
{
	NotifyColor = FColor(255, 200, 100);
	bShouldAttach = false; 
}

// Attach the player's rope to a grapple point
void UCombatAnimNotify_AttachRope::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation); 

	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OwnerReference))
		Player->SetRopeAttached(bShouldAttach);
}
