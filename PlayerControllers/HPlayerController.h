// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HPlayerController.generated.h"

class UCombatComponent;
class AHHUD;
class AMatchGameMode;
/**
 * 
 */
UCLASS()
class HANEDANRPG3_API AHPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaSeconds) override;

	/**
	 *  Server-Client Time Sync
	 */

	/** Auth Player controller oyunun başında gidip servertime ı alır ve Cliente çoğaltır */
	float ServerMapTime = 0.f;

	UFUNCTION(Client,Reliable)
	void Client_ReceivedTime(float L_ServerMapTime);

	UFUNCTION(Server,Reliable)
	void Server_ReportTime();
	
	/** Servere yollanan rpc den sonra serverden cliente dönen rpc ye kadar geçen zamanın bulunması için
	 *  Servere yollanınca true serverden rpc alınınca false olacak.
	 */
	bool bServerRpcSendedTrack = false;
	float RoundTripTime = 0.f;
	
	UPROPERTY(EditAnywhere, Category="Properties")
	float CheckTimeSyncFrequency = 3.f;
	float CheckTimeSyncFrequencyTrack = 0.f;
	bool bTimeSycned = false;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* PlayerControllerDefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ESCAction;

	void ESCKeyPressed();

	virtual void SetupInputComponent() override;

public:
	/**
	 * references
	 */
	UPROPERTY()
	AMatchGameMode* MGameMode;

	UPROPERTY()
	AHHUD* HHUD;

	UPROPERTY()
	UCombatComponent* OwnerCombatComponent;

	
	UFUNCTION(Server,Reliable)
	void Server_RequestMatchGameModeSpawnCharacter(APlayerController* NewPlayer, const FName& CharacterClassName);

	void GetHHUDAfterDelay();
	FTimerHandle HUDTimerHandle;

	/**
	 *  GENEL WIDGET
	 */

	
	











	
	
};
