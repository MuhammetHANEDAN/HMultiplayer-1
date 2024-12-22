// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HANEDANRPG3/Enums/EnumsType.h"
#include "SkillActor.generated.h"

#define TRACE_LENGTH  100000.f;

class IHitInterface;
class AHCharacter;

UCLASS()
class HANEDANRPG3_API ASkillActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASkillActor();
	virtual void Tick(float DeltaTime) override;

protected:
	
	virtual void BeginPlay() override;

public:

	/**
	 * Free space
	 */

	UFUNCTION(BlueprintCallable)
	void AddSkillActorToActiveAttachedSkillActorArray(ASkillActor* LSkillActor);

	/** Skill Actorun Owmer HCharacteri*/
	UFUNCTION(BlueprintCallable)
	AHCharacter* GetOwnerHCharacter() const;

	UFUNCTION(BlueprintCallable)
	AHCharacter* GetAHCharacterFromActor(AActor* LActor) const;

	/**
	 * Trget actor setlenip direk bu referans üzerinden işlemler yapılıyodu
	 * ama garip bir bug oldu bugın sebebi target actor refin çok hızlı değişmesi olabilirmi 
	 * o yüzden önce actprleri arraye alıp sonra indexe göre işlem yapmaya karaar verdik . Referans değişmeden
	 */
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite,Category="Properties")
	AActor* TargetActor;
	UPROPERTY()
	AHCharacter* HTargetActor = nullptr; // tekli hedef için cast işlemlerinde kolaylık olsun diye fonklarda
	// ---------------------------------------

	/** BP den eklenecek olan targetleri alacak array */
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Properties")
	TArray<AActor*> TargetActors;
	/** TargetActors arrayinden istenilen indexe gider o indexi hcharactere castleyip döndürür null olabilir  */
	UFUNCTION(BlueprintCallable)
	AHCharacter* GetHCharacterFromTargetActorsIndex(uint8 Index) const;

	/**Add ve Delete ile eklenecek olan AActor leri eğer HCharacter ise ekleneceği array
	 *
	 * Daha çok attachlenen ve sürekli olarak alan damage veren skiller kullanıcka mesela tornado veya spin
	 */
	UPROPERTY(BlueprintReadWrite)
	TArray<AHCharacter*> HTargetActors;
	UFUNCTION(BlueprintCallable)
	void AddActorToHTargetActors(AActor* L_TargetActor);
	UFUNCTION(BlueprintCallable)
	void DeleteActorOnHTargetActros(AActor* L_TargetActor);
	

	/** Bp de değişik amaçlar için kullanılabilir mesela bir defa damage verdiğine daha damage vermemek için refler tutulabilir*/
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Properties")
	TArray<AActor*> DamagedActors;

	/** Çalışması için TargetActor setlenmelidir */
	UFUNCTION(BlueprintCallable)
	void Damage(float Damage, const FVector& HitLocation, const EHitType HitType, const AActor* DamageCauserActor);

	/** Çalıştırmak için addactortohtargetactors ve dDeleteActorOnHTargetActros fonkları çalıştırılmalıdır.
	 *
	 * Dikkat sadece TArray<AHCharacter*> HTargetActors; arrayinde bulunanlara damage verir
	 */
	UFUNCTION(BlueprintCallable)
	void DamageToAllHTargets(float Damage, const FVector& HitLocation, const EHitType HitType, const AActor* DamageCauserActor);
	
	UFUNCTION(BlueprintCallable)
	void EffectDamage(float Damage, bool bFire);
	UFUNCTION(BlueprintCallable)
	void EffectDamageToAllTargets(float Damage, bool bFire);

	/** TargetActors Arrayi Hedef alınır*/
	UFUNCTION(BlueprintCallable)
	void DamageOnIndex(float Damage, const FVector& HitLocation, const EHitType HitType, const AActor* DamageCauserActor, uint8 Index);
	/** TargetActors Arrayi Hedef alınır*/
	UFUNCTION(BlueprintCallable)
	void EffectDamageOnIndex(float Damage, bool bFire, uint8 Index);
	
	
	
	/** Gameplay Effects */

	UFUNCTION(BlueprintCallable)
	void EnableStun(float StunDuration);
	UFUNCTION(BlueprintCallable)
	void EnableStunOnIndex(float StunDuration, uint8 Index);
	UFUNCTION(BlueprintCallable)
	void EnableStunOnAllHTargets(float StunDuration);

	UFUNCTION(BlueprintCallable)
	void EnableSilence(float SilenceDuration);
	UFUNCTION(BlueprintCallable)
	void EnableSilenceOnIndex(float SilenceDuration, uint8 Index);
	UFUNCTION(BlueprintCallable)
	void ClearSilenceOnIndex(uint8 Index);
	UFUNCTION(BlueprintCallable)
	void EnableSilenceOnAllHTargets(float SilenceDuration);

	UFUNCTION(BlueprintCallable)
	void EnableSilenceOnRef(AActor*LActor,float SilenceDuration);
	UFUNCTION(BlueprintCallable)
	void ClearSilenceOnRef(AActor* LActor);
	
	UFUNCTION(BlueprintCallable)
	void EnableSlow(float SlowDuration);
	UFUNCTION(BlueprintCallable)
	void EnableSlowOnIndex(float SlowDuration, uint8 Index);
	UFUNCTION(BlueprintCallable)
	void EnableSlowOnAllHTargets(float SlowDuration);

	UFUNCTION(BlueprintCallable)
	void EnableSlowOnRef(AActor* LActor,float SlowDuration);
	UFUNCTION(BlueprintCallable)
	void ClearSlowOnRef(AActor* LActor);

	UFUNCTION(BlueprintCallable)
	void EnableFroze(float Duration, float FrequencySeconds, float FrequencyDamage);
	UFUNCTION(BlueprintCallable)
	void EnableFrozeOnIndex(float Duration, float FrequencySeconds, float FrequencyDamage, uint8 Index);
	UFUNCTION(BlueprintCallable)
	void EnableFrozeOnAllHTargets(float Duration, float FrequencySeconds, float FrequencyDamage);

	UFUNCTION(BlueprintCallable)
	void EnableFrozeOnRef(AActor* LActor, float Duration, float FrequencySeconds, float FrequencyDamage);
	UFUNCTION(BlueprintCallable)
	void ClearFrozeOnRef(AActor* LActor);

	UFUNCTION(BlueprintCallable)
	void EnableBurning(float Duration, float FrequencySeconds, float FrequencyDamage);
	UFUNCTION(BlueprintCallable)
	void EnableBurningOnIndex(float Duration, float FrequencySeconds, float FrequencyDamage, uint8 Index);
	UFUNCTION(BlueprintCallable)
	void EnableBurningOnAllHTargets(float Duration, float FrequencySeconds, float FrequencyDamage);

	UFUNCTION(BlueprintCallable)
	void EnableBurningOnRef(AActor* LActor, float Duration, float FrequencySeconds, float FrequencyDamage);
	UFUNCTION(BlueprintCallable)
	void ClearBurningOnRef(AActor* LActor);

	/**
	 * KNOCKDOWN
	 */
	
	UFUNCTION(BlueprintCallable)
	void EnableKnockdown(bool bForward);
	UFUNCTION(BlueprintCallable)
	void EnableKnockdownOnIndex(bool bForward, uint8 Index);

	/**
	 * VECTOR CALCULATIONS
	 */

	/** 
	 * dönüş olarak damage alanın önündenmi yoksa arkasındanmı aldığı bilgisini verir.
	 * true -> önünden hit almış demektir
	 */
	UFUNCTION(BlueprintCallable)
	bool CalculateForwardOrBackwardHit(const AActor* DamagedActor, const FVector& HitLocation) const ;

	UFUNCTION(BlueprintCallable)
	FVector GetLocationUnderCrosshair() const;

	/**
	 * Characters Special Skills Handle  
	 */
	
	UFUNCTION(BlueprintCallable)
	bool bTargetCanExecutable(AActor* LActor) const;

	UFUNCTION(BlueprintCallable)
	void PerformGreatSwordExecutioner(AActor* LActor);

	UFUNCTION(BlueprintCallable)
	void PerformGreatSwordExecuted(AActor* LActor);

	UFUNCTION(BlueprintCallable)
	bool bTargetCanAmbushed(AActor* LActor) const;

	UFUNCTION(BlueprintCallable)
	void PerformDualSwordAmbush(AActor* LActor);

	UFUNCTION(BlueprintCallable)
	void PerformDualSwordAmbushed(AActor* LActor);

	/**
	 * Skill Coldowns
	 */

	UFUNCTION(BlueprintCallable)
	void ActivateSkillECooldown(); // Serverde Çağırdığından Emin ol

	UFUNCTION(BlueprintCallable)
	bool CheckCurrentManaAndReduce(float ManaCost) const;

	UFUNCTION(BlueprintCallable)
	float GetSkillEManaCost() const;

	








	
	
	

};
