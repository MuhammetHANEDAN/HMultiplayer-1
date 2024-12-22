// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageIndicatorWidgetComponent.generated.h"

class UDamageIndicator;
/**
 * 
 */
UCLASS()
class HANEDANRPG3_API UDamageIndicatorWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()


	
public:

	void DestroyThisComp();

	UPROPERTY()
	UDamageIndicator* DamageIndicatorWidget;
	
	void PlayNormalDamageWidgetAnim(uint16 DamageAmount);
	void DelayedPlayNormalDamageWidgetAnim();
	void PlayShieldDamageWidgetAnim(uint16 DamageAmount);
	void PlayBurnDamageWidgetAnim(uint16 DamageAmount);
	void PlayFrozeDamageWidgetAnim(uint16 DamageAmount);
	
	
};
