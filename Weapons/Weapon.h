// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class IHitInterface;
class AHCharacter;
class AHPlayerController;

UCLASS()
class HANEDANRPG3_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AWeapon();
	virtual void Tick(float DeltaTime) override;

protected:
	
	virtual void BeginPlay() override;

public:

	UPROPERTY()
	AHPlayerController* OwnerHPlayerController;
	
	IHitInterface* I_OwnerHCharacter = nullptr;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* WeaponStaticMeshComponent;

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	FName HandSocketName;
	UPROPERTY(EditDefaultsOnly,Category="Properties")
	FName BackSocketName;

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	float WeaponDamage = 20.f;
	
	/**
	 * WEAPON HIT TRACE
	 * Her makinede saldırı animasyonu oynayacak multicast ile ama sadece sunucuda hittrace çalıştırılacak ANS de
	 */

	void StartHitTrace();
	void EndHitTrace();
	bool bTracing = false;

	FVector TraceStart;
	FVector TraceEnd;
	UPROPERTY(EditDefaultsOnly,Category="Properties")
	float TraceRadius = 30.f;

	UPROPERTY()
	TArray<AActor*> HittedActors;

	UPROPERTY()
	TArray<TEnumAsByte<EObjectTypeQuery>> TargetObjectTypes;
	
	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY()
	TArray<FHitResult> HitResults;

	/**
	 * FREE SPACE
	 */

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void SetVisibilityProps(bool bVisible);

	
};
