// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageIndicator.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class HANEDANRPG3_API UDamageIndicator : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta=(BindWidget))
	UTextBlock* DamageText;

	UPROPERTY(Transient, meta = (BindWidgetAnim), BlueprintReadOnly)
	UWidgetAnimation* NormalDamage;

	UPROPERTY(Transient, meta = (BindWidgetAnim), BlueprintReadOnly)
	UWidgetAnimation* ShieldDamage;

	UPROPERTY(Transient, meta = (BindWidgetAnim), BlueprintReadOnly)
	UWidgetAnimation* FireDamage;

	UPROPERTY(Transient, meta = (BindWidgetAnim), BlueprintReadOnly)
	UWidgetAnimation* FrostDamage;

	







	
};
