
#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ECombatState : uint8 {
	COMBAT_Neutral UMETA(DisplayName = "Netural"),
	COMBAT_DamagedNormal UMETA(DisplayName = "Damaged Normal"),
	COMBAT_DamagedHeavy UMETA(DisplayName = "Damaged Heavy"),
	COMBAT_DamagedStun UMETA(DisplayName = "Damaged Stun"),
	COMBAT_AttackStartup UMETA(DisplayName = "Attack Startup"),
	COMBAT_AttackActive UMETA(DisplayName = "Attack Active"),
	COMBAT_AttackRecover UMETA(DisplayName = "Attack Recovery"),
	COMBAT_Dead UMETA(DisplayName = "Dead")
};
