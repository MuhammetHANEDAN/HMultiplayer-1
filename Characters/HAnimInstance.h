// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HANEDANRPG3/Enums/EnumsType.h"
#include "HAnimInstance.generated.h"

class UCombatComponent;
class AHCharacter;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class HANEDANRPG3_API UHAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY(BlueprintReadWrite)
	AHCharacter* HCharacter;

	UPROPERTY(BlueprintReadWrite)
	UCombatComponent* HCombatComponent;

	UPROPERTY(BlueprintReadWrite,Category=Movement)
	UCharacterMovementComponent* CharacterMovementComponent;

	UPROPERTY(BlueprintReadWrite,Category=Movement)
	ECharacterState CurrentCharacterState;

	UPROPERTY(BlueprintReadWrite,Category=Movement)
	EWeaponType CurrentWeaponType;

	UPROPERTY(BlueprintReadWrite,Category=Movement)
	bool bTargeting;

	UPROPERTY(BlueprintReadWrite,Category=Movement)
	float GroundSpeed; 

	UPROPERTY(BlueprintReadWrite,Category=Movement)
	bool bIsFalling;

	UPROPERTY(BlueprintReadWrite,Category=Movement)
	bool bIsAccelerating;

	UPROPERTY(BlueprintReadWrite,Category=Movement)
	float Direction;

	UPROPERTY(BlueprintReadWrite,Category=Movement)
	bool bWeaponOnHand = false;
	



	
	
	
	
	
	
	
	
	
	
	
};
