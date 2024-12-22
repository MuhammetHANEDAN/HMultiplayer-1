// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayEffectsComponent.h"

#include "CombatComponent.h"
#include "Components/Image.h"
#include "HANEDANRPG3/Characters/HCharacter.h"
#include "HANEDANRPG3/HUD/CharacterOverlayWidget.h"
#include "HANEDANRPG3/HUD/GenelWidget.h"
#include "HANEDANRPG3/HUD/HHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"

UGamePlayEffectsComponent::UGamePlayEffectsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	
	
}

void UGamePlayEffectsComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UGamePlayEffectsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UGamePlayEffectsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UGamePlayEffectsComponent,bStunned);
	DOREPLIFETIME(UGamePlayEffectsComponent,bSilenced);
	DOREPLIFETIME(UGamePlayEffectsComponent,bSlowed);
	DOREPLIFETIME(UGamePlayEffectsComponent,bFrozen);
	DOREPLIFETIME(UGamePlayEffectsComponent,bBurning);
}

bool UGamePlayEffectsComponent::bCanEffectOwnerHCharacter() const
{
	if(OwnerHCharacter)
	{
		return(!OwnerHCharacter->CurrentCharacterActionStateStruct.bBeingExecuted &&
		!OwnerHCharacter->CurrentCharacterActionStateStruct.bOnExecution &&
		!OwnerHCharacter->CurrentCharacterActionStateStruct.bOnSuperArmor &&
		OwnerHCharacter->bVisible);
	}
	return false;
}

void UGamePlayEffectsComponent::EffectDamage(float Damage, bool bFire)
{
	if(OwnerCombatComponent)
	{
		OwnerCombatComponent->GetEffectDamage(Damage, bFire);
	}
}

void UGamePlayEffectsComponent::OnRep_Stunned()
{
	if(bStunned)
	{
		EnableStunParticle();
		EnableStunUIProps();
	}
	else
	{
		DisableStunParticle();
		DisableStunUIProps();
	}
}

void UGamePlayEffectsComponent::EnableStun(float Seconds)
{
	if(!bCanEffectOwnerHCharacter()) return;
	if(bStunned) return;
	
	NewActionStateStruct = OwnerHCharacter->CurrentCharacterActionStateStruct;
	NewActionStateStruct.bStunned = true;

	bStunned = true;

	OwnerHCharacter->CurrentCharacterActionStateStruct = NewActionStateStruct;
	
	GetWorld()->GetTimerManager().SetTimer(StunTimerHandle,this,&UGamePlayEffectsComponent::ClearStun,Seconds);

	EnableStunParticle();
	EnableStunUIProps();
}

void UGamePlayEffectsComponent::ClearStun()
{
	GetWorld()->GetTimerManager().ClearTimer(StunTimerHandle);

	NewActionStateStruct = OwnerHCharacter->CurrentCharacterActionStateStruct;
	NewActionStateStruct.bStunned = false;

	bStunned = false;

	OwnerHCharacter->CurrentCharacterActionStateStruct = NewActionStateStruct;

	DisableStunParticle();
	DisableStunUIProps();
}

void UGamePlayEffectsComponent::EnableStunParticle()
{
	if(StunParticleSystem && OwnerMeshComponent)
	{
		FVector Location = OwnerHCharacter->GetActorLocation() + OwnerHCharacter->GetActorUpVector()*45;
		StunParticleSystemComponent = UGameplayStatics::SpawnEmitterAttached(StunParticleSystem,OwnerMeshComponent,NAME_Name,Location);
	}
}

void UGamePlayEffectsComponent::DisableStunParticle()
{
	if(StunParticleSystemComponent)
	{
		StunParticleSystemComponent->DestroyComponent();
	}
}

void UGamePlayEffectsComponent::EnableStunUIProps()
{
	if(OwnerHCharacter->HHudReference)
	{
		if(OwnerHCharacter->HHudReference->GenelWidget)
		{
			OwnerHCharacter->HHudReference->GenelWidget->StunImage->SetRenderOpacity(1);
		}
	}
	else if(OwnerHCharacter->CharacterOverlayWidget)
	{
		OwnerHCharacter->CharacterOverlayWidget->StunImage->SetRenderOpacity(1);
	}
}

void UGamePlayEffectsComponent::DisableStunUIProps()
{
	if(OwnerHCharacter->HHudReference)
	{
		if(OwnerHCharacter->HHudReference->GenelWidget)
		{
			OwnerHCharacter->HHudReference->GenelWidget->StunImage->SetRenderOpacity(0);
		}
	}
	else if(OwnerHCharacter->CharacterOverlayWidget)
	{
		OwnerHCharacter->CharacterOverlayWidget->StunImage->SetRenderOpacity(0);
	}
}

void UGamePlayEffectsComponent::OnRep_Silenced()
{
	if(bSilenced)
	{
		EnableSilenceParticle();
		EnableSilenceUIProps();
	}
	else
	{
		DisableSilenceParticle();
		DisableSilenceUIProps();
	}
}

void UGamePlayEffectsComponent::EnableSilence(float Seconds)
{
	if(!bCanEffectOwnerHCharacter()) return;
	if(bSilenced) return;
	
	NewActionStateStruct = OwnerHCharacter->CurrentCharacterActionStateStruct;
	NewActionStateStruct.bSilenced = true;

	bSilenced = true;

	OwnerHCharacter->CurrentCharacterActionStateStruct = NewActionStateStruct;
	
	GetWorld()->GetTimerManager().SetTimer(SilenceTimerHandle,this,&UGamePlayEffectsComponent::ClearSilence,Seconds);

	EnableSilenceParticle();
	EnableSilenceUIProps();
}

void UGamePlayEffectsComponent::ClearSilence()
{
	GetWorld()->GetTimerManager().ClearTimer(SilenceTimerHandle);

	NewActionStateStruct = OwnerHCharacter->CurrentCharacterActionStateStruct;
	NewActionStateStruct.bSilenced = false;

	bSilenced = false;

	OwnerHCharacter->CurrentCharacterActionStateStruct = NewActionStateStruct;

	DisableSilenceParticle();
	DisableSilenceUIProps();
}

void UGamePlayEffectsComponent::EnableSilenceParticle()
{
	if(SilenceParticleSystem && OwnerMeshComponent)
	{
		SilenceParticleSystemComponent = UGameplayStatics::SpawnEmitterAttached(SilenceParticleSystem,OwnerMeshComponent);
	}
}

void UGamePlayEffectsComponent::DisableSilenceParticle()
{
	if(SlowParticleSystemComponent)
	{
		SlowParticleSystemComponent->DestroyComponent();
	}
}

void UGamePlayEffectsComponent::EnableSilenceUIProps()
{
	if(OwnerHCharacter->HHudReference)
	{
		if(OwnerHCharacter->HHudReference->GenelWidget)
		{
			OwnerHCharacter->HHudReference->GenelWidget->SilenceImage->SetRenderOpacity(1);
		}
	}
	else if(OwnerHCharacter->CharacterOverlayWidget)
	{
		OwnerHCharacter->CharacterOverlayWidget->SilenceImage->SetRenderOpacity(1);
	}
}

void UGamePlayEffectsComponent::DisableSilenceUIProps()
{
	if(OwnerHCharacter->HHudReference)
	{
		if(OwnerHCharacter->HHudReference->GenelWidget)
		{
			OwnerHCharacter->HHudReference->GenelWidget->SilenceImage->SetRenderOpacity(0);
		}
	}
	else if(OwnerHCharacter->CharacterOverlayWidget)
	{
		OwnerHCharacter->CharacterOverlayWidget->SilenceImage->SetRenderOpacity(0);
	}
}

void UGamePlayEffectsComponent::OnRep_Slowed()
{
	if(bSlowed)
	{
		EnableSlowParticle();
		EnableSlowUIProps();
	}
	else
	{
		DisableSlowParticle();
		DisableSlowUIProps();
	}
}

void UGamePlayEffectsComponent::EnableSlow(float Seconds)
{
	if(!bCanEffectOwnerHCharacter()) return;
	if(bSlowed) return;
	
	NewActionStateStruct = OwnerHCharacter->CurrentCharacterActionStateStruct;
	NewActionStateStruct.bSlowed = true;

	bSlowed = true;

	OwnerHCharacter->CurrentCharacterActionStateStruct = NewActionStateStruct;

	OwnerCombatComponent->SetCurrentSpeed(OwnerCombatComponent->SlowedSpeed);
	
	GetWorld()->GetTimerManager().SetTimer(SlowTimerHandle,this,&UGamePlayEffectsComponent::ClearSlow,Seconds);

	EnableSlowParticle();
	EnableSlowUIProps();
}

void UGamePlayEffectsComponent::ClearSlow()
{
	GetWorld()->GetTimerManager().ClearTimer(SlowTimerHandle);

	NewActionStateStruct = OwnerHCharacter->CurrentCharacterActionStateStruct;
	NewActionStateStruct.bSlowed = false;

	bSlowed = false;

	OwnerHCharacter->CurrentCharacterActionStateStruct = NewActionStateStruct;
	
	OwnerCombatComponent->SetCurrentSpeed(OwnerCombatComponent->NormalSpeed);

	DisableSlowParticle();
	DisableSlowUIProps();
}

void UGamePlayEffectsComponent::EnableSlowParticle()
{
	if(SlowParticleSystem&& OwnerMeshComponent)
	{
		SlowParticleSystemComponent = UGameplayStatics::SpawnEmitterAttached(SlowParticleSystem,OwnerMeshComponent);
	}
}

void UGamePlayEffectsComponent::DisableSlowParticle()
{
	if(SlowParticleSystemComponent)
	{
		SlowParticleSystemComponent->DestroyComponent();
	}
}

void UGamePlayEffectsComponent::EnableSlowUIProps()
{
	if(OwnerHCharacter->HHudReference)
	{
		if(OwnerHCharacter->HHudReference->GenelWidget)
		{
			OwnerHCharacter->HHudReference->GenelWidget->SlowImage->SetRenderOpacity(1);
		}
	}
	else if(OwnerHCharacter->CharacterOverlayWidget)
	{
		OwnerHCharacter->CharacterOverlayWidget->SlowImage->SetRenderOpacity(1);
	}
}

void UGamePlayEffectsComponent::DisableSlowUIProps()
{
	if(OwnerHCharacter->HHudReference)
	{
		if(OwnerHCharacter->HHudReference->GenelWidget)
		{
			OwnerHCharacter->HHudReference->GenelWidget->SlowImage->SetRenderOpacity(0);
		}
	}
	else if(OwnerHCharacter->CharacterOverlayWidget)
	{
		OwnerHCharacter->CharacterOverlayWidget->SlowImage->SetRenderOpacity(0);
	}
}

void UGamePlayEffectsComponent::OnRep_Frozen()
{
	if(bFrozen)
	{
		EnableFrozeParticle();
		EnableFrozeUIProps();
	}
	else
	{
		DisableFrozeParticle();
		DisableFrozeUIProps();
	}
}

void UGamePlayEffectsComponent::EnableFroze(float Seconds, float FrequencySeconds, float FrequencyDamage)
{
	if(!bCanEffectOwnerHCharacter()) return;
	if(bFrozen) return;
	
	FrozingDamage = FrequencyDamage;

	bFrozen =true;

	NewActionStateStruct = OwnerHCharacter->CurrentCharacterActionStateStruct; // daha optimize bir yol olabilirmi ?
	NewActionStateStruct.bFrozed = true;
	OwnerHCharacter->CurrentCharacterActionStateStruct = NewActionStateStruct;
	
	GetWorld()->GetTimerManager().SetTimer(FrozeTimerHandle,this,&UGamePlayEffectsComponent::ClearFroze,Seconds);
	
	GetWorld()->GetTimerManager().SetTimer(FrozeFrequencyTimerHandle,this,&ThisClass::FrozeDamage,FrequencySeconds,true);

	EnableFrozeParticle();
	EnableFrozeUIProps();
}

void UGamePlayEffectsComponent::ClearFroze()
{
	bFrozen = false;

	NewActionStateStruct = OwnerHCharacter->CurrentCharacterActionStateStruct; // daha optimize bir yol olabilirmi ?
	NewActionStateStruct.bFrozed = false;
	OwnerHCharacter->CurrentCharacterActionStateStruct = NewActionStateStruct;
	
	GetWorld()->GetTimerManager().ClearTimer(FrozeTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(FrozeFrequencyTimerHandle);

	DisableFrozeParticle();
	DisableFrozeUIProps();
}

void UGamePlayEffectsComponent::EnableFrozeParticle()
{
	if(FrozeParticleSystem && OwnerMeshComponent)
	{
		FrozeParticleSystemComponent = UGameplayStatics::SpawnEmitterAttached(FrozeParticleSystem,OwnerMeshComponent);
	}
}

void UGamePlayEffectsComponent::DisableFrozeParticle()
{
	if(FrozeParticleSystemComponent)
	{
		FrozeParticleSystemComponent->DestroyComponent();
	}
}

void UGamePlayEffectsComponent::EnableFrozeUIProps()
{
	if(OwnerHCharacter->HHudReference)
	{
		if(OwnerHCharacter->HHudReference->GenelWidget)
		{
			OwnerHCharacter->HHudReference->GenelWidget->FrozeImage->SetRenderOpacity(1);
		}
	}
	else if(OwnerHCharacter->CharacterOverlayWidget)
	{
		OwnerHCharacter->CharacterOverlayWidget->FrozeImage->SetRenderOpacity(1);
	}
}

void UGamePlayEffectsComponent::DisableFrozeUIProps()
{
	if(OwnerHCharacter->HHudReference)
	{
		if(OwnerHCharacter->HHudReference->GenelWidget)
		{
			OwnerHCharacter->HHudReference->GenelWidget->FrozeImage->SetRenderOpacity(0);
		}
	}
	else if(OwnerHCharacter->CharacterOverlayWidget)
	{
		OwnerHCharacter->CharacterOverlayWidget->FrozeImage->SetRenderOpacity(0);
	}
}

void UGamePlayEffectsComponent::FrozeDamage()
{
	EffectDamage(FrozingDamage, false);
}

void UGamePlayEffectsComponent::OnRep_Burning()
{
	if(!bCanEffectOwnerHCharacter()) return;
	if(bBurning)
	{
		EnableBurningParticle();
		EnableBurnUIProps();
	}
	else
	{
		DisableBurningParticle();
		DisableBurnUIProps();
	}
}

void UGamePlayEffectsComponent::EnableBurn(float Seconds, float FrequencySeconds, float FrequencyDamage)
{
	if(bBurning) return;
	
	BurningDamage = FrequencyDamage;

	bBurning = true;

	NewActionStateStruct = OwnerHCharacter->CurrentCharacterActionStateStruct; // daha optimize bir yol olabilirmi ?
	NewActionStateStruct.bBurning = true;
	OwnerHCharacter->CurrentCharacterActionStateStruct = NewActionStateStruct;
	
	GetWorld()->GetTimerManager().SetTimer(BurnTimerHandle,this,&UGamePlayEffectsComponent::ClearBurn,Seconds);
	
	GetWorld()->GetTimerManager().SetTimer(BurnFrequencyTimerHandle,this,&ThisClass::BurnDamage,FrequencySeconds,true);

	EnableBurningParticle();
	EnableBurnUIProps();
}

void UGamePlayEffectsComponent::ClearBurn()
{
	bBurning = false;

	NewActionStateStruct = OwnerHCharacter->CurrentCharacterActionStateStruct; // daha optimize bir yol olabilirmi ?
	NewActionStateStruct.bBurning = false;
	OwnerHCharacter->CurrentCharacterActionStateStruct = NewActionStateStruct;
	
	GetWorld()->GetTimerManager().ClearTimer(BurnTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(BurnFrequencyTimerHandle);

	DisableBurningParticle();
	DisableBurnUIProps();
}

void UGamePlayEffectsComponent::BurnDamage()
{
	EffectDamage(BurningDamage, true);
}

void UGamePlayEffectsComponent::EnableBurningParticle()
{
	if(BurningParticleSystem && OwnerMeshComponent)
	{
		BurningParticleSystemComponent = UGameplayStatics::SpawnEmitterAttached(BurningParticleSystem,OwnerMeshComponent);
	}
}

void UGamePlayEffectsComponent::DisableBurningParticle()
{
	if(BurningParticleSystemComponent)
	{
		BurningParticleSystemComponent->DestroyComponent();
	}
}

void UGamePlayEffectsComponent::EnableBurnUIProps()
{
	if(OwnerHCharacter->HHudReference)
	{
		if(OwnerHCharacter->HHudReference->GenelWidget)
		{
			OwnerHCharacter->HHudReference->GenelWidget->BurnImage->SetRenderOpacity(1);
		}
	}
	else if(OwnerHCharacter->CharacterOverlayWidget)
	{
		OwnerHCharacter->CharacterOverlayWidget->BurnImage->SetRenderOpacity(1);
	}
}

void UGamePlayEffectsComponent::DisableBurnUIProps()
{
	if(OwnerHCharacter->HHudReference)
	{
		if(OwnerHCharacter->HHudReference->GenelWidget)
		{
			OwnerHCharacter->HHudReference->GenelWidget->BurnImage->SetRenderOpacity(0);
		}
	}
	else if(OwnerHCharacter->CharacterOverlayWidget)
	{
		OwnerHCharacter->CharacterOverlayWidget->BurnImage->SetRenderOpacity(0);
	}
}



