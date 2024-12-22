// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchGameMode.h"

#include "GameFramework/PlayerState.h"
#include "HANEDANRPG3/Characters/HCharacter.h"
#include "HANEDANRPG3/PlayerControllers/HPlayerController.h"

void AMatchGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	GameStartedTime = GetWorld()->GetRealTimeSeconds();

	
}

void AMatchGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ServerMapTime = GetWorld()->GetRealTimeSeconds() - GameStartedTime;
}

void AMatchGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	/*// If players should start as spectators, leave them in the spectator state
	if (!bStartPlayersAsSpectators && !MustSpectate(NewPlayer) && PlayerCanRestart(NewPlayer))
	{
		// Otherwise spawn their pawn immediately
		RestartPlayer(NewPlayer);
	}*/
}

void AMatchGameMode::MatchGameModeRestartPlayer(APlayerController* NewPlayer, const FName& CharacterClassName)
{
	if(GreatSwordCharacterClass && DualSwordCharacterClass && MageCharacterClass)
	{
		if(CharacterClassName == FName("GreatSword"))
		{
			DefaultPawnClass = GreatSwordCharacterClass;
		}
		else if(CharacterClassName == FName("DualSword"))
		{
			DefaultPawnClass = DualSwordCharacterClass;
		}
		else if(CharacterClassName == FName("Mage"))
		{
			DefaultPawnClass = MageCharacterClass;
		}
	}
	
	// If players should start as spectators, leave them in the spectator state
	if (!bStartPlayersAsSpectators && !MustSpectate(NewPlayer) && PlayerCanRestart(NewPlayer))
	{
		// Otherwise spawn their pawn immediately
		RestartPlayer(NewPlayer);
	}
}

void AMatchGameMode::RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot)
{
	if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
	{
		return;
	}

	if (!StartSpot)
	{
		UE_LOG(LogGameMode, Warning, TEXT("RestartPlayerAtPlayerStart: Player start not found"));
		return;
	}

	FRotator SpawnRotation = StartSpot->GetActorRotation();

	UE_LOG(LogGameMode, Verbose, TEXT("RestartPlayerAtPlayerStart %s"), (NewPlayer && NewPlayer->PlayerState) ? *NewPlayer->PlayerState->GetPlayerName() : TEXT("Unknown"));

	if (MustSpectate(Cast<APlayerController>(NewPlayer)))
	{
		UE_LOG(LogGameMode, Verbose, TEXT("RestartPlayerAtPlayerStart: Tried to restart a spectator-only player!"));
		return;
	}

	if (NewPlayer->GetPawn() != nullptr)
	{
		// If we have an existing pawn, just use it's rotation
		SpawnRotation = NewPlayer->GetPawn()->GetActorRotation();
	}
	else if (GetDefaultPawnClassForController(NewPlayer) != nullptr)
	{
		// Try to create a pawn to use of the default class for this player
		APawn* NewPawn = SpawnDefaultPawnFor(NewPlayer, StartSpot);
		if (IsValid(NewPawn))
		{
			NewPlayer->SetPawn(NewPawn);
		}
	}
	
	if (!IsValid(NewPlayer->GetPawn()))
	{
		FailedToRestartPlayer(NewPlayer);
	}
	else
	{
		// Tell the start spot it was used
		InitStartSpot(StartSpot, NewPlayer);

		FinishRestartPlayer(NewPlayer, SpawnRotation);
	}
}

void AMatchGameMode::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
{

	NewPlayer->Possess(NewPlayer->GetPawn());

	// If the Pawn is destroyed as part of possession we have to abort
	if (!IsValid(NewPlayer->GetPawn()))
	{
		FailedToRestartPlayer(NewPlayer);
	}
	else
	{
		// Set initial control rotation to starting rotation rotation
		NewPlayer->ClientSetRotation(NewPlayer->GetPawn()->GetActorRotation(), true);

		FRotator NewControllerRot = StartRotation;
		NewControllerRot.Roll = 0.f;
		NewPlayer->SetControlRotation(NewControllerRot);

		SetPlayerDefaults(NewPlayer->GetPawn());

		K2_OnRestartPlayer(NewPlayer);
		

		/*if(AHPlayerController* L_HPlayerController = Cast<AHPlayerController>(NewPlayer))
		{
			L_HPlayerController->ServerMapTime = ServerMapTime;
		}*/
		
	}
	
}
