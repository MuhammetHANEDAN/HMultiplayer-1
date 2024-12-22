// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageIndicatorWidgetComponent.h"

#include "Components/TextBlock.h"
#include "HANEDANRPG3/HUD/DamageIndicator.h"


void UDamageIndicatorWidgetComponent::DestroyThisComp()
{
	DestroyComponent(true);
}

void UDamageIndicatorWidgetComponent::PlayNormalDamageWidgetAnim(uint16 DamageAmount)
{
	DamageIndicatorWidget = Cast<UDamageIndicator>(GetUserWidgetObject());
	if(DamageIndicatorWidget)
	{
		FString DamageString =FString::FromInt(DamageAmount);
		DamageIndicatorWidget->DamageText->SetText(FText::FromString(DamageString));
		DamageIndicatorWidget->PlayAnimation(DamageIndicatorWidget->NormalDamage);
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("damageind daha oulşturulmadı"));
	}
}

void UDamageIndicatorWidgetComponent::PlayShieldDamageWidgetAnim(uint16 DamageAmount)
{
	DamageIndicatorWidget = Cast<UDamageIndicator>(GetUserWidgetObject());
	if(DamageIndicatorWidget)
	{
		FString DamageString =FString::FromInt(DamageAmount);
		DamageIndicatorWidget->DamageText->SetText(FText::FromString(DamageString));
		DamageIndicatorWidget->PlayAnimation(DamageIndicatorWidget->ShieldDamage);
	}
}

void UDamageIndicatorWidgetComponent::PlayBurnDamageWidgetAnim(uint16 DamageAmount)
{
	DamageIndicatorWidget = Cast<UDamageIndicator>(GetUserWidgetObject());
	if(DamageIndicatorWidget)
	{
		FString DamageString =FString::FromInt(DamageAmount);
		DamageIndicatorWidget->DamageText->SetText(FText::FromString(DamageString));
		DamageIndicatorWidget->PlayAnimation(DamageIndicatorWidget->FireDamage);
	}
}

void UDamageIndicatorWidgetComponent::PlayFrozeDamageWidgetAnim(uint16 DamageAmount)
{
	DamageIndicatorWidget = Cast<UDamageIndicator>(GetUserWidgetObject());
	if(DamageIndicatorWidget)
	{
		FString DamageString =FString::FromInt(DamageAmount);
		DamageIndicatorWidget->DamageText->SetText(FText::FromString(DamageString));
		DamageIndicatorWidget->PlayAnimation(DamageIndicatorWidget->FrostDamage);
	}
}

void UDamageIndicatorWidgetComponent::DelayedPlayNormalDamageWidgetAnim()
{
	DamageIndicatorWidget = Cast<UDamageIndicator>(GetUserWidgetObject());
	if(DamageIndicatorWidget)
	{
		FString DamageString =FString::FromInt(10);
		DamageIndicatorWidget->DamageText->SetText(FText::FromString(DamageString));
		DamageIndicatorWidget->PlayAnimation(DamageIndicatorWidget->NormalDamage);
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("damageind daha oulşturulmadı"));
	}
}
