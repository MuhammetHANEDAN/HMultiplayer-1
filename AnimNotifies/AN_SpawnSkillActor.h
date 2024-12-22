// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_SpawnSkillActor.generated.h"

class ASkillActor;
/**
 * 
 */
UCLASS()
class HANEDANRPG3_API UAN_SpawnSkillActor : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Properties")
	TSubclassOf<ASkillActor> SkillActorClass;
	
};
