// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HANEDANRPG3/Enums/EnumsType.h"
#include "UObject/Interface.h"
#include "HitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HANEDANRPG3_API IHitInterface
{
	GENERATED_BODY()

public:

	virtual void GetHit(const float Damage, const FVector& HitLocation, const EHitType HitType, const AActor* DamageCauserActor) = 0;

	virtual void ActivateLockOnIcon() = 0;
	virtual void DeactivateLockOnIcon() = 0;

	virtual void Interface_BrokeInvisibility() = 0;
	
};
