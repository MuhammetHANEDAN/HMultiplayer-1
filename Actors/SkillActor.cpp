// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillActor.h"

#include "HANEDANRPG3/Characters/HCharacter.h"
#include "HANEDANRPG3/Components/CombatComponent.h"
#include "HANEDANRPG3/Components/GamePlayEffectsComponent.h"
#include "HANEDANRPG3/PlayerControllers/HPlayerController.h"
#include "Kismet/GameplayStatics.h"


ASkillActor::ASkillActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;

}

void ASkillActor::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		SetReplicateMovement(true);
	}
	
}

void ASkillActor::AddSkillActorToActiveAttachedSkillActorArray(ASkillActor* LSkillActor)
{
	if(GetOwnerHCharacter())
	{
		GetOwnerHCharacter()->CombatComponent->AddSkillActorToActiveAttachedSkillActorArray(LSkillActor);
	}
}

AHCharacter* ASkillActor::GetOwnerHCharacter() const
{
	if(GetOwner())
	{
		if(AHCharacter* RCharacter = Cast<AHCharacter>(GetOwner()))
		{
			return RCharacter;
		}
		return nullptr;
	}
	return nullptr;
}

AHCharacter* ASkillActor::GetAHCharacterFromActor(AActor* LActor) const
{
	if(LActor)
	{
		return Cast<AHCharacter>(LActor);
	}
	return nullptr;
}

void ASkillActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASkillActor::AddActorToHTargetActors(AActor* L_TargetActor)
{
	if(IsValid(L_TargetActor))
	{
		if(AHCharacter* L_Target = Cast<AHCharacter>(L_TargetActor))
		{
			HTargetActors.AddUnique(L_Target);
		}
	}
}

void ASkillActor::DeleteActorOnHTargetActros(AActor* L_TargetActor)
{
	if(IsValid(L_TargetActor))
	{
		if(AHCharacter* L_Target = Cast<AHCharacter>(L_TargetActor))
		{
			HTargetActors.Remove(L_Target);
		}
	}
}

void ASkillActor::Damage(float Damage, const FVector& HitLocation, const EHitType HitType,
                         const AActor* DamageCauserActor)
{
	HTargetActor = Cast<AHCharacter>(TargetActor);
	if(HTargetActor)
	{
		HTargetActor->GetHit(Damage,HitLocation,HitType,DamageCauserActor);
	}
}

void ASkillActor::DamageToAllHTargets(float Damage, const FVector& HitLocation, const EHitType HitType,
	const AActor* DamageCauserActor)
{
	if(HTargetActors.Num() > 0)
	{
		for (auto Element : HTargetActors)
		{
			if(!(IsValid(Element))) return;
			Element->GetHit(Damage,HitLocation,HitType,DamageCauserActor);
		}
	}
}

void ASkillActor::EffectDamage(float Damage, bool bFire)
{
	
}

void ASkillActor::EffectDamageToAllTargets(float Damage, bool bFire)
{
	if(HTargetActors.Num() > 0)
	{
		for (auto Element : HTargetActors)
		{
			if(!(IsValid(Element))) return;
			Element->CombatComponent->GetEffectDamage(Damage,bFire);
		}
	}
}

void ASkillActor::DamageOnIndex(float Damage, const FVector& HitLocation, const EHitType HitType,
	const AActor* DamageCauserActor, uint8 Index)
{
	if(GetHCharacterFromTargetActorsIndex(Index))
	{
		GetHCharacterFromTargetActorsIndex(Index)->CombatComponent->GetDamage(Damage,HitLocation,HitType,DamageCauserActor);
	}
}

void ASkillActor::EffectDamageOnIndex(float Damage, bool bFire, uint8 Index)
{
	
}

AHCharacter* ASkillActor::GetHCharacterFromTargetActorsIndex(uint8 Index) const
{
	if(TargetActors.Num()>0)
	{
		if(TargetActors[Index])
		{
			if(AHCharacter* TargetHCharacter = Cast<AHCharacter>(TargetActors[Index]))
			{
				return TargetHCharacter;
			}
			return nullptr;
		}
		return nullptr;
	}
	return nullptr;
}

void ASkillActor::EnableStun(float StunDuration)
{
	if(TargetActor)
	{
		if(AHCharacter* TargetHCharacter = Cast<AHCharacter>(TargetActor))
		{
			TargetHCharacter->GamePlayEffectsComponent->EnableStun(StunDuration);
		}
	}
}

void ASkillActor::EnableStunOnIndex(float StunDuration, uint8 Index)
{
	if(GetHCharacterFromTargetActorsIndex(Index))
	{
		GetHCharacterFromTargetActorsIndex(Index)->GamePlayEffectsComponent->EnableStun(StunDuration);
	}
}

void ASkillActor::EnableStunOnAllHTargets(float StunDuration)
{
	if(HTargetActors.Num() > 0)
	{
		for (auto Element : HTargetActors)
		{
			if(!(IsValid(Element))) return;
			Element->GamePlayEffectsComponent->EnableStun(StunDuration);
		}
	}
}

void ASkillActor::EnableSilence(float SilenceDuration)
{
	if(TargetActor)
	{
		if(AHCharacter* TargetHCharacter = Cast<AHCharacter>(TargetActor))
		{
			TargetHCharacter->GamePlayEffectsComponent->EnableSilence(SilenceDuration);
		}
	}
}

void ASkillActor::EnableSilenceOnRef(AActor* LActor, float SilenceDuration)
{
	AHCharacter* hactor = GetAHCharacterFromActor(LActor);
	if(hactor)
	{
		hactor->GamePlayEffectsComponent->EnableSilence(SilenceDuration);
	}
}

void ASkillActor::EnableSilenceOnIndex(float SilenceDuration, uint8 Index)
{
	if(GetHCharacterFromTargetActorsIndex(Index))
	{
		GetHCharacterFromTargetActorsIndex(Index)->GamePlayEffectsComponent->EnableSilence(SilenceDuration);
	}
}

void ASkillActor::ClearSilenceOnIndex(uint8 Index)
{
	if(GetHCharacterFromTargetActorsIndex(Index))
	{
		GetHCharacterFromTargetActorsIndex(Index)->GamePlayEffectsComponent->ClearSilence();
	}
}

void ASkillActor::ClearSilenceOnRef(AActor* LActor)
{
	AHCharacter* hactor = GetAHCharacterFromActor(LActor);
	if(hactor)
	{
		hactor->GamePlayEffectsComponent->ClearSilence();
	}
}

void ASkillActor::EnableSilenceOnAllHTargets(float SilenceDuration)
{
	if(HTargetActors.Num() > 0)
	{
		for (auto Element : HTargetActors)
		{
			if(!(IsValid(Element))) return;
			Element->GamePlayEffectsComponent->EnableSilence(SilenceDuration);
		}
	}
}

void ASkillActor::EnableSlow(float SlowDuration)
{
	if(TargetActor)
	{
		if(AHCharacter* TargetHCharacter = Cast<AHCharacter>(TargetActor))
		{
			TargetHCharacter->GamePlayEffectsComponent->EnableSlow(SlowDuration);
		}
	}
}

void ASkillActor::EnableSlowOnIndex(float SlowDuration, uint8 Index)
{
	if(GetHCharacterFromTargetActorsIndex(Index))
	{
		GetHCharacterFromTargetActorsIndex(Index)->GamePlayEffectsComponent->EnableSlow(SlowDuration);
	}
}

void ASkillActor::EnableSlowOnAllHTargets(float SlowDuration)
{
	if(HTargetActors.Num() > 0)
	{
		for (auto Element : HTargetActors)
		{
			if(!(IsValid(Element))) return;
			Element->GamePlayEffectsComponent->EnableSlow(SlowDuration);
		}
	}
}

void ASkillActor::EnableSlowOnRef(AActor* LActor, float SlowDuration)
{
	AHCharacter* hactor = GetAHCharacterFromActor(LActor);
	if(hactor)
	{
		hactor->GamePlayEffectsComponent->EnableSlow(SlowDuration);
	}
}

void ASkillActor::ClearSlowOnRef(AActor* LActor)
{
	AHCharacter* hactor = GetAHCharacterFromActor(LActor);
	if(hactor)
	{
		hactor->GamePlayEffectsComponent->ClearSlow();
	}
}

void ASkillActor::EnableFroze(float Duration, float FrequencySeconds, float FrequencyDamage)
{
	if(TargetActor)
	{
		if(AHCharacter* TargetHCharacter = Cast<AHCharacter>(TargetActor))
		{
			TargetHCharacter->GamePlayEffectsComponent->EnableFroze(Duration,FrequencySeconds,FrequencyDamage);
		}
	}
}

void ASkillActor::EnableFrozeOnIndex(float Duration, float FrequencySeconds, float FrequencyDamage, uint8 Index)
{
	if(GetHCharacterFromTargetActorsIndex(Index))
	{
		GetHCharacterFromTargetActorsIndex(Index)->GamePlayEffectsComponent->EnableFroze(Duration,FrequencySeconds,FrequencyDamage);
	}
}

void ASkillActor::EnableFrozeOnAllHTargets(float Duration, float FrequencySeconds, float FrequencyDamage)
{
	if(HTargetActors.Num() > 0)
	{
		for (auto Element : HTargetActors)
		{
			if(!(IsValid(Element))) return;
			Element->GamePlayEffectsComponent->EnableFroze(Duration,FrequencySeconds,FrequencyDamage);
		}
	}
}

void ASkillActor::EnableFrozeOnRef(AActor* LActor, float Duration, float FrequencySeconds, float FrequencyDamage)
{
	AHCharacter* hactor = GetAHCharacterFromActor(LActor);
	if(hactor)
	{
		hactor->GamePlayEffectsComponent->EnableFroze(Duration,FrequencySeconds,FrequencyDamage);
	}
}

void ASkillActor::ClearFrozeOnRef(AActor* LActor)
{
	AHCharacter* hactor = GetAHCharacterFromActor(LActor);
	if(hactor)
	{
		hactor->GamePlayEffectsComponent->ClearFroze();
	}
}

void ASkillActor::EnableBurning(float Duration, float FrequencySeconds, float FrequencyDamage)
{
	if(TargetActor)
	{
		if(AHCharacter* TargetHCharacter = Cast<AHCharacter>(TargetActor))
		{
			TargetHCharacter->GamePlayEffectsComponent->EnableBurn(Duration,FrequencySeconds,FrequencyDamage);
		}
	}
}

void ASkillActor::EnableBurningOnIndex(float Duration, float FrequencySeconds, float FrequencyDamage, uint8 Index)
{
	if(GetHCharacterFromTargetActorsIndex(Index))
	{
		GetHCharacterFromTargetActorsIndex(Index)->GamePlayEffectsComponent->EnableBurn(Duration,FrequencySeconds,FrequencyDamage);
	}
}

void ASkillActor::EnableBurningOnAllHTargets(float Duration, float FrequencySeconds, float FrequencyDamage)
{
	if(HTargetActors.Num() > 0)
	{
		for (auto Element : HTargetActors)
		{
			if(!(IsValid(Element))) return;
			Element->GamePlayEffectsComponent->EnableBurn(Duration,FrequencySeconds,FrequencyDamage);
		}
	}
}

void ASkillActor::EnableBurningOnRef(AActor* LActor, float Duration, float FrequencySeconds, float FrequencyDamage)
{
	AHCharacter* hactor = GetAHCharacterFromActor(LActor);
	if(hactor)
	{
		hactor->GamePlayEffectsComponent->EnableBurn(Duration,FrequencySeconds,FrequencyDamage);
	}
}

void ASkillActor::ClearBurningOnRef(AActor* LActor)
{
	AHCharacter* hactor = GetAHCharacterFromActor(LActor);
	if(hactor)
	{
		hactor->GamePlayEffectsComponent->ClearBurn();
	}
}

void ASkillActor::EnableKnockdown(bool bForward)
{
	if(TargetActor)
	{
		if(AHCharacter* TargetHCharacter = Cast<AHCharacter>(TargetActor))
		{
			TargetHCharacter->CombatComponent->Server_PerformKnockdown(bForward);
		}
	}
}

void ASkillActor::EnableKnockdownOnIndex(bool bForward, uint8 Index)
{
	if(GetHCharacterFromTargetActorsIndex(Index))
	{
		GetHCharacterFromTargetActorsIndex(Index)->CombatComponent->PerformKnockdown(bForward);
	}
}

bool ASkillActor::CalculateForwardOrBackwardHit(const AActor* DamagedActor, const FVector& HitLocation) const
{
	if(DamagedActor == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("damaged actor null o yüzzden false döndü"));
		return false;
	}
	const FVector Forward = DamagedActor->GetActorForwardVector();
	// Lower Impact Point to the Enemy's Actor Location Z
	const FVector ImpactLowered(HitLocation.X, HitLocation.Y,  DamagedActor->GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered -  DamagedActor->GetActorLocation()).GetSafeNormal();

	// Forward * ToHit = |Forward||ToHit| * cos(theta)
	// |Forward| = 1, |ToHit| = 1, so Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	// Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);
	// convert from radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	// if CrossProduct points down, Theta should be negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	if (Theta >= -90.f && Theta <= 90.f)
	{
		return true;
	}
	
	return false;
}

FVector ASkillActor::GetLocationUnderCrosshair() const
{
	FVector2d ViewportSize;
	
	if(GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	else
	{
		return FVector::ZeroVector;
	}

	FVector2d CrosshairLocationOnViewport = FVector2d(ViewportSize.X/2.f,ViewportSize.Y/2.f);
	FVector WorldLocation;
	FVector WorldDirection;
	bool bProjected =UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this,0),
		CrosshairLocationOnViewport,WorldLocation,WorldDirection);

	if(bProjected)
	{
		FVector End = WorldLocation + WorldDirection*TRACE_LENGTH;

		FHitResult OutHitResult;
		
		GetWorld()->LineTraceSingleByChannel(OutHitResult,WorldLocation,End,ECC_Visibility);

		if(OutHitResult.bBlockingHit)
		{
			return OutHitResult.ImpactPoint;
		}
		return End;
	}
	return FVector::ZeroVector;
}

bool ASkillActor::bTargetCanExecutable(AActor* LActor) const
{
	if(AHCharacter* LHCharacter = Cast<AHCharacter>(LActor))
	{
		return LHCharacter->CombatComponent->bCanBeExecuted();
	}
	return false;
}

void ASkillActor::PerformGreatSwordExecutioner(AActor* LActor)
{
	if(AHCharacter* LHCharacter = Cast<AHCharacter>(LActor))
	{
		return LHCharacter->CombatComponent->PerformGreatSwordExecutioner();
	}
}

void ASkillActor::PerformGreatSwordExecuted(AActor* LActor)
{
	if(AHCharacter* LHCharacter = Cast<AHCharacter>(LActor))
	{
		return LHCharacter->CombatComponent->PerformGreatSwordExecuted();
	}
}

bool ASkillActor::bTargetCanAmbushed(AActor* LActor) const
{
	if(AHCharacter* LHCharacter = Cast<AHCharacter>(LActor))
	{
		return LHCharacter->CombatComponent->bCanBeAmbushed();
	}
	return false;
}

void ASkillActor::PerformDualSwordAmbush(AActor* LActor)
{
	if(AHCharacter* LHCharacter = Cast<AHCharacter>(LActor))
	{
		return LHCharacter->CombatComponent->PerformDualSwordAmbush();
	}
}

void ASkillActor::PerformDualSwordAmbushed(AActor* LActor)
{
	if(AHCharacter* LHCharacter = Cast<AHCharacter>(LActor))
	{
		return LHCharacter->CombatComponent->PerformDualSwordAmbushed();
	}
}

void ASkillActor::ActivateSkillECooldown()
{
	if(GetOwnerHCharacter() && GetOwnerHCharacter()->HHudReference)
	{
		GetOwnerHCharacter()->CombatComponent->ActivateSkillECooldown();
	}
	else if(GetOwnerHCharacter() && !GetOwnerHCharacter()->HHudReference)
	{
		GetOwnerHCharacter()->CombatComponent->ActivateSkillECooldown();
		GetOwnerHCharacter()->CombatComponent->Client_ActivateSkillECooldown();
	}
}

bool ASkillActor::CheckCurrentManaAndReduce(float ManaCost) const
{
	if(GetOwnerHCharacter())
	{
		return GetOwnerHCharacter()->CombatComponent->CheckCurrentManaAndReduce(ManaCost);
	}
	return false;
}

float ASkillActor::GetSkillEManaCost() const
{
	if(GetOwnerHCharacter())
	{
		return GetOwnerHCharacter()->CombatComponent->SkillEManaCost;
	}
	return 0;
}





