// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_SetUseControllerRotationYaw.h"

#include "Components/CapsuleComponent.h"
#include "HANEDANRPG3/Characters/HCharacter.h"

void UANS_SetUseControllerRotationYaw::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                   float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if(MeshComp->GetOwner() == nullptr) return;
	if(AHCharacter* HCharacter = Cast<AHCharacter>(MeshComp->GetOwner()))
	{
		HCharacter->bUseControllerRotationYaw = bNewValue;
	}
}

void UANS_SetUseControllerRotationYaw::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	
	if(MeshComp->GetOwner() == nullptr) return;
	if(AHCharacter* HCharacter = Cast<AHCharacter>(MeshComp->GetOwner()))
	{
		HCharacter->bUseControllerRotationYaw = bOutValue;

		HCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn,ECR_Block);
		HCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1,ECR_Block);
		HCharacter->GetMesh()->SetCollisionResponseToChannel(ECC_Pawn,ECR_Block);
		HCharacter->GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel1,ECR_Block);
	}
}
