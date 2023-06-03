
#include "CombatAnimNotify.h"
#include "CombatPractice/Characters/CombatCharacter.h"

// Set default values
UCombatAnimNotify::UCombatAnimNotify()
{
	OwnerReference = nullptr;
}

// Get a reference to the owning character
void UCombatAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp && Animation && MeshComp->GetOwner()->IsA(ACombatCharacter::StaticClass()))
		OwnerReference = Cast<ACombatCharacter>(MeshComp->GetOwner());	
}