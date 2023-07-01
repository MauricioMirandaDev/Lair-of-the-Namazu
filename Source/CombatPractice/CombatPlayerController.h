
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CombatPlayerController.generated.h"

UCLASS()
class COMBATPRACTICE_API ACombatPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupInputComponent() override;

private:
	// Functions for movement
	void MoveForward(float Scale);

	void MoveRight(float Scale);

	// Functions for player's attacks
	void CallLightAttack();

	class APlayerCharacter* Player;
};
