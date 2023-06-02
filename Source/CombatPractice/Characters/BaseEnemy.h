
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

class ABaseWeapon;

UCLASS()
class COMBATPRACTICE_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABaseWeapon> WeaponClass;

	ABaseWeapon* Weapon;
};
