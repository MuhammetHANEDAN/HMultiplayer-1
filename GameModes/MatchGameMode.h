// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MatchGameMode.generated.h"

class AHCharacter;
/**
 * 
 */
UCLASS()
class HANEDANRPG3_API AMatchGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	
	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaSeconds) override;

	// TIME SYNC
	/** Menüden oyun başlangıç mapinin başlangıcına kadar geçen süre */
	UPROPERTY(BlueprintReadOnly)
	float GameStartedTime = 0.f;
	/** Mapin başlangıcından itibaren geçen süre */
	float ServerMapTime = 0.f;


	UPROPERTY(EditDefaultsOnly,Category="Properties")
	TSubclassOf<AHCharacter> GreatSwordCharacterClass;
	
	UPROPERTY(EditDefaultsOnly,Category="Properties")
	TSubclassOf<AHCharacter> DualSwordCharacterClass;

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	TSubclassOf<AHCharacter> MageCharacterClass;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	void MatchGameModeRestartPlayer(APlayerController* NewPlayer, const FName& CharacterClassName);

	virtual void RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot) override;

	virtual void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation) override;
	











	
	
	
};
