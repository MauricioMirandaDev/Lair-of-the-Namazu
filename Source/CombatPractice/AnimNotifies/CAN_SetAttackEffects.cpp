
#include "CAN_SetAttackEffects.h"
#include "CombatPractice/Characters/BaseCharacter.h"

UCAN_SetAttackEffects::UCAN_SetAttackEffects()
{
	NotifyColor = FColor(255, 100, 100);
	bStartOfAttack = false; 
}

void UCAN_SetAttackEffects::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (PlayerRef)
		PlayerRef->AttackEffects(bStartOfAttack);
}
