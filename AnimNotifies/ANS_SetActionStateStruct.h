// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "HANEDANRPG3/Characters/HCharacter.h"
#include "ANS_SetActionStateStruct.generated.h"

struct FCharacterActionStateStruct;
/**
 * 
 */
UCLASS()
class HANEDANRPG3_API UANS_SetActionStateStruct : public UAnimNotifyState
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
	ECAS ChangeValue;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bNewValue = true;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bOutValue = false;

	
	
};
