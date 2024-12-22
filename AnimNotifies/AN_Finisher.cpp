// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_Finisher.h"

#include "HANEDANRPG3/Characters/HCharacter.h"

void UAN_Finisher::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	if(AHCharacter* hactor = Cast<AHCharacter>(MeshComp->GetOwner()))
	{
		hactor->GetMesh()->GetAnimInstance()->StopAllMontages(1);
		if(hactor->HasAuthority())
		{
			hactor->EnableDieAndPhysicImpulseWhenOnFinisher(ImpulseScale,bForward);
		}
	}
}
