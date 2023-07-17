
#include "CombatAnimNotify_ResetAttack.h"
#include "CombatPractice/Characters/CombatCharacter.h"
#include "CombatPractice/Characters/PlayerCharacter.h"

// Sets default values
UCombatAnimNotify_ResetAttack::UCombatAnimNotify_ResetAttack()
{
	NotifyColor = FColor(100, 255, 255);
}

// Reset the player's attack count back to 0
void UCombatAnimNotify_ResetAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation); 

	APlayerCharacter* Player = Cast<APlayerCharacter>(OwnerReference);

	if (Player)
		Player->AttackCount = 0; 
}
