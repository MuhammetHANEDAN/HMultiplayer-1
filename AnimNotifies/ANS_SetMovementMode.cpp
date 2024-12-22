// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_SetMovementMode.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "HANEDANRPG3/Characters/HCharacter.h"

void UANS_SetMovementMode::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                       float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	HCharacter = Cast<AHCharacter>(MeshComp->GetOwner());
	if(HCharacter)
	{
		HCharacter->GetCharacterMovement()->SetMovementMode(NewMovementMode);
	}
}

void UANS_SetMovementMode::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	
	HCharacter = Cast<AHCharacter>(MeshComp->GetOwner()); 
	if(HCharacter)
	{
		HCharacter->GetCharacterMovement()->SetMovementMode(OutMovementMode);
	}
}
