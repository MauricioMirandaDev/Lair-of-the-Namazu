
#include "CombatAnimNotify_AfterDeath.h"
#include "CombatPractice/Characters/EnemyCharacter.h"

UCombatAnimNotify_AfterDeath::UCombatAnimNotify_AfterDeath()
{
	NotifyColor = FColor(255, 50, 50);
}

void UCombatAnimNotify_AfterDeath::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OwnerReference))
		Enemy->AfterDeathEffects();
}
