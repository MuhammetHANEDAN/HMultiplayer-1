// Fill out your copyright notice in the Description page of Project Settings.


#include "SetCharacterStatesAnimNotState.h"

#include "HANEDANRPG3/Characters/HCharacter.h"
#include "HANEDANRPG3/Components/CombatComponent.h"

void USetCharacterStatesAnimNotState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                  float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if(MeshComp->GetOwner()->HasAuthority())
	{
		HCharacter = Cast<AHCharacter>(MeshComp->GetOwner());
		if(HCharacter && HCharacter->HasAuthority())
		{
			if(HCharacter->CurrentCharacterState != NewCharacterState)
			{
				HCharacter->CurrentCharacterState = NewCharacterState;	
			}
		}
	}
}

void USetCharacterStatesAnimNotState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	
	if(HCharacter && HCharacter->HasAuthority())
	{
		if(NewCharacterState == ECharacterState::ECS_Blocking && HCharacter->CurrentCharacterState == ECharacterState::ECS_Dashing)
		{
			return;
		}
		/*if(NewCharacterState == ECharacterState::ECS_Blocking && HCharacter->CurrentCharacterState == ECharacterState::ECS_Attacking)
		{
			return;
		}*/
		HCharacter->CurrentCharacterState = EndCharacterState;

		/** karakter skill attı ve skill atarken bu anim bir şekilde interupt edilmişse bu,
		 * durumda destroylanması istenen skill actorler destroylanmalı
		 * */
		if(NewCharacterState == ECharacterState::ECS_Casting)
		{
			HCharacter->CombatComponent->DestroyAttachedAllSkillActors();
		}
	}
}


