// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HHUD.generated.h"

class UCombatComponent;
class AHCharacter;
class UGenelWidget;
class AHPlayerController;
class UChooseClassWidget;
/**
 * 
 */
UCLASS()
class HANEDANRPG3_API AHHUD : public AHUD
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	virtual void DrawHUD() override;

public:

	/** REFERENCES */

	UPROPERTY()
	AHPlayerController* HPlayerController;

	UPROPERTY()
	AHCharacter* OwnerHCharacter;

	UPROPERTY()
	UCombatComponent* OwnersCombatComponent;

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	TSubclassOf<UChooseClassWidget> ChooseClassWidgetClass;

	UPROPERTY()
	UChooseClassWidget* ChooseClassWidget;

	void AddChooseClassWidget();
	void SetChooseClassWidgetVisibilityAndUIProps(bool bVisible);

	void RequestPlayerControllerSpawnCharacter(const FName& CharacterClassName);

	/**
	 *  GENEL WIDGET
	 */

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	TSubclassOf<UGenelWidget> GreatSwordCharacterGenelWidgetClass;

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	TSubclassOf<UGenelWidget> DualSwordCharacterGenelWidgetClass;

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	TSubclassOf<UGenelWidget> MageCharacterGenelWidgetClass;

	UPROPERTY()
	UGenelWidget* GenelWidget;

	void InitializeGenelWidget();
	void DestroyGenelWidget();

	void GenelWidgetWorks(TSubclassOf<UGenelWidget> &WidgetClass);
	void SetgenelWidgetVisibility(bool bVisible);
	
	void SetGenelWidgetHPBar(float Percent);
	void SetGenelWidgetMPBar(float Percent);
	void SetGenelWidgetShieldBar(float Percent);
	
	
	/**
	 *  CROSSHAIR
	 */

	bool bCrosshairEnabled = false;

	UPROPERTY(EditDefaultsOnly, Category="Properties")
	UTexture2D* CrosshairsCenter;
	
	UPROPERTY(EditDefaultsOnly, Category="Properties")
	UTexture2D* CrosshairsLeft;

	UPROPERTY(EditDefaultsOnly, Category="Properties")
	UTexture2D* CrosshairsRight;

	UPROPERTY(EditDefaultsOnly, Category="Properties")
	UTexture2D* CrosshairsTop;

	UPROPERTY(EditDefaultsOnly, Category="Properties")
	UTexture2D* CrosshairsBottom;

	UPROPERTY(EditDefaultsOnly, Category="Properties")
	FLinearColor CrosshairsColor = FLinearColor::Green;
	
	void DrawCrosshair(UTexture2D* Texture, FVector2d ViewportCenter,FLinearColor Color);











	





	
	
};
