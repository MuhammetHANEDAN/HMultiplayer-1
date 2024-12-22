// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "HANEDANRPG3/Enums/EnumsType.h"
#include "AN_SetCharacterActionStateStruct.generated.h"

/**
 * 
 */
UCLASS()
class HANEDANRPG3_API UAN_SetCharacterActionStateStruct : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ECAS ChangeValue;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bNewValue = true;
	
};
