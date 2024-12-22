// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_SetCharacterActionStateStruct.h"

#include "HANEDANRPG3/Characters/HCharacter.h"

void UAN_SetCharacterActionStateStruct::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if(MeshComp->GetOwner()->HasAuthority())
	{
		if(AHCharacter* HCharacter = Cast<AHCharacter>(MeshComp->GetOwner()))
		{
			HCharacter->Server_SetCurrentCharacterActionStateStruct(ChangeValue,bNewValue);
		}
	}
}
