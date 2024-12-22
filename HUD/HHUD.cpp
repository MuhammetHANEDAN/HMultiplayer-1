// Fill out your copyright notice in the Description page of Project Settings.


#include "HHUD.h"

#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "HANEDANRPG3/Characters/HCharacter.h"
#include "HANEDANRPG3/Components/CombatComponent.h"
#include "HANEDANRPG3/HUD/ChooseClassWidget.h"
#include "HANEDANRPG3/HUD/GenelWidget.h"
#include "HANEDANRPG3/PlayerControllers/HPlayerController.h"

void AHHUD::BeginPlay()
{
	Super::BeginPlay();

	HPlayerController = Cast<AHPlayerController>(GetOwningPlayerController());
	
	AddChooseClassWidget();
	
}

void AHHUD::DrawHUD()
{
	Super::DrawHUD();

	if(bCrosshairEnabled)
	{
		FVector2d ViewPortSize;
		if(GEngine)
		{
			GEngine->GameViewport->GetViewportSize(ViewPortSize);
			const FVector2d ViewportCenter(ViewPortSize.X/2.f, ViewPortSize.Y/2.f);

			if(CrosshairsCenter)
			{
				DrawCrosshair(CrosshairsCenter,ViewportCenter,CrosshairsColor);
			}
			if(CrosshairsLeft)
			{
				DrawCrosshair(CrosshairsLeft,ViewportCenter,CrosshairsColor);
			}
			if(CrosshairsRight)
			{
				DrawCrosshair(CrosshairsRight,ViewportCenter,CrosshairsColor);
			}
			if(CrosshairsBottom)
			{
				DrawCrosshair(CrosshairsBottom,ViewportCenter,CrosshairsColor);
			}
			if(CrosshairsTop)
			{
				// UV kordinat olduğu için Y de yukarısı - oluyor
				DrawCrosshair(CrosshairsTop,ViewportCenter,CrosshairsColor);
			}
		}
	}
}

void AHHUD::DrawCrosshair(UTexture2D* Texture, FVector2d ViewportCenter,FLinearColor Color)
{
	const float TextureHeight = Texture->GetSizeX();
	const float TextureWidth = Texture->GetSizeY();

	const FVector2d TextureDrawPoint(ViewportCenter.X-(TextureWidth/2.f),ViewportCenter.Y-(TextureHeight/2.f));

	DrawTexture(Texture,TextureDrawPoint.X,TextureDrawPoint.Y,TextureWidth,TextureHeight,0,0,1,1,
		Color);
}

void AHHUD::RequestPlayerControllerSpawnCharacter(const FName& CharacterClassName)
{
	if(HPlayerController)
	{
		HPlayerController->Server_RequestMatchGameModeSpawnCharacter(HPlayerController,CharacterClassName);
	}
}

void AHHUD::InitializeGenelWidget() 
{
	if(OwnerHCharacter == nullptr) return;
	
	if(OwnerHCharacter->ActorHasTag(FName("GreatSwordCharacter")))
	{
		if(GreatSwordCharacterGenelWidgetClass)
		{
			GenelWidgetWorks(GreatSwordCharacterGenelWidgetClass);
		}
	}
	else if(OwnerHCharacter->ActorHasTag(FName("DualSwordCharacter")))
	{
		if(DualSwordCharacterGenelWidgetClass)
		{
			GenelWidgetWorks(DualSwordCharacterGenelWidgetClass);
		}
	}
	else if(OwnerHCharacter->ActorHasTag(FName("MageCharacter")))
	{
		if(MageCharacterGenelWidgetClass)
		{
			GenelWidgetWorks(MageCharacterGenelWidgetClass);
		}
	}
}

void AHHUD::GenelWidgetWorks(TSubclassOf<UGenelWidget>& WidgetClass)
{
	GenelWidget = CreateWidget<UGenelWidget>(GetOwningPlayerController(),WidgetClass);
	if(GenelWidget == nullptr) return;
	GenelWidget->AddToViewport();

	if(OwnersCombatComponent) // Oyuncu hile yapsa bile bir şey değişmez o yüzden direk client karakterden çekebilirz 
	{
		SetGenelWidgetHPBar(OwnersCombatComponent->CurrentHP/OwnersCombatComponent->MaxHP);
		SetGenelWidgetMPBar(OwnersCombatComponent->CurrentMP/OwnersCombatComponent->MaxMP);
		SetGenelWidgetShieldBar(OwnersCombatComponent->CurrentBlockAmount/OwnersCombatComponent->MaxBlockAmount);
	}
}

void AHHUD::SetgenelWidgetVisibility(bool bVisible)
{
	if(GenelWidget)
	{
		if(bVisible)
		{
			GenelWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			GenelWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AHHUD::DestroyGenelWidget()
{
	if(GenelWidget)
	{
		GenelWidget->RemoveFromParent();
	}
}

void AHHUD::SetGenelWidgetHPBar(float Percent)
{
	if(GenelWidget == nullptr || GenelWidget->HPBar == nullptr) return;
	GenelWidget->HPBar->SetPercent(Percent);
}

void AHHUD::SetGenelWidgetMPBar(float Percent)
{
	if(GenelWidget == nullptr || GenelWidget->MPBar == nullptr) return;
	GenelWidget->MPBar->SetPercent(Percent);
}

void AHHUD::SetGenelWidgetShieldBar(float Percent)
{
	if(GenelWidget == nullptr || GenelWidget->ShieldBar == nullptr) return;
	GenelWidget->ShieldBar->SetPercent(Percent);
}

void AHHUD::AddChooseClassWidget()
{
	if(ChooseClassWidgetClass && HPlayerController)
	{
		FInputModeUIOnly InputModeUIOnly;
		HPlayerController->SetInputMode(InputModeUIOnly);
		ChooseClassWidget = CreateWidget<UChooseClassWidget>(HPlayerController,ChooseClassWidgetClass);
		ChooseClassWidget->AddToViewport();
		ChooseClassWidget->SetupChooseClassWidget();
		HPlayerController->SetShowMouseCursor(true);
	}
}

void AHHUD::SetChooseClassWidgetVisibilityAndUIProps(bool bVisible)
{
	if(ChooseClassWidget)
	{
		UE_LOG(LogTemp,Error,TEXT("fonk çlaıştı"));
		if(bVisible)
		{
			UE_LOG(LogTemp,Error,TEXT("bvisible içine gelindi"));
			ChooseClassWidget->SetVisibility(ESlateVisibility::Visible);
			HPlayerController->SetShowMouseCursor(true);
			FInputModeGameAndUI InputModeGameAndUI;
			HPlayerController->SetInputMode(InputModeGameAndUI);
		}
		else
		{
			ChooseClassWidget->SetVisibility(ESlateVisibility::Hidden);
			HPlayerController->SetShowMouseCursor(false);
			FInputModeGameOnly InputModeGameOnly;
			HPlayerController->SetInputMode(InputModeGameOnly);
		}
	}
}

