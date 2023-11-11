
#pragma once

#include "CoreMinimal.h"
#include "AttackAnimation.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8 {
	ATTACK_None UMETA(DisplayName = "No Attack"),
	ATTACK_Normal UMETA(DisplayName = "Nomral Attack"),
	ATTACK_Heavy UMETA(DisplayName = "Heavy Attack"),
	ATTACK_Stun UMETA(DisplayName = "Stun Attack")
};

USTRUCT(BlueprintType)
struct FAttackAnimation
{
	GENERATED_BODY()

	FAttackAnimation(); 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UAnimMontage* Animation;

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