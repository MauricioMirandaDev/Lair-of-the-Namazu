
#include "CombatAnimNotify_GameOver.h"
#include "CombatPractice/CombatPracticeGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Set default values
UCombatAnimNotify_GameOver::UCombatAnimNotify_GameOver()
{
	NotifyColor = FColor(125, 125, 125);
}

// Declare a game over after the player loses all their health
void UCombatAnimNotify_GameOver::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (ACombatPracticeGameModeBase* GameMode = Cast<ACombatPracticeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
		GameMode->GameOver();
}
