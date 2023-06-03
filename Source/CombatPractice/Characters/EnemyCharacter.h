
#pragma once

#include "CoreMinimal.h"
#include "CombatCharacter.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class COMBATPRACTICE_API AEnemyCharacter : public ACombatCharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

};
