
#include "CombatAnimNotify_SetAttack.h"
#include "CombatPractice/Characters/CombatCharacter.h"
#include "CombatPractice/Characters/PlayerCharacter.h"

// Set default values
UCombatAnimNotify_SetAttack::UCombatAnimNotify_SetAttack()
{
	NotifyColor = FColor(255, 100, 100);
	bAllowAttack = true; 
}

// Open or close this character's opportunity to start the next attack animation 
void UCombatAnimNotify_SetAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OwnerReference))
		Player->bCanAttack = bAllowAttack; 
}
