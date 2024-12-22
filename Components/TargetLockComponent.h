// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetLockComponent.generated.h"


class IHitInterface;
class AHCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HANEDANRPG3_API UTargetLockComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UTargetLockComponent();
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

	/**
	 * FREE SPACE
	 */

	UPROPERTY(Replicated)
	bool bTargeting = false;

	UFUNCTION(Server,Reliable)
	void Server_EnableTargeting();

	UFUNCTION(Client,Reliable)
	void Client_SetCameraHeightToTargeting();
	
	UFUNCTION(Server,Reliable)
	void Server_DisableTargeting();
	
	UFUNCTION(Client,Reliable)
	void Client_SetCameraHeightToDefault();

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	uint16 TraceLength = 500;

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	uint16 TraceRadius = 200;

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	uint16 InterpSpeed = 4;

	UPROPERTY()
	TArray<TEnumAsByte<EObjectTypeQuery>> TargetObjectTypes;
	
	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(Replicated)
	AActor* TargetingActor;

	void Delayed1();
	void Delayed2();
	
	
	

		
};
