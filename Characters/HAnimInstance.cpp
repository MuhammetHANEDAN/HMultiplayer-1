// Fill out your copyright notice in the Description page of Project Settings.


#include "HAnimInstance.h"

#include "HCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HANEDANRPG3/Components/CombatComponent.h"
#include "Kismet/KismetMathLibrary.h"


void UHAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	HCharacter = Cast<AHCharacter>(TryGetPawnOwner());
	if(HCharacter)
	{
		HCombatComponent = HCharacter->CombatComponent;
	}
	if(HCharacter && HCharacter->GetCharacterMovement())
	{
		CharacterMovementComponent = HCharacter->GetCharacterMovement();
	}
}

void UHAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(HCharacter == nullptr)
	{
		HCharacter = Cast<AHCharacter>(TryGetPawnOwner());
		if(HCharacter && HCharacter->GetCharacterMovement())
		{
			CharacterMovementComponent = HCharacter->GetCharacterMovement();
		}
	}
	if(HCharacter == nullptr) return;

	if(CharacterMovementComponent && HCombatComponent)
	{
		CurrentCharacterState = HCharacter->CurrentCharacterState;
		CurrentWeaponType = HCharacter->CurrentWeaponType;
		GroundSpeed=UKismetMathLibrary::VSizeXY(CharacterMovementComponent->Velocity);
		bTargeting = HCharacter->bTargeting;
		bIsFalling=CharacterMovementComponent->IsFalling();
		bIsAccelerating = CharacterMovementComponent->GetCurrentAcceleration().Size() > 0.f ? true : false ;
		Direction=CalculateDirection(CharacterMovementComponent->Velocity,HCharacter->GetActorRotation());
		bWeaponOnHand = HCombatComponent->bWeaponOnHand;
	}
}


