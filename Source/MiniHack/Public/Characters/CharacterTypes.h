#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon"),
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
};


UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	EDP_Alive UMETA(DisplayName = "Alive (no death pose)"),
	EDP_Death1 UMETA(DisplayName = "Death 1"),
	EDP_Death2 UMETA(DisplayName = "Death 2"),
	EDP_Death3 UMETA(DisplayName = "Death 3"),
};