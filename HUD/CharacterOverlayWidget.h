// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterOverlayWidget.generated.h"

class UImage;
class UProgressBar;
/**
 * 
 */
UCLASS()
class HANEDANRPG3_API UCharacterOverlayWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	
	UPROPERTY(meta=(BindWidget))
	UProgressBar* HPBar;

	UPROPERTY(meta=(BindWidget))
	UProgressBar* MPBar;

	UPROPERTY(meta=(BindWidget))
	UProgressBar* ShieldBar;

	UPROPERTY(meta=(BindWidget))
	UImage* SilenceImage;

	UPROPERTY(meta=(BindWidget))
	UImage* StunImage;
	
	UPROPERTY(meta=(BindWidget))
	UImage* SlowImage;

	UPROPERTY(meta=(BindWidget))
	UImage* BurnImage;

	UPROPERTY(meta=(BindWidget))
	UImage* FrozeImage;
	
};
