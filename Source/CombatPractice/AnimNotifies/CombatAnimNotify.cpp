
#include "CombatAnimNotify.h"
#include "CombatPractice/Characters/BaseCharacter.h"

UCombatAnimNotify::UCombatAnimNotify()
{
	PlayerRef = nullptr; 
}

void UCombatAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp && Animation)
	{
		if (MeshComp->GetOwner()->IsA(ABaseCharacter::StaticClass()))
			PlayerRef = Cast<ABaseCharacter>(MeshComp->GetOwner());
	}
}