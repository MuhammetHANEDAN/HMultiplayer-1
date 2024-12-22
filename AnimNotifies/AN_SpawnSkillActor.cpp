// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_SpawnSkillActor.h"

#include "HANEDANRPG3/Actors/SkillActor.h"

void UAN_SpawnSkillActor::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	if(MeshComp->GetOwner()->HasAuthority())
	{
		FActorSpawnParameters SParams;
		SParams.Owner = MeshComp->GetOwner();
		
		if(APawn* IPawn = Cast<APawn>(MeshComp->GetOwner()))
		{
			SParams.Instigator = IPawn;
		}
		
		GetWorld()->SpawnActor<ASkillActor>(SkillActorClass,FVector::Zero(),FRotator::ZeroRotator,SParams);
		UE_LOG(LogTemp,Error,TEXT(" hasauthority içinde spawn skill actor notify fire yedi "));
	}
}
