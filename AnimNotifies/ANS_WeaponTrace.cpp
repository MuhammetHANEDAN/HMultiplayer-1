// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_WeaponTrace.h"

#include "HANEDANRPG3/Characters/HCharacter.h"
#include "HANEDANRPG3/Components/CombatComponent.h"
#include "HANEDANRPG3/Weapons/Weapon.h"

void UANS_WeaponTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if(MeshComp->GetOwner()->HasAuthority())
	{
		HCharacter = Cast<AHCharacter>(MeshComp->GetOwner());
		if(HCharacter)
		{
			if(bFirstWeapon)
			{
				if(HCharacter->CombatComponent->FirstWeapon == nullptr) return;
				HCharacter->CombatComponent->FirstWeapon->StartHitTrace();
			}
			if(bSecondWeapon)
			{
				if(HCharacter->CombatComponent->SecondWeapon == nullptr) return;
				HCharacter->CombatComponent->SecondWeapon->StartHitTrace();
			}
		}
	}
}

void UANS_WeaponTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if(HCharacter)
	{
		if(bFirstWeapon)
		{
			if(HCharacter->CombatComponent->FirstWeapon == nullptr) return;
			HCharacter->CombatComponent->FirstWeapon->EndHitTrace();
		}
		if(bSecondWeapon)
		{
			if(HCharacter->CombatComponent->SecondWeapon == nullptr) return;
			HCharacter->CombatComponent->SecondWeapon->EndHitTrace();
		}
	}
}
