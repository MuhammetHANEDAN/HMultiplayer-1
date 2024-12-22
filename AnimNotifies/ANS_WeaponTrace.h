// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_WeaponTrace.generated.h"

class AHCharacter;
/**
 * 
 */
UCLASS()
class HANEDANRPG3_API UANS_WeaponTrace : public UAnimNotifyState
{
	GENERATED_BODY()

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY()
	AHCharacter* HCharacter;

public:

	UPROPERTY(EditAnywhere,Category="Properties")
	bool bFirstWeapon = false;

	UPROPERTY(EditAnywhere,Category="Properties")
	bool bSecondWeapon = false; 

	





	
};
