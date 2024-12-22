// Fill out your copyright notice in the Description page of Project Settings.


#include "ChooseClassWidget.h"

#include "HHUD.h"
#include "Components/Button.h"
#include "HANEDANRPG3/PlayerControllers/HPlayerController.h"

bool UChooseClassWidget::Initialize()
{
	HPlayerController = Cast<AHPlayerController>(GetOwningPlayer());
	
	if(HPlayerController)
	{
		HHUD = Cast<AHHUD>(HPlayerController->GetHUD());
	}
	
	return Super::Initialize();
}

void UChooseClassWidget::SetupChooseClassWidget()
{
	GButton->OnClicked.AddDynamic(this,&UChooseClassWidget::GButtonPressed);
	DButton->OnClicked.AddDynamic(this,&UChooseClassWidget::DButtonPressed);
	MButton->OnClicked.AddDynamic(this,&UChooseClassWidget::MButtonPressed);
}

void UChooseClassWidget::GButtonPressed()
{
	if(HPlayerController && HHUD)
	{
		HPlayerController->Server_RequestMatchGameModeSpawnCharacter(HPlayerController,FName("GreatSword"));
		
		FInputModeGameOnly InputModeGameOnly;
		HPlayerController->SetInputMode(InputModeGameOnly);
		HHUD->SetChooseClassWidgetVisibilityAndUIProps(false);
	}
}

void UChooseClassWidget::DButtonPressed()
{
	if(HPlayerController)
	{
		HPlayerController->Server_RequestMatchGameModeSpawnCharacter(HPlayerController,FName("DualSword"));
		
		FInputModeGameOnly InputModeGameOnly;
		HPlayerController->SetInputMode(InputModeGameOnly);
		HHUD->SetChooseClassWidgetVisibilityAndUIProps(false);
	}
}

void UChooseClassWidget::MButtonPressed()
{
	if(HPlayerController)
	{
		HPlayerController->Server_RequestMatchGameModeSpawnCharacter(HPlayerController,FName("Mage"));

		FInputModeGameOnly InputModeGameOnly;
		HPlayerController->SetInputMode(InputModeGameOnly);
		HHUD->SetChooseClassWidgetVisibilityAndUIProps(false);
	}
}
