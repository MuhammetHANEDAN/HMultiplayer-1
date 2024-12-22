// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChooseClassWidget.generated.h"

class AHHUD;
class AHPlayerController;
class UButton;
/**
 * 
 */
UCLASS()
class HANEDANRPG3_API UChooseClassWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY()
	AHPlayerController* HPlayerController;

	UPROPERTY()
	AHHUD* HHUD;

	virtual bool Initialize() override;
	void SetupChooseClassWidget();

	UPROPERTY(meta=(BindWidget))
	UButton* GButton;
	UFUNCTION()
	void GButtonPressed();

	UPROPERTY(meta=(BindWidget))
	UButton* DButton;
	UFUNCTION()
	void DButtonPressed();

	UPROPERTY(meta=(BindWidget))
	UButton* MButton;
	UFUNCTION()
	void MButtonPressed();
	




	
	
};
