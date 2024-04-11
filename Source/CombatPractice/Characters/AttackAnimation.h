
#pragma once

#include "CoreMinimal.h"
#include "AttackAnimation.generated.h"

class UAnimMontage;

UENUM(BlueprintType)
enum class EAttackType : uint8 {
	ATTACK_None UMETA(DisplayName = "No Attack"),
	ATTACK_Normal UMETA(DisplayName = "Nomral Attack"),
	ATTACK_Heavy UMETA(DisplayName = "Heavy Attack"),
	ATTACK_Stun UMETA(DisplayName = "Stun Attack"),
	ATTACK_Trip UMETA(DisplayName = "Trip Attack"), 
};

USTRUCT(BlueprintType)
struct FAttackAnimation
{
	GENERATED_BODY()

	FAttackAnimation(); 

	FAttackAnimation(UAnimMontage* InAnimMontage, EAttackType InAttackType, float InDamageAmount, float InForwardThrust, float InKnockback, float InVerticalKnockback);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* Animation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAttackType AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ForwardThrustStrength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ForwardKnockbackStrength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float VerticalKnockbackStrength;
};