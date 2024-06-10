
#include "CombatAnimNotify_UpdateRopeState.h"
#include "Namazu/Characters/CombatCharacter.h"
#include "Namazu/Characters/PlayerCharacter.h"

// Sets default values
UCombatAnimNotify_UpdateRopeState::UCombatAnimNotify_UpdateRopeState()
{
	NotifyColor = FColor(175, 189, 10);
}

// Update the state of the player's rope
void UCombatAnimNotify_UpdateRopeState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OwnerReference))
		Player->GetGrappleComponent()->SetRopeState(NewState);
}
