// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HANEDANRPG3/Characters/HCharacter.h"
#include "GamePlayEffectsComponent.generated.h"


class UCombatComponent;
class AHCharacter;
class AHPlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HANEDANRPG3_API UGamePlayEffectsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UGamePlayEffectsComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	
	virtual void BeginPlay() override;

public:

	/**
	 * REFERENCES
	 */

	UPROPERTY()
	AHCharacter* OwnerHCharacter;

	UPROPERTY()
	AHPlayerController* OwnerHPlayerController;

	UPROPERTY()
	UCombatComponent* OwnerCombatComponent;

	UPROPERTY()
	USkeletalMeshComponent* OwnerMeshComponent;

	/**
	 * EFFECTS
	 * Stun
	 * Silence
	 * Slow
	 * Froze
	 * Burning
	 */

	/**
	 * GENEL
	 */

	bool bCanEffectOwnerHCharacter() const;

	void EffectDamage(float Damage, bool bFire);
	
	UPROPERTY()
	FCharacterActionStateStruct NewActionStateStruct;

	/** STUN */

	UPROPERTY(ReplicatedUsing=OnRep_Stunned)
	bool bStunned = false;
	UFUNCTION()
	void OnRep_Stunned();

	void EnableStun(float Seconds);
	void ClearStun();
	
	FTimerHandle StunTimerHandle;
	
	void EnableStunParticle();
	void DisableStunParticle();
	void EnableStunUIProps();
	void DisableStunUIProps();
	UPROPERTY(EditDefaultsOnly,Category="Properties")
	UParticleSystem* StunParticleSystem;
	UPROPERTY()
	UParticleSystemComponent* StunParticleSystemComponent;

	

	/** SILENCE */
	
	UPROPERTY(ReplicatedUsing=OnRep_Silenced)
	bool bSilenced = false;
	UFUNCTION()
	void OnRep_Silenced();

	void EnableSilence(float Seconds);
	void ClearSilence();
	
	FTimerHandle SilenceTimerHandle;
	
	void EnableSilenceParticle();
	void DisableSilenceParticle();
	void EnableSilenceUIProps();
	void DisableSilenceUIProps();
	UPROPERTY(EditDefaultsOnly,Category="Properties")
	UParticleSystem* SilenceParticleSystem;
	UPROPERTY()
	UParticleSystemComponent* SilenceParticleSystemComponent;

	/** SLOW */
	
	UPROPERTY(ReplicatedUsing=OnRep_Slowed)
	bool bSlowed = false;
	UFUNCTION()
	void OnRep_Slowed();
	
	void EnableSlow(float Seconds);
	void ClearSlow();
	
	FTimerHandle SlowTimerHandle;
	
	void EnableSlowParticle();
	void DisableSlowParticle();
	void EnableSlowUIProps();
	void DisableSlowUIProps();
	UPROPERTY(EditDefaultsOnly,Category="Properties")
	UParticleSystem* SlowParticleSystem;
	UPROPERTY()
	UParticleSystemComponent* SlowParticleSystemComponent;

	/** FROZE */

	UPROPERTY(ReplicatedUsing=OnRep_Frozen)
	bool bFrozen = false;
	UFUNCTION()
	void OnRep_Frozen();
	
	void EnableFroze(float Seconds, float FrequencySeconds, float FrequencyDamage);
	
	FTimerHandle FrozeTimerHandle;
	FTimerHandle FrozeFrequencyTimerHandle;
	
	float FrozingDamage = 5.f;
	
	void FrozeDamage();
	
	void ClearFroze();

	void EnableFrozeParticle();
	void DisableFrozeParticle();
	void EnableFrozeUIProps();
	void DisableFrozeUIProps();
	UPROPERTY(EditDefaultsOnly,Category="Properties")
	UParticleSystem* FrozeParticleSystem;
	UPROPERTY()
	UParticleSystemComponent* FrozeParticleSystemComponent;

	/** BURN */

	UPROPERTY(ReplicatedUsing=OnRep_Burning)
	bool bBurning = false;
	UFUNCTION()
	void OnRep_Burning();

	UFUNCTION(BlueprintCallable)
	void EnableBurn(float Seconds, float FrequencySeconds, float FrequencyDamage);

	FTimerHandle BurnTimerHandle;
	FTimerHandle BurnFrequencyTimerHandle;
	
	float BurningDamage = 5.f;
	void BurnDamage();

	void ClearBurn();

	void EnableBurningParticle();
	void DisableBurningParticle();
	void EnableBurnUIProps();
	void DisableBurnUIProps();
	UPROPERTY(EditDefaultsOnly,Category="Properties")
	UParticleSystem* BurningParticleSystem;
	UPROPERTY()
	UParticleSystemComponent* BurningParticleSystemComponent;
	

	

	

	
	

	
	
	
		
};
