// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_SetActionStateStruct.h"

void UANS_SetActionStateStruct::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                            float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if(MeshComp->GetOwner()->HasAuthority())
	{
		HCharacter = Cast<AHCharacter>(MeshComp->GetOwner());
		if(HCharacter && HCharacter->HasAuthority())
		{
			HCharacter->Server_SetCurrentCharacterActionStateStruct(ChangeValue,bNewValue);
		}
		
	}
}

void UANS_SetActionStateStruct::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	
	HCharacter = Cast<AHCharacter>(MeshComp->GetOwner()); // castlemezsek büyük sorun çıkıyor sebebini çözemedim
	if(HCharacter && HCharacter->HasAuthority())
	{
		HCharacter->Server_SetCurrentCharacterActionStateStruct(ChangeValue,bOutValue);
	}
	
}
