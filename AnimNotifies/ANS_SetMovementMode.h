// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_SetMovementMode.generated.h"

class AHCharacter;
/**
 * 
 */
UCLASS()
class HANEDANRPG3_API UANS_SetMovementMode : public UAnimNotifyState
{
	GENERATED_BODY()

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY()
	AHCharacter* HCharacter;

public:

	/**
	 * 
	 */

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TEnumAsByte<EMovementMode> NewMovementMode;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TEnumAsByte<EMovementMode> OutMovementMode;
	
};
