
#include "AttackAnimation.h"

FAttackAnimation::FAttackAnimation()
{
	Animation = nullptr;
	AttackType = EAttackType::ATTACK_None;
	DamageAmount = 0.0f;
	ForwardThrustStrength = 0.0f;
	ForwardKnockbackStrength = 0.0f;
	VerticalKnockbackStrength = 0.0f;
}

FAttackAnimation::FAttackAnimation(UAnimMontage* InAnimMontage, EAttackType InAttackType, float InDamageAmount, float InForwardThrust, float InKnockback, float InVerticalKnockback)
{
	Animation = InAnimMontage;
	AttackType = InAttackType;
	DamageAmount = InDamageAmount;
	ForwardThrustStrength = InForwardThrust;
	ForwardKnockbackStrength = InKnockback;
	VerticalKnockbackStrength = InVerticalKnockback;
}
