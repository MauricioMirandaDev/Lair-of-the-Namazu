
#include "CombatAnimNotify_ResetAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CombatPractice/AI/EnemyAIController.h"
#include "CombatPractice/Characters/CombatCharacter.h"
#include "CombatPractice/Characters/EnemyCharacter.h"
#include "CombatPractice/Characters/PlayerCharacter.h"

// Sets default values
UCombatAnimNotify_ResetAttack::UCombatAnimNotify_ResetAttack()
{
	NotifyColor = FColor(100, 255, 255);
}

// Reset the player's attack count back to 0 OR reset enemy's blackboard key
void UCombatAnimNotify_ResetAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation); 

	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OwnerReference))
		Player->AttackCount = 0;
	else if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OwnerReference))
		Enemy->AIController->GetBlackboardComponent()->ClearValue(TEXT("HitSuccessful"));
}
