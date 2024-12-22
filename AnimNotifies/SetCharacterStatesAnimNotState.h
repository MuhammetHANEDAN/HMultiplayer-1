// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "HANEDANRPG3/Enums/EnumsType.h"
#include "SetCharacterStatesAnimNotState.generated.h"

class AHCharacter;
/**
 * 
 */
UCLASS()
class HANEDANRPG3_API USetCharacterStatesAnimNotState : public UAnimNotifyState
{
	GENERATED_BODY()

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:

	UPROPERTY()
	AHCharacter* HCharacter;

	UPROPERTY(EditAnywhere)
	ECharacterState NewCharacterState;
	UPROPERTY(EditAnywhere)
	ECharacterState EndCharacterState;
	
};
