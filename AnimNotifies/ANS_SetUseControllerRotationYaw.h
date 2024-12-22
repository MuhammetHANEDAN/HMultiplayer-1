// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_SetUseControllerRotationYaw.generated.h"

/**
 * 
 */
UCLASS()
class HANEDANRPG3_API UANS_SetUseControllerRotationYaw : public UAnimNotifyState
{
	GENERATED_BODY()

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bNewValue = true;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bOutValue = false;
	
};
