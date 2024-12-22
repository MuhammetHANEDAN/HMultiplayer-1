// Fill out your copyright notice in the Description page of Project Settings.


#include "HPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HANEDANRPG3/Characters/HCharacter.h"
#include "HANEDANRPG3/Components/CombatComponent.h"
#include "HANEDANRPG3/GameModes/MatchGameMode.h"
#include "HANEDANRPG3/HUD/ChooseClassWidget.h"
#include "HANEDANRPG3/HUD/HHUD.h"

void AHPlayerController::BeginPlay()
{
	Super::BeginPlay();


	MGameMode = GetWorld()->GetAuthGameMode<AMatchGameMode>();
	
	if(MGameMode) // Sadece serverda valid olacak
	{
		ServerMapTime = MGameMode->ServerMapTime;
	}
	
	if(!HasAuthority() &&  IsLocalPlayerController()) // Server değil ve Owner Client isek
	{
		ServerMapTime = 0.f;
		bServerRpcSendedTrack = true ;
		Server_ReportTime();
	}
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(PlayerControllerDefaultMappingContext, 0);
	}

	GetWorldTimerManager().SetTimer(HUDTimerHandle,this,&AHPlayerController::GetHHUDAfterDelay,0.2f);
	
}

void AHPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ServerMapTime += DeltaSeconds;

	if(bServerRpcSendedTrack) // Serverdada yapmamız önemsiz gibi o yüzden checklemedim
	{
		RoundTripTime += DeltaSeconds;
	}

	if(!HasAuthority() && IsLocalController() && bTimeSycned) // Server değil ve Owner Client isek
	{
		CheckTimeSyncFrequencyTrack += DeltaSeconds;
		if(CheckTimeSyncFrequencyTrack >= CheckTimeSyncFrequency)
		{
			Server_ReportTime();
			bTimeSycned = false;
		}
	}
	
}

void AHPlayerController::Server_ReportTime_Implementation()
{
	Client_ReceivedTime(ServerMapTime);
}

void AHPlayerController::Client_ReceivedTime_Implementation(float L_ServerMapTime)
{
	bServerRpcSendedTrack = false; // rpc servere gitti ve cliente geri göderildi
	ServerMapTime = L_ServerMapTime +(RoundTripTime/2); // Fazla veri kaybı var muhtemelen, veri kaybı yüzünden saniye gösterimi 1 saniyede geride kalıyodu  şu anlık böyle kalsın +1 koymak doğru bir çözüm değil
	if(OwnerCombatComponent)
	{
		OwnerCombatComponent->LastRoundTripTime = RoundTripTime;
	}
	RoundTripTime = 0;
	CheckTimeSyncFrequencyTrack = 0;
	bTimeSycned = true;
}

void AHPlayerController::GetHHUDAfterDelay()
{
	if(IsLocalController())
	{
		HHUD = Cast<AHHUD>(GetHUD());
	}
}

void AHPlayerController::ESCKeyPressed()
{
	if(HHUD && HHUD->ChooseClassWidget)
	{
		if(HHUD->ChooseClassWidget->IsVisible())
		{
			HHUD->SetChooseClassWidgetVisibilityAndUIProps(false);
			HHUD->SetgenelWidgetVisibility(true);
		}
		else
		{
			HHUD->SetChooseClassWidgetVisibilityAndUIProps(true);
			HHUD->SetgenelWidgetVisibility(false);
		}
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("hpc 41 hhud valid değil"));
	}
}

void AHPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(ESCAction,ETriggerEvent::Started,this,&AHPlayerController::ESCKeyPressed);
	}

}

void AHPlayerController::Server_RequestMatchGameModeSpawnCharacter_Implementation(APlayerController* NewPlayer,
                                                                                  const FName& CharacterClassName)
{
	if(GetPawn())
	{
		APawn* PossesedPawn = GetPawn();
		UnPossess();
		if(AHCharacter* PossesedHCharacter = Cast<AHCharacter>(PossesedPawn))
		{
			PossesedHCharacter->DestroyHCharacter();
		}
	}
	
	if(MGameMode)
	{
		MGameMode->MatchGameModeRestartPlayer(NewPlayer,CharacterClassName);
	}
	
	else
	{
		UE_LOG(LogTemp,Error,TEXT("MGAmemode valid değil hpc cpp 24"));
	}
}















