#pragma once

#include "CoreMinimal.h"

/**
 * CHARACTER
 */

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unoccupied UMETA(DisplayName= "Unoccupied"),
	ECS_Equiping UMETA(DisplayName= "Equiping"),
	ECS_Attacking UMETA(DisplayName= "Attacking"),
	ECS_Casting UMETA(DisplayName= "Casting"),
	ECS_Dashing UMETA(DisplayName= "Dashing"),
	ECS_Blocking UMETA(DisplayName= "Blocking"),

	ECS_MAX UMETA(DisplayName = "Default MAX")
};

/**
 * WEAPON
 */

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_GreatSword UMETA(DisplayName= "GreatSword"),
	EWT_Staff UMETA(DisplayName= "Staff"),
	EWT_DualSword UMETA(DisplayName= "DualSword"),
	
	ECS_MAX UMETA(DisplayName = "Default MAX")
};


UENUM(BlueprintType)
enum class EHitType : uint8
{
	EHT_NormalHit UMETA(DisplayName= "Normal Hit"),
	EHT_BigHit UMETA(DisplayName= "Big Hit"),
	
	EHT_MAX UMETA(DisplayName = "Default MAX")
};

/**
 * ANS_Setactionstate Structta kullanmak için
 */
UENUM(BlueprintType)
enum class ECAS : uint8
{
	ECAS_Stunned UMETA(DisplayName= "Stunned"),
	ECAS_Silenced UMETA(DisplayName= "Silenced"),
	ECAS_Slowed UMETA(DisplayName= "Slowed"),
	ECAS_Frozen UMETA(DisplayName= "Frozen"),
	ECAS_Burning UMETA(DisplayName= "Burning"),
	ECAS_OnFall UMETA(DisplayName= "OnFall"),
	ECAS_BeingExecuting UMETA(DisplayName= "BeingExecuting"),
	ECAS_OnExecution UMETA(DisplayName= "OnExecution"),
	ECAS_OnSuperArmor UMETA(DisplayName= "OnSuperArmor"),
	
	ECAS_MAX UMETA(DisplayName = "Default MAX")
};
