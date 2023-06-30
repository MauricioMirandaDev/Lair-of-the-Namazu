
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "CombatDamageType.generated.h"


UCLASS()
class COMBATPRACTICE_API UCombatDamageType : public UDamageType
{
	GENERATED_BODY()
	
public:
	// Set default values
	UCombatDamageType();

	// Amount of damage to apply to hit actor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Values")
	float Amount;
};
