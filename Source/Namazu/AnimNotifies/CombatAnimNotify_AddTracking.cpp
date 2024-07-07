
#include "CombatAnimNotify_AddTracking.h"
#include "Namazu/Characters/CombatCharacter.h"
#include "Namazu/Characters/EnemyCharacter.h"

// Sets default values
UCombatAnimNotify_AddTracking::UCombatAnimNotify_AddTracking()
{
	NotifyColor = FColor(200, 0, 0);
}

// Add tracking to an enemy's attack animation
void UCombatAnimNotify_AddTracking::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OwnerReference))
		Enemy->AddTrackingToAttack();
}
