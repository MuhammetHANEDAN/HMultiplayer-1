// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HANEDANRPG3/Enums/EnumsType.h"
#include "CombatComponent.generated.h"


class ASkillActor;
class UDamageIndicatorWidgetComponent;
class AHHUD;
class UCharacterOverlayWidget;
class AWeapon;
class UHAnimInstance;
class AHPlayerController;
class AHCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HANEDANRPG3_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UCombatComponent();
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
	USkeletalMeshComponent* OwnerHMeshComponent;

	UPROPERTY()
	UHAnimInstance* OwnerHAnimInstance;

	UPROPERTY()
	AHHUD* OwnerHHUD;

	UPROPERTY()
	UCharacterOverlayWidget* OwnerCharacterOverlayWidget;
	

	/**
	 * TIME SYNC
	 */

	float ServerMapTime = 0.f;

	float LastRoundTripTime = 0.f;

	/**
	 * NORMAL ATTACKS
	 */
	
	uint8 MaxAttackAnimAmount = 0; // başlangıçta arrayden alınacak sadece serverden alınabilir
	UPROPERTY(Replicated)
	uint8 NormalAttackNumber = 0; 

	void RequestNormalAttack();
	UFUNCTION(Server,Reliable)
	void Server_NormalAttack();
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_PlayNormalAttackMontage();
	
	UFUNCTION(BlueprintCallable) // AnimBp de montagede animnotfiy olarak çağıralacak çok beklenirse 1 normal attack atılınca combo refresh yicek
	void RefreshNormalAttacks();

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	TArray<UAnimMontage*> NormalAttackAnimMontageArray;
	
	/**
	 * WEAPON
	 */

	void HandleBeginSpawnAndAttachToBackWeapon();

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	TSubclassOf<AWeapon> FirstWeaponClass;
	UPROPERTY(Replicated)
	AWeapon* FirstWeapon = nullptr;
	
	UPROPERTY(EditDefaultsOnly,Category="Properties")
	TSubclassOf<AWeapon> SecondWeaponClass;
	UPROPERTY(Replicated)
	AWeapon* SecondWeapon;
	

	/**
	 * MOVEMENT MODE 
	 */

	UPROPERTY(ReplicatedUsing=OnRep_MovementMode)
	bool bMovementModeIsArmed = false;
	UFUNCTION()
	void OnRep_MovementMode();
	void SetMovementMode(bool bToArmed); // serverda çağır

	/**
	 * EQUİP MECHANIC
	 */
	
	UPROPERTY(ReplicatedUsing=OnRep_bWeaponOnHand)
	bool bWeaponOnHand = false;
	UFUNCTION()
	void OnRep_bWeaponOnHand();
	UFUNCTION(Server,Reliable)
	void Server_PlayEquipWeaponMontageAndSetMoveMode(); 

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	UAnimMontage* EquipMontage;
	UFUNCTION(BlueprintCallable)
	void EquipMontageCallNotify();
	
	UPROPERTY(EditDefaultsOnly,Category="Properties")
	UAnimMontage* DisarmMontage;
	UFUNCTION(BlueprintCallable)
	void DisarmMontageCallNotify();

	UFUNCTION(NetMulticast,Reliable)
	void Multicast_AttachWeaponToBack();
	
	
	/**
	 * STATS
	 */

	
	virtual void GetDamage(float Damage, const FVector& HitLocation, const EHitType HitType, const AActor* DamageCauserActor);
	virtual void GetEffectDamage(float Damage, bool bFire);

	UPROPERTY(ReplicatedUsing=OnRep_CurrentSpeed, VisibleDefaultsOnly, Category="Properties" )
	uint16 CurrentSpeed = 0;
	UFUNCTION()
	void OnRep_CurrentSpeed();
	void SetCurrentSpeed(uint16 Speed);

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	uint16 NormalSpeed = 800;

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	uint16 SlowedSpeed = 500;

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	float MaxHP = 500;

	UPROPERTY(BlueprintReadWrite,ReplicatedUsing=OnRep_CurrentHP,Category="Properties")
	float CurrentHP = 0;
	UFUNCTION()
	void OnRep_CurrentHP();

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	float MaxMP = 1000;

	UPROPERTY(BlueprintReadWrite,ReplicatedUsing=OnRep_CurrentMP,Category="Properties")
	float CurrentMP = 0;
	UFUNCTION()
	void OnRep_CurrentMP();

	/**
	 * REGEN SYSTEM
	 * İstiyoruzki her damage alındığında tickte bregentimerstart olsun
	 */

	FTimerHandle HPRegenCooldownTimerHandle;
	void StartHPRegenCooldownTimer(); // damage alınınca çağırılacak
	void StartHPRegen(); // Rateli bir şekilde HPRegen çağıracak
	FTimerHandle HPRegenTimerHandle;
	void HPRegen();

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	float HPRegenCooldownTime = 150.f; // Hasar alındıktan sonra herhangi bir hasar alınmazsa hp regenin başlaması için geçecek süre
	UPROPERTY(EditDefaultsOnly,Category="Properties")
	float HPRegenTimeRate = 1.f; // Hp regenin ne kadar sıklıkla çağırılacağı süre
	UPROPERTY(EditDefaultsOnly,Category="Properties")
	float HPRegenAmount = 5.f; // HP regenin çağırılnca uygulayacağı HP miktarı


	FTimerHandle MPRegenCooldownTimerHandle;
	void StartMPRegenCooldownTimer(); 
	void StartMPRegen();
	FTimerHandle MPRegenTimerHandle;
	void MPRegen();

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	float MPRegenCooldownTime = 10.f; 
	UPROPERTY(EditDefaultsOnly,Category="Properties")
	float MPRegenTimeRate = 1.f; 
	UPROPERTY(EditDefaultsOnly,Category="Properties")
	float MPRegenAmount = 50.f; 

	/** Verilen mana costu alır current manaya bakar eğer, current < manacost false döndürür.
	 * Büyük veya eşitse gerekli azaltma işlemleri ve ui işlemlerini başlatır.
	 */
	bool CheckCurrentManaAndReduce (float ManaCost);
	

	/**
	 * SKILL WORKS
	 */

	/**
	 * 1 -- skill 1
	 * 4 -- skill 4
	 */
	UFUNCTION(Server,Reliable)
	void Server_CastSkill(uint8 SkillNumber);
	
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_PlaySkillMontage(uint8 SkillNumber);

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	TArray<UAnimMontage*> SkillMontages;

	/** Cast animi intereupt yerse veya biterse silinmesi gereken skill actorler varsa buraya ekle
	 * ANS_SetCharacterStatesAnimNotState in endnotifyı clearallctorsu, aşağıdaki fonku kullnarak clearlayacak
	 */
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite,Category="Properties")
	TArray<ASkillActor*> ActiveAttachedSkillActors;

	/** Cast animi intrepu yiyen veya anim bitince silinmesi gereken skill actorler ActiveSkillActors arrayine eklenmişse
	 * doğru şekilde, o actörleri silmeye yarar.
	 */
	UFUNCTION(BlueprintCallable)
	void DestroyAttachedAllSkillActors();
	UFUNCTION(BlueprintCallable)
	void AddSkillActorToActiveAttachedSkillActorArray(ASkillActor* LSkillActor);

	/**
	 * SKİLL COOLDOWN SYSTEM
	 */

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Properties")
	uint8 Skill1Cooldown = 10;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Properties")
	uint8 Skill1ManaCost = 20;
	
	bool bSkill1OnCooldDown = false;
	float Skill1TimeRemainSeconds = 0.f;
	void SetGenelWidgetSkill1Text(float TimeRemainSeconds);
	void ActivateSkill1Cooldown(); 
	void DeactivateSkill1Cooldown();

	

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Properties")
	uint8 Skill2Cooldown = 10;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Properties")
	uint8 Skill2ManaCost = 20;
	
	bool bSkill2OnCooldDown = false;
	float Skill2TimeRemainSeconds = 0.f;
	void SetGenelWidgetSkill2Text(float TimeRemainSeconds);
	void ActivateSkill2Cooldown(); 
	void DeactivateSkill2Cooldown();


	

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Properties")
	uint8 Skill3Cooldown = 10;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Properties")
	uint8 Skill3ManaCost = 20;
	
	bool bSkill3OnCooldDown = false;
	float Skill3TimeRemainSeconds = 0.f;
	void SetGenelWidgetSkill3Text(float TimeRemainSeconds);
	void ActivateSkill3Cooldown(); 
	void DeactivateSkill3Cooldown();

	

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Properties")
	uint8 Skill4Cooldown = 10;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Properties")
	uint8 Skill4ManaCost = 20;
	
	bool bSkill4OnCooldDown = false;
	float Skill4TimeRemainSeconds = 0.f;
	void SetGenelWidgetSkill4Text(float TimeRemainSeconds);
	void ActivateSkill4Cooldown(); 
	void DeactivateSkill4Cooldown(); 



	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Properties")
	uint8 SkillECooldown = 10;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Properties")
	uint8 SkillEManaCost = 20;
	
	bool bSkillEOnCooldDown = false;
	float SkillETimeRemainSeconds = 0.f;
	void SetGenelWidgetSkillEText(float TimeRemainSeconds);
	void ActivateSkillECooldown(); 
	void DeactivateSkillECooldown();

	UFUNCTION(Client,Reliable)
	void Client_ActivateSkillECooldown(); 

	
	
	
	

	/**
	 * BLOCK
	 */

	/**
	 * begin playde temporary block amount max block amounta çekildi.
	 * HCharacterde Bloğun açılmasını check ettik doğruysa combatcomp.server rpc attık
	 * server rpc dede check ettik, 
	 * current block amount temporary block amounta çekildi
	 * current block amount > 0 ise block montage oynamaya başlar
	 * Blok açıkken - herhangibir hasar alınmadan kapanırsa temp = current - current=0
	 * blok açıkken- hasar alındı -  temp block amount = current - yeniden maks block açabilme timerı etkinleşsin
	 * 
	 * 
	 */
	
	UPROPERTY(EditDefaultsOnly,Category="Properties")
	UAnimMontage* BlockMontage;

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	UAnimMontage* HitMontage;


	UPROPERTY(EditDefaultsOnly,Category="Properties")
	float MaxBlockAmount = 100;

	UPROPERTY(VisibleAnywhere)
	float TemporaryMaxBlockAmount = 0; // Blok açıldı resetleme zamanı geçmeden alınabilecek max block amount

	UPROPERTY(ReplicatedUsing=OnRep_CurrentBlockAmount,EditDefaultsOnly,Category="Properties")
	float CurrentBlockAmount = 0; 
	UFUNCTION()
	void OnRep_CurrentBlockAmount();

	UFUNCTION(Server,Reliable)
	void Server_PerformBlock();

	UFUNCTION(Server,Reliable)
	void Server_CloseBlock();

	UFUNCTION(NetMulticast,Reliable)
	void Multicast_PlayBlockMontage();
	
	void PlayBlockMontageHit();
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_PlayBlockMontageHit();

	UFUNCTION(NetMulticast,Reliable)
	void Multicast_PlayBlockMontageBroken();
	
	UFUNCTION(BlueprintCallable)
	void ContinueBlock(); // Blok Açıkken hit yiyince block kırılmadı ve açıksa block montage loop oynamaya devam etmeli

	UFUNCTION(NetMulticast,Reliable)
	void Multicast_PlayCloseBlockMontage();
	
	
	FTimerHandle RefreshTempBlockAmountTimerHandle;
	UPROPERTY(EditDefaultsOnly,Category="Properties")
	uint8 RefreshTempBlockAmountTime = 5.f; // Bu kadar süre boyunca hasar alınmazsa açılabilecek block max blocka eşitlenir.
	void StartTimerForRefreshTempBlockAmount();
	void RefreshTempBlockAmount();
	bool bRefreshTimerForTempBlockAmountStarted = false;

	/**
	 * Knock Down
	 */

	/** Anim eksikliğinden dolayı sadece geriye knock var o yüzden hit alınan yere göre karakteri döndür*/
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Properties")
	UAnimMontage* KnockDownForwardMontage;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Properties")
	UAnimMontage* KnockDownBackwardMontage;

	bool bcanKnockdown();
	void PerformKnockdown(bool bForward);
	UFUNCTION(Server,Reliable)
	void Server_PerformKnockdown(bool bForward);
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_PlayKnockDownMontage(bool bForward);
	

	/**
	 * Character Overlay Widget Component Works
	 */

	void UpdateCharacterOverlayHPBar();
	void UpdateCharacterOverlayMPBar();
	void UpdateCharacterOverlayShieldBar();
	void SetVisibilityOfShieldBar(bool bVisible);

	/**
	 * Genel Widget Works
	 */

	void UpdateGenelWidgetHPBar();
	void UpdateGenelWidgetMPBar();
	void UpdateGenelWidgetShieldBar();

	/**
	 * DASH COOLDOWN SYSTEM
	 */
	
	bool bDashOnCooldown= false;
	float DashTrack = 0;
	UPROPERTY(EditDefaultsOnly)
	uint8 DashCooldown = 5;

	/**
	 * DAMAGE INDICATOR
	 */

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Properties")
	TSubclassOf<UDamageIndicatorWidgetComponent> DamageIndicatorWidgetComponentClass;

	void DIndicatorForNormalDamage(float Damage);
	/**
	 * Damage Alma fonk sadece serverde gerçekleştirildiği için damageindicatoru bir şekilde clientlere replike etmek zorundaydık
	 * Clientler için çalışacak olan multicast fonk.
	 */
	UFUNCTION(NetMulticast,Unreliable) 
	void Multicast_DIndicatorForNormalDamage(uint16 Damage); 
	
	void DIndicatorForShieldDamage(float Damage);
	UFUNCTION(NetMulticast,Unreliable) 
	void Multicast_DIndicatorForShieldDamage(uint16 Damage);
	
	void DIndicatorForFireDamage(float Damage);
	UFUNCTION(NetMulticast,Unreliable) 
	void Multicast_DIndicatorForFireDamage(uint16 Damage);
	
	void DIndicatorForFrostDamage(float Damage);
	UFUNCTION(NetMulticast,Unreliable) 
	void Multicast_DIndicatorForFrostDamage(uint16 Damage);

	/**
	 * CHARACTER SPECIAL PASSIVES
	 */

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	uint8 GreatSwordCharacterPassiveDamageReductionPercent = 10; // Effect damageler hariç tüm damageyi yüzde olarak düşürecek

	/**
	 * CHARACTERS SPECIAL ABILITIES 
	 */

	UFUNCTION(Server,Reliable)
	void Server_CastSkillEGreatSword();

	UFUNCTION(Server,Reliable)
	void Server_CastSkillEDualSword();

	UFUNCTION(Server,Reliable)
	void Server_CastSkillEMage();

	// Çok fazla farklı data olmayacağı için child class alınmadı tag yardımı ile devam edilecek

	/**
	 *GreatSword E Special Skill
	 * Greatsowrd execution olayında actor rot setlenmeleri skill actor ile yapılacak.
	 */
	

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	UAnimMontage* GreatSwordExecutionerMontage;

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	UAnimMontage* GreatSwordExecutedMontage;

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	TSubclassOf<ASkillActor> GreatSwordSkillEActorClass;

	bool bCanBeExecuted();

	void PerformGreatSwordExecutioner();
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_PlayGreatSwordExecutionerMontage();

	void PerformGreatSwordExecuted();
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_PlayGreatSwordExecutedMontage();

	/**
	 * DualSword E Special Skill
	 */

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	UAnimMontage* DualSwordAmbushMontage;

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	UAnimMontage* DualSwordAmbushedMontage;

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	TSubclassOf<ASkillActor> DualSwordSkillEActorClass;

	bool bCanBeAmbushed();

	void PerformDualSwordAmbush();
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_PlayDualSwordAmbushMontage();

	void PerformDualSwordAmbushed();
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_PlayDualSwordAmbushedMontage();

	/**
	 * Mage E Special Skill
	 */

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	UAnimMontage* MageESkillMontage;

	UFUNCTION(Server,Reliable)
	void Server_PerformMageESkill();

	UFUNCTION(NetMulticast,Reliable)
	void Multicast_PlayMageESkillMontage();
	











	
		
};
