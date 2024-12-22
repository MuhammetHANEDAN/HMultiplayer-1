// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_Finisher.generated.h"

/**
 * 
 */
UCLASS()
class HANEDANRPG3_API UAN_Finisher : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bForward = true;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float ImpulseScale = 1;
	
};
