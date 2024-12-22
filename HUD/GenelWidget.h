// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GenelWidget.generated.h"

class UImage;
class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class HANEDANRPG3_API UGenelWidget : public UUserWidget
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
	UImage* Skill1Image;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* Skill1Text;

	UPROPERTY(meta=(BindWidget))
	UImage* Skill2Image;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* Skill2Text;

	UPROPERTY(meta=(BindWidget))
	UImage* Skill3Image;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* Skill3Text;

	UPROPERTY(meta=(BindWidget))
	UImage* Skill4Image;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* Skill4Text;

	UPROPERTY(meta=(BindWidget))
	UImage* SkillEImage;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* SkillEText;

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
