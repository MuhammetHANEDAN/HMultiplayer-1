// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_SetCharacterState.h"

#include "HANEDANRPG3/Characters/HCharacter.h"

void UAN_SetCharacterState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	if(MeshComp->GetOwner()->HasAuthority())
	{
		AHCharacter* HCharacter = Cast<AHCharacter>(MeshComp->GetOwner());
		if(HCharacter && HCharacter->HasAuthority())
		{
			
			HCharacter->CurrentCharacterState = NewCharacterState;
		}
	}
}
