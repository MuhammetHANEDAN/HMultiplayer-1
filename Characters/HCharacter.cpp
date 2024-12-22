// Fill out your copyright notice in the Description page of Project Settings.


#include "HCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HANEDANRPG3/Components/CombatComponent.h"
#include "HANEDANRPG3/Components/GamePlayEffectsComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "HAnimInstance.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetComponent.h"
#include "HANEDANRPG3/Components/TargetLockComponent.h"
#include "HANEDANRPG3/HUD/CharacterOverlayWidget.h"
#include "HANEDANRPG3/HUD/GenelWidget.h"
#include "HANEDANRPG3/HUD/HHUD.h"
#include "HANEDANRPG3/PlayerControllers/HPlayerController.h"
#include "HANEDANRPG3/Weapons/Weapon.h"
#include "Net/UnrealNetwork.h"


AHCharacter::AHCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;

	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement=true;

	CameraBoom=CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength=600.f;
	CameraBoom->bUsePawnControlRotation=true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
	FollowCamera->bUsePawnControlRotation = false;

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
	CombatComponent->SetIsReplicated(true);

	GamePlayEffectsComponent = CreateDefaultSubobject<UGamePlayEffectsComponent>(TEXT("GamePlay Effects Component"));
	GamePlayEffectsComponent->SetIsReplicated(true);

	TargetLockComponent = CreateDefaultSubobject<UTargetLockComponent>(TEXT("Target Lock Component"));
	TargetLockComponent->SetIsReplicated(true);

	CharacterOverlayWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("CharacterOverlay Widget Comp"));
	CharacterOverlayWidgetComponent->SetupAttachment(GetRootComponent());
	CharacterOverlayWidgetComponent->SetIsReplicated(true);
	
	GetMesh()->SetCollisionObjectType(NAME_SkeletalMesh);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);

	LockOnIconWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOn Icon Widget Comp"));
	LockOnIconWidgetComponent->SetupAttachment(GetRootComponent());
	LockOnIconWidgetComponent->SetVisibility(false);

}

void AHCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		HPlayerController = Cast<AHPlayerController>(Controller);
		
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
		if(Controller && Controller->IsLocalController())
		{
			HudReferenceWorks();
		}
	}
	else
	{
		FTimerHandle PETimerHandle;
		GetWorldTimerManager().SetTimer(PETimerHandle,this,&AHCharacter::InitializePlayerControllerAndEnhancedInput,0.2f);
	}

	FTimerHandle COWR_TimerHandle;
	GetWorldTimerManager().SetTimer(COWR_TimerHandle,this,&AHCharacter::InitializeCharacterOverlayWidgetReference,0.2f);
	
}

void AHCharacter::InitializePlayerControllerAndEnhancedInput()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		HPlayerController = Cast<AHPlayerController>(Controller);
		
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
		if(Controller && Controller->IsLocalController())
		{
			HudReferenceWorks();
		}
	}
}

void AHCharacter::Multicast_SetActorRotation_Implementation(const FRotator& NewRotation)
{
	SetActorRotation(NewRotation);
}

void AHCharacter::Multicast_SetUseControllerYaw_Implementation(bool bUseYaw)
{
	bUseControllerRotationYaw = bUseYaw;
}

void AHCharacter::HudReferenceWorks()
{
	if(AHPlayerController* L_HplayerController = Cast<AHPlayerController>(Controller))
	{
		HPlayerController = L_HplayerController;
		HHudReference = Cast<AHHUD>(HPlayerController->GetHUD());
			
		if(HHudReference)
		{
			HHudReference->OwnerHCharacter=this;
			HHudReference->HPlayerController = HPlayerController;
			if(CombatComponent)
			{
				CombatComponent->OwnerHHUD = HHudReference;
				
				HHudReference->OwnersCombatComponent = CombatComponent;
			}
			HHudReference->OwnerHCharacter = this; // hudun içindeki ownerhcharacteri setledik
			// şimdi initializegenelwidget karakterde bulunan taglere göre farklı widgetleri oluşturacak
			HHudReference->InitializeGenelWidget();

			HPlayerController->HHUD = HHudReference;
			HPlayerController->OwnerCombatComponent = CombatComponent;
		}
	}
}

void AHCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHCharacter::PostInitializeComponents() // TODO başta controller setlenmezse gameplayeffectcomponent içindeki hplayer controller null kalabilir
{
	Super::PostInitializeComponents();
	if(CombatComponent)
	{
		CombatComponent->OwnerHCharacter = this;
	}
	if(GamePlayEffectsComponent)
	{
		if(Controller && Controller->IsLocalController())
		{
			if(AHPlayerController* QController = Cast<AHPlayerController>(Controller))
			{
				GamePlayEffectsComponent->OwnerHPlayerController = QController;
			}
		}
		GamePlayEffectsComponent->OwnerHCharacter = this;
		GamePlayEffectsComponent->OwnerCombatComponent = CombatComponent;
		GamePlayEffectsComponent->OwnerMeshComponent = GetMesh();
	}
	if(TargetLockComponent)
	{
		TargetLockComponent->OwnerHCharacter=this;
	}
	
}

void AHCharacter::InitializeCharacterOverlayWidgetReference()
{
	if(CharacterOverlayWidgetComponent && CharacterOverlayWidgetComponent->GetWidget())
	{
		CharacterOverlayWidget = Cast<UCharacterOverlayWidget>(CharacterOverlayWidgetComponent->GetWidget());

		if(Controller && Controller->IsLocalController() && CharacterOverlayWidget)
		{
			CharacterOverlayWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		
		if(CharacterOverlayWidget)
		{
			CharacterOverlayWidget->ShieldBar->SetVisibility(ESlateVisibility::Hidden);
			CombatComponent->OwnerCharacterOverlayWidget = CharacterOverlayWidget;
			CombatComponent->UpdateCharacterOverlayHPBar();
			CombatComponent->UpdateCharacterOverlayMPBar();
			CombatComponent->UpdateCharacterOverlayShieldBar();
		}
		
	}
	
}

void AHCharacter::ActivateLockOnIcon()
{
	if(LockOnIconWidgetComponent)
	{
		LockOnIconWidgetComponent->SetVisibility(true);
	}
}

void AHCharacter::DeactivateLockOnIcon()
{
	if(LockOnIconWidgetComponent)
	{
		LockOnIconWidgetComponent->SetVisibility(false);
	}
}

void AHCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHCharacter,bIsAlive);
	DOREPLIFETIME(AHCharacter,CurrentCharacterState);
	DOREPLIFETIME(AHCharacter,CurrentCharacterActionStateStruct);
	DOREPLIFETIME(AHCharacter,bTargeting);
	DOREPLIFETIME(AHCharacter,bTargeted);
	DOREPLIFETIME(AHCharacter,bVisible);
	DOREPLIFETIME(AHCharacter,bOnFinisherDying)
}


void AHCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHCharacter::Look);

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AHCharacter::Jump);

		//Equping
		EnhancedInputComponent->BindAction(EquipAction,ETriggerEvent::Started,this,&AHCharacter::I_Equip);

		//Targeting
		EnhancedInputComponent->BindAction(EnableTargetingAction,ETriggerEvent::Started,this,&AHCharacter::I_EnableTargeting);
		EnhancedInputComponent->BindAction(DisableTargetingAction,ETriggerEvent::Started,this,&AHCharacter::I_DisableTargeting);

		//Normal Attack
		EnhancedInputComponent->BindAction(NormalAttackAction,ETriggerEvent::Started,this,&AHCharacter::I_NormalAttack);

		//Switch Camera Mode
		EnhancedInputComponent->BindAction(RightClickAction,ETriggerEvent::Started,this,&AHCharacter::I_RightClickOn);
		EnhancedInputComponent->BindAction(RightClickAction,ETriggerEvent::Completed,this,&AHCharacter::I_RightClickOff);
		EnhancedInputComponent->BindAction(RightClickAction,ETriggerEvent::Canceled,this,&AHCharacter::I_RightClickOff);

		//Dash
		EnhancedInputComponent->BindAction(DashAction,ETriggerEvent::Started,this,&AHCharacter::I_Dash);

		//Block
		EnhancedInputComponent->BindAction(BlockAction,ETriggerEvent::Started,this,&AHCharacter::I_Block);

		//Skills
		EnhancedInputComponent->BindAction(Skill1Action,ETriggerEvent::Started,this,&AHCharacter::I_Skill1);
		EnhancedInputComponent->BindAction(Skill2Action,ETriggerEvent::Started,this,&AHCharacter::I_Skill2);
		EnhancedInputComponent->BindAction(Skill3Action,ETriggerEvent::Started,this,&AHCharacter::I_Skill3);
		EnhancedInputComponent->BindAction(Skill4Action,ETriggerEvent::Started,this,&AHCharacter::I_Skill4);
		EnhancedInputComponent->BindAction(SkillEAction,ETriggerEvent::Started,this,&AHCharacter::I_SkillE);

		//Camera Offset
		EnhancedInputComponent->BindAction(ChangeCameraOffsetAction,ETriggerEvent::Triggered,this,&AHCharacter::I_ChangeCameraOffset);
		
	}
}

void AHCharacter::Move(const FInputActionValue& Value)
{
	FVector2d MoveVector = Value.Get<FVector2d>();
	ForwardOrBackward = MoveVector.Y;
	RightOrLeft = MoveVector.X;

	if(GetController()!=nullptr && bIsAlive && bCanDoMoveActionsCharacterActionStateStruct())
	{
		FRotator ControllerRotation = GetControlRotation();
		FRotator YawRotation = FRotator(0,ControllerRotation.Yaw,0);

		FVector FrontDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(FrontDirection,ForwardOrBackward);
		AddMovementInput(RightDirection,RightOrLeft);
	}
}

void AHCharacter::Look(const FInputActionValue& Value)
{
	FVector2d MoveVector = Value.Get<FVector2d>();
	float PitchScale = -MoveVector.Y;
	float YawScale = MoveVector.X;

	// Finisherda kamerayı döndürebilmek için bcanmoveactioncharacter state fonku kullanmak yerine b1 kullandık.
	bool b1 = (!CurrentCharacterActionStateStruct.bFrozed &&
		!CurrentCharacterActionStateStruct.bStunned &&
		!CurrentCharacterActionStateStruct.bOnFall);
	
	if(GetController()!=nullptr && bIsAlive && !bTargeting && b1)
	{
		AddControllerPitchInput(PitchScale);
		AddControllerYawInput(YawScale);
	}
}

void AHCharacter::Jump()
{
	if(!bCanDoMoveActionsCharacterActionStateStruct()) return;
	Super::Jump();
}

bool AHCharacter::bCanCastCurrentCharacterState()
{
	return(CurrentCharacterState != ECharacterState::ECS_Equiping &&
		CurrentCharacterState!= ECharacterState::ECS_Blocking &&
		CurrentCharacterState != ECharacterState::ECS_Casting);
}

bool AHCharacter::bCanDoMoveActionsCharacterActionStateStruct()
{
	return (!CurrentCharacterActionStateStruct.bFrozed &&
		!CurrentCharacterActionStateStruct.bStunned &&
		!CurrentCharacterActionStateStruct.bBeingExecuted &&
		!CurrentCharacterActionStateStruct.bOnExecution &&
		!CurrentCharacterActionStateStruct.bOnFall);
}

bool AHCharacter::bCanCastActionStateStruct()
{
	return (!CurrentCharacterActionStateStruct.bFrozed &&
		!CurrentCharacterActionStateStruct.bSilenced &&
		!CurrentCharacterActionStateStruct.bStunned &&
		!CurrentCharacterActionStateStruct.bBeingExecuted &&
		!CurrentCharacterActionStateStruct.bOnExecution &&
		!CurrentCharacterActionStateStruct.bOnFall);
}

void AHCharacter::RefreshCurrentCharacterActionStateStruct()
{
	FCharacterActionStateStruct NewStruct;

	CurrentCharacterActionStateStruct = NewStruct;
}

bool AHCharacter::bCanNormalAttack()
{
	if(ActorHasTag(FName("MageCharacter")))
	{
		if(GetCharacterMovement()->IsFalling() || !bCanDoMoveActionsCharacterActionStateStruct()) return false;
		return(CombatComponent->bWeaponOnHand && CurrentCharacterState == ECharacterState::ECS_Unoccupied);
	}
	else
	{
		if(CombatComponent->MaxAttackAnimAmount== 0 || GetCharacterMovement()->IsFalling() || !bCanDoMoveActionsCharacterActionStateStruct()) return false;
		return(CombatComponent->bWeaponOnHand && CurrentCharacterState == ECharacterState::ECS_Unoccupied);
	}
	
	
}

bool AHCharacter::bCanTargeting()
{
	if(bCanDoMoveActionsCharacterActionStateStruct() && bIsAlive && CombatComponent->bWeaponOnHand)
	{
		return true;
	}
	return false;
}

bool AHCharacter::bCanEquipWeapon()
{
	if(CombatComponent && CombatComponent->FirstWeapon != nullptr && bCanDoMoveActionsCharacterActionStateStruct() && 
		CurrentCharacterState == ECharacterState::ECS_Unoccupied)
	{
		return true;
	}
	return false;
}

void AHCharacter::I_Equip()
{
	if(bCanEquipWeapon() == false) return;
	CombatComponent->Server_PlayEquipWeaponMontageAndSetMoveMode();
}

void AHCharacter::I_EnableTargeting()
{
	if(bCanTargeting() == false) return;
	if(TargetLockComponent)
	{
		TargetLockComponent->Server_EnableTargeting();
	}
}

void AHCharacter::I_DisableTargeting()
{
	if(bCanTargeting() == false) return;
	if(TargetLockComponent)
	{
		TargetLockComponent->Server_DisableTargeting();
	}
}

void AHCharacter::SetCharacterVisibility(bool visible)
{
	if(HasAuthority() && Controller->IsLocalController())
	{
		SetScreenInvisibilityEffectWidget(!visible);
		return; // serverdeki oyuncu ise return
	}
	
	GetMesh()->SetVisibility(visible);

	if(LockOnIconWidgetComponent)
	{
		// targetlanmışken invisible moda geçerse ---- invisible moda geçince zaten target düşecek ve targetlanamıcak
		if(bTargeted && !visible)
		{
			LockOnIconWidgetComponent->SetVisibility(false);
			if(TargetedAhCharacter)
			{
				TargetedAhCharacter->TargetLockComponent->Server_DisableTargeting();
			}
		}
		// normal modda invisible mode geçerse 
		else
		{
			LockOnIconWidgetComponent->SetVisibility(false);
		}
		
	}
		
	if(Controller && CharacterOverlayWidgetComponent)
	{
		if(!(GetController()->IsLocalController())) // serverdeki simulatedp
		{
			CharacterOverlayWidgetComponent->SetVisibility(visible);
			if(CharacterOverlayWidget)
			{
				if(visible)
				{
					CharacterOverlayWidget->SetVisibility(ESlateVisibility::Visible);
				}
				else
				{
					CharacterOverlayWidget->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}
	}
		
	if(CombatComponent)
	{
		if(CombatComponent->FirstWeapon)
		{
			CombatComponent->FirstWeapon->WeaponStaticMeshComponent->SetVisibility(visible);
			CombatComponent->FirstWeapon->SetVisibilityProps(visible);
		}
		if(CombatComponent->SecondWeapon)
		{
			CombatComponent->SecondWeapon->WeaponStaticMeshComponent->SetVisibility(visible);
			CombatComponent->SecondWeapon->SetVisibilityProps(visible);
		}
	}
}

void AHCharacter::Server_SetCharacterVisibility_Implementation(bool visible)
{
	if(bVisible == visible) return;
	
	bVisible = visible;

	SetCharacterVisibility(visible);
	
}

void AHCharacter::OnRep_bVisible()
{
	if(Controller && Controller->IsLocalController())
	{
		SetScreenInvisibilityEffectWidget(!bVisible);
		return; // clientteki local oyuncudur
	}
	
	GetMesh()->SetVisibility(bVisible);

	if(LockOnIconWidgetComponent)
	{
		// targetlanmışken invisible moda geçerse ---- invisible moda geçince zaten target düşecek ve targetlanamıcak
		if(bTargeted && !bVisible)
		{
			LockOnIconWidgetComponent->SetVisibility(false);
			if(TargetedAhCharacter)
			{
				TargetedAhCharacter->TargetLockComponent->Server_DisableTargeting();
			}
		}
		// normal modda invisible mode geçerse 
		else
		{
			LockOnIconWidgetComponent->SetVisibility(false);
		}
		
	}
		
	if(CharacterOverlayWidgetComponent)
	{
		if(Controller && Controller->IsLocalController()) // clientteki local oyuncudur
		{
			
		}
		else // simulatedp
		{
			UE_LOG(LogTemp,Error,TEXT("gelindi1"));
			CharacterOverlayWidgetComponent->SetVisibility(bVisible);
			if(CharacterOverlayWidget)
			{
				UE_LOG(LogTemp,Error,TEXT("gelindi2"));
				if(bVisible)
				{
					CharacterOverlayWidget->SetVisibility(ESlateVisibility::Visible);
				}
				else
				{
					CharacterOverlayWidget->SetVisibility(ESlateVisibility::Hidden);
					UE_LOG(LogTemp,Error,TEXT("gelindi3"));
				}
			}
		}
	}
		
	if(CombatComponent)
	{
		if(CombatComponent->FirstWeapon)
		{
			CombatComponent->FirstWeapon->WeaponStaticMeshComponent->SetVisibility(bVisible);
			CombatComponent->FirstWeapon->SetVisibilityProps(bVisible);
		}
		if(CombatComponent->SecondWeapon)
		{
			CombatComponent->SecondWeapon->WeaponStaticMeshComponent->SetVisibility(bVisible);
			CombatComponent->SecondWeapon->SetVisibilityProps(bVisible);
		}
	}
}

void AHCharacter::Interface_BrokeInvisibility()
{
	if(!bVisible)
	{
		Multicast_BrokeInvisibility();
	}
}

void AHCharacter::Multicast_BrokeInvisibility_Implementation()
{
	
	GetMesh()->SetVisibility(true);

	if(CombatComponent)
	{
		if(CombatComponent->FirstWeapon)
		{
			CombatComponent->FirstWeapon->WeaponStaticMeshComponent->SetVisibility(true);
			CombatComponent->FirstWeapon->SetVisibilityProps(true);
		}
		if(CombatComponent->SecondWeapon)
		{
			CombatComponent->SecondWeapon->WeaponStaticMeshComponent->SetVisibility(true);
			CombatComponent->SecondWeapon->SetVisibilityProps(true);
		}
	}

	// Character Overlay Widget (Barların) işlemleri

	if(HasAuthority() && Controller->IsLocalController()) // serverdeki local oyuncu
	{
		
	}
	else if(HasAuthority() && !Controller->IsLocalController()) // serverdeki simulted p
	{
		CharacterOverlayWidgetComponent->SetVisibility(true);
		if(CharacterOverlayWidget)
		{
			CharacterOverlayWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else if(!HasAuthority() && Controller) // cliennteki local oyuncu
	{
		
	}
	else // clientteki simulated p
	{
		CharacterOverlayWidgetComponent->SetVisibility(true);
		if(CharacterOverlayWidget)
		{
			CharacterOverlayWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
	
}

void AHCharacter::OnRep_bTargeting()
{
	
}

void AHCharacter::I_NormalAttack()
{
	if(bCanNormalAttack() == false) return;
	SetScreenInvisibilityEffectWidget(false);
	CombatComponent->RequestNormalAttack();
}

void AHCharacter::I_RightClickOn()
{
	if(bCanSwitchCameraMode())
	{
		SetCameraToAimingMode();
	}
}

void AHCharacter::I_RightClickOff()
{
	if(bCanSwitchCameraMode())
	{
		SetCameraToOldMode();
	}
}

void AHCharacter::SetCameraToOldMode()
{
	if(HHudReference)
	{
		HHudReference->bCrosshairEnabled = false;
	}
	CameraBoom->TargetArmLength =OldTargetArmLength;
	CameraBoom->SocketOffset = FVector(0,0,0);
}

void AHCharacter::SetCameraToAimingMode()
{
	if(HHudReference)
	{
		HHudReference->bCrosshairEnabled = true;
	}
	OldTargetArmLength = CameraBoom->TargetArmLength;
	CameraBoom->TargetArmLength =350.f;
	CameraBoom->SocketOffset = FVector(0,110,60);
}

bool AHCharacter::bCanSwitchCameraMode()
{
	return true;
}

void AHCharacter::Server_SetCurrentCharacterState_Implementation(ECharacterState NewCharacterState)
{
	CurrentCharacterState = NewCharacterState;
}

void AHCharacter::Server_SetCurrentCharacterActionStateStruct_Implementation(const ECAS& ChangeActionStructValue,
                                                                             bool bNewValue)
{
	switch (ChangeActionStructValue)
	{
	default:
		CurrentCharacterActionStateStruct.bBurning = bNewValue;
		break;
	case ECAS::ECAS_Burning:
		CurrentCharacterActionStateStruct.bBurning = bNewValue;
		break;
	case ECAS::ECAS_Frozen:
		CurrentCharacterActionStateStruct.bFrozed = bNewValue;
		break;
	case ECAS::ECAS_Silenced:
		CurrentCharacterActionStateStruct.bSilenced = bNewValue;
		break;
	case ECAS::ECAS_Slowed:
		CurrentCharacterActionStateStruct.bSlowed = bNewValue;
		break;
	case ECAS::ECAS_Stunned:
		CurrentCharacterActionStateStruct.bStunned = bNewValue;
		break;
	case ECAS::ECAS_BeingExecuting:
		CurrentCharacterActionStateStruct.bBeingExecuted = bNewValue;
		break;
	case ECAS::ECAS_OnFall:
		CurrentCharacterActionStateStruct.bOnFall = bNewValue;
		break;
	case ECAS::ECAS_OnExecution:
		CurrentCharacterActionStateStruct.bOnExecution = bNewValue;
		break;
	case ECAS::ECAS_OnSuperArmor:
		CurrentCharacterActionStateStruct.bOnSuperArmor = bNewValue;
		break;
	}
}

void AHCharacter::OnRep_CurrentCharacterActionStateStruct()
{
	
}

void AHCharacter::Die()
{
	bIsAlive = false;
	DisableCapsuleCollision();
	if(!bOnFinisherDying) // Finisherda olursa impulse uygulayacaz çünkü
	{
		EnablePhysicWhenDying();
	}
	DestroyHUDworksForDestroyHCHaracter();
	UnpossesHPlayerController();
	FTimerHandle DieTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DieTimerHandle,this,&AHCharacter::DestroyHCharacter,5.f);
}

void AHCharacter::OnRep_bIsAlive()
{
	DisableCapsuleCollision();
	if(!bOnFinisherDying) // Finisherda olursa impulse uygulayacaz çünkü
	{
		EnablePhysicWhenDying();
	}
	DestroyHUDworksForDestroyHCHaracter();
}

void AHCharacter::DisableCapsuleCollision()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if(CharacterOverlayWidgetComponent)
	{
		CharacterOverlayWidgetComponent->SetVisibility(false);
	}
	if(LockOnIconWidgetComponent)
	{
		LockOnIconWidgetComponent->SetVisibility(false);
	}
}

void AHCharacter::EnablePhysicWhenDying()
{
	
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetSimulatePhysics(true);

	if(CombatComponent->FirstWeapon)
	{
		FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
		CombatComponent->FirstWeapon->DetachFromActor(DetachRules);
		CombatComponent->FirstWeapon->WeaponStaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
		CombatComponent->FirstWeapon->WeaponStaticMeshComponent->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
		CombatComponent->FirstWeapon->WeaponStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CombatComponent->FirstWeapon->WeaponStaticMeshComponent->SetEnableGravity(true);
		CombatComponent->FirstWeapon->WeaponStaticMeshComponent->SetSimulatePhysics(true);
	}
	if(CombatComponent->SecondWeapon)
	{
		FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
		CombatComponent->SecondWeapon->DetachFromActor(DetachRules);
		CombatComponent->SecondWeapon->WeaponStaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
		CombatComponent->SecondWeapon->WeaponStaticMeshComponent->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
		CombatComponent->SecondWeapon->WeaponStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CombatComponent->SecondWeapon->WeaponStaticMeshComponent->SetEnableGravity(true);
		CombatComponent->SecondWeapon->WeaponStaticMeshComponent->SetSimulatePhysics(true);
	}
}

void AHCharacter::EnableDieAndPhysicImpulseWhenOnFinisher(float ImpulseScale, bool bImpulseFromForward)
{
	bOnFinisherDying = true;
	Die();
	Multicast_EnableDieAndPhysicImpulseWhenOnFinisher(ImpulseScale,bImpulseFromForward);
}

void AHCharacter::Multicast_EnableDieAndPhysicImpulseWhenOnFinisher_Implementation(float ImpulseScale,
	bool bImpulseFromForward)
{

	FVector ActorForwarOrBackVector;
	
	if(bImpulseFromForward)
	{
		ActorForwarOrBackVector = GetActorForwardVector();
	}
	else
	{
		ActorForwarOrBackVector = GetActorForwardVector()*-1;
	}
	
	ActorForwarOrBackVector.Y = 0;
	ActorForwarOrBackVector.Z=0;
	FVector ImpulseVector = ActorForwarOrBackVector*1000*ImpulseScale;

	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->AddImpulse(ImpulseVector);

	if(CombatComponent->FirstWeapon)
	{
		FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
		CombatComponent->FirstWeapon->DetachFromActor(DetachRules);
		CombatComponent->FirstWeapon->WeaponStaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
		CombatComponent->FirstWeapon->WeaponStaticMeshComponent->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
		CombatComponent->FirstWeapon->WeaponStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CombatComponent->FirstWeapon->WeaponStaticMeshComponent->SetEnableGravity(true);
		CombatComponent->FirstWeapon->WeaponStaticMeshComponent->SetSimulatePhysics(true);
		CombatComponent->FirstWeapon->WeaponStaticMeshComponent->AddImpulse(ImpulseVector);
	}
	if(CombatComponent->SecondWeapon)
	{
		FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
		CombatComponent->SecondWeapon->DetachFromActor(DetachRules);
		CombatComponent->SecondWeapon->WeaponStaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
		CombatComponent->SecondWeapon->WeaponStaticMeshComponent->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
		CombatComponent->SecondWeapon->WeaponStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CombatComponent->SecondWeapon->WeaponStaticMeshComponent->SetEnableGravity(true);
		CombatComponent->SecondWeapon->WeaponStaticMeshComponent->SetSimulatePhysics(true);
		CombatComponent->SecondWeapon->WeaponStaticMeshComponent->AddImpulse(ImpulseVector);
	}
}

void AHCharacter::DestroyHCharacter()
{
	if(CombatComponent->FirstWeapon)
	{
		CombatComponent->FirstWeapon->Destroy();
	}
	if(CombatComponent->SecondWeapon)
	{
		CombatComponent->SecondWeapon->Destroy();
	}
	
	Destroy();
}

void AHCharacter::UnpossesHPlayerController()
{
	if(Controller)
	{
		Controller->UnPossess();
	}
}

void AHCharacter::Destroyed()
{
	Super::Destroyed();
}

void AHCharacter::DestroyHUDworksForDestroyHCHaracter()
{
	if(HHudReference)
	{
		HHudReference->DestroyGenelWidget();
	}
}

void AHCharacter::GetHit(const float Damage, const FVector& HitLocation, const EHitType HitType, const AActor* DamageCauserActor)
{
	if(HasAuthority())
	{
		if(CombatComponent)
		{
			CombatComponent->GetDamage(Damage,HitLocation,HitType,DamageCauserActor);
		}
	}
}

FName AHCharacter::GetDashDirectionName()
{
	if(ForwardOrBackward==1 && RightOrLeft==0)
	{
		return "Forward";
	}
	if(ForwardOrBackward==1 && RightOrLeft==1)
	{
		return "Right";
	}
	if(ForwardOrBackward==1 && RightOrLeft==-1)
	{
		return"Left";
	}
	if(ForwardOrBackward==0 && RightOrLeft==1)
	{
		return "Right";
	}
	if(ForwardOrBackward==0 && RightOrLeft==-1)
	{
		return"Left";
	}
	if(ForwardOrBackward==-1 && RightOrLeft==0)
	{
		return "Backward";
	}
	if(ForwardOrBackward==-1 && RightOrLeft==1)
	{
		return "Right";
	}
	if(ForwardOrBackward==-1 && RightOrLeft==-1)
	{
		return "Left";
	}
	return "Forward";
}

bool AHCharacter::CanDash()
{
	UE_LOG(LogTemp, Error, TEXT("bOnExecution = %s"), CurrentCharacterActionStateStruct.bOnExecution ? TEXT("true") : TEXT("false"));
	if(CurrentCharacterActionStateStruct.bOnExecution) return false;
	
	if(CurrentCharacterActionStateStruct.bFrozed || CurrentCharacterActionStateStruct.bStunned || CurrentCharacterActionStateStruct.bBeingExecuted &&
		CurrentCharacterActionStateStruct.bOnExecution || CurrentCharacterActionStateStruct.bOnFall)
	{
		UE_LOG(LogTemp,Error,TEXT("dash atamaz"));
		return false;
	}
	
	if(CurrentCharacterState != ECharacterState::ECS_Blocking && CurrentCharacterState != ECharacterState::ECS_Equiping && !GetCharacterMovement()->IsFalling() && bIsAlive && bCanDoMoveActionsCharacterActionStateStruct() &&
		CombatComponent->bWeaponOnHand && GetVelocity().Length()>0.f) return true;
	
	if(CurrentCharacterState == ECharacterState::ECS_Blocking && CurrentCharacterState != ECharacterState::ECS_Equiping && !GetCharacterMovement()->IsFalling() && bIsAlive && bCanDoMoveActionsCharacterActionStateStruct() &&
		CombatComponent->bWeaponOnHand) return true;
	
	return false;
}

bool AHCharacter::bCanDash()
{
	UE_LOG(LogTemp, Error, TEXT("bOnExecution = %s"), CurrentCharacterActionStateStruct.bOnExecution ? TEXT("true") : TEXT("false"));
	if(CurrentCharacterActionStateStruct.bOnExecution) return false;
	
	if(CurrentCharacterActionStateStruct.bFrozed || CurrentCharacterActionStateStruct.bStunned || CurrentCharacterActionStateStruct.bBeingExecuted &&
		CurrentCharacterActionStateStruct.bOnExecution || CurrentCharacterActionStateStruct.bOnFall)
	{
		UE_LOG(LogTemp,Error,TEXT("dash atamaz"));
		return false;
	}
	
	if(CurrentCharacterState != ECharacterState::ECS_Blocking && CurrentCharacterState != ECharacterState::ECS_Equiping && !GetCharacterMovement()->IsFalling() && bIsAlive && bCanDoMoveActionsCharacterActionStateStruct() &&
		CombatComponent->bWeaponOnHand && GetVelocity().Length()>0.f) return true;
	
	if(CurrentCharacterState == ECharacterState::ECS_Blocking && CurrentCharacterState != ECharacterState::ECS_Equiping && !GetCharacterMovement()->IsFalling() && bIsAlive && bCanDoMoveActionsCharacterActionStateStruct() &&
		CombatComponent->bWeaponOnHand) return true;
	
	return false;
}

void AHCharacter::I_Dash()
{
	if(bCanDash())
	{
		Server_PerformDash(GetDashDirectionName());
	}
}

void AHCharacter::I_Block()
{
	if(CurrentCharacterState == ECharacterState::ECS_Blocking && bCanDoMoveActionsCharacterActionStateStruct() && CombatComponent->bWeaponOnHand
		&& CombatComponent->CurrentBlockAmount > 0)
	{
		CombatComponent->Server_CloseBlock();
	}
	
	else if(bCanBlock())
	{
		CombatComponent->Server_PerformBlock();
	}
}

bool AHCharacter::bCanBlock()
{
	bool b1 = (CurrentCharacterState != ECharacterState::ECS_Blocking &&  CurrentCharacterState != ECharacterState::ECS_Dashing &&
	CurrentCharacterState != ECharacterState::ECS_Equiping && CombatComponent->bWeaponOnHand
	&& bCanDoMoveActionsCharacterActionStateStruct() && bVisible);

	return b1;
}

bool AHCharacter::bCanCast()
{
	return (bCanCastActionStateStruct() && bCanCastCurrentCharacterState() && CombatComponent->bWeaponOnHand);
}

void AHCharacter::I_Skill1()
{
	if(!bCanCast()) return;
	if(!bVisible)
	{
		SetScreenInvisibilityEffectWidget(false);
		Server_SetCharacterVisibility(true);
	}
	CombatComponent->Server_CastSkill(1);
}

void AHCharacter::I_Skill2()
{
	if(!bCanCast()) return;
	if(!bVisible)
	{
		SetScreenInvisibilityEffectWidget(false);
		Server_SetCharacterVisibility(true);
	}
	CombatComponent->Server_CastSkill(2);
}

void AHCharacter::I_Skill3()
{
	if(!bCanCast()) return;
	if(!bVisible)
	{
		SetScreenInvisibilityEffectWidget(false);
		Server_SetCharacterVisibility(true);
	}
	CombatComponent->Server_CastSkill(3);
}

void AHCharacter::I_Skill4()
{
	if(!bCanCast()) return;
	if(!bVisible)
	{
		SetScreenInvisibilityEffectWidget(false);
		Server_SetCharacterVisibility(true);
	}
	CombatComponent->Server_CastSkill(4);
}

void AHCharacter::I_SkillE()
{
	if(!bCanCast()) return;
	if(ActorHasTag(FName("GreatSwordCharacter")))
	{
		CombatComponent->Server_CastSkillEGreatSword();
	}
	else if(ActorHasTag(FName("DualSwordCharacter")))
	{
		if(bVisible) return; // dualsword karakter invisible olmadan atamaz bu skili.
		SetScreenInvisibilityEffectWidget(false);
		CombatComponent->Server_CastSkillEDualSword();
	}
	else if(ActorHasTag(FName("MageCharacter")))
	{
		CombatComponent->Server_PerformMageESkill();
	}
}

void AHCharacter::I_ChangeCameraOffset(const FInputActionValue& Value)
{
	if(CameraBoom)
	{
		float AddAmount = Value.Get<float>();
		AddAmount = AddAmount*50*-1;
		CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength+AddAmount,600,1500);
	}
}

void AHCharacter::Server_PerformDash_Implementation(const FName DirectionName)
{
	if(bCanDash() && !CombatComponent->bDashOnCooldown)
	{
		CombatComponent->bDashOnCooldown = true;
		Multicast_PlayDashMontage(DirectionName);
		if(CombatComponent->CurrentBlockAmount > 0)
		{
			CombatComponent->CurrentBlockAmount = 0;
			CombatComponent->SetVisibilityOfShieldBar(false);
			
			CombatComponent->UpdateGenelWidgetShieldBar(); // current block amount rep notify ile clientlerı düzgün setlerken server setlenmemiş kalıyordu
		}
	}
}

void AHCharacter::Multicast_PlayDashMontage_Implementation(const FName DirectionName)
{
	if(UHAnimInstance* Ainstance = Cast<UHAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		if(DashOrRollMontage)
		{
			Ainstance->Montage_Play(DashOrRollMontage);
			Ainstance->Montage_JumpToSection(DirectionName,DashOrRollMontage);
		}
	}
}


