// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"

#include "DamageIndicatorWidgetComponent.h"
#include "TargetLockComponent.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HANEDANRPG3/Actors/SkillActor.h"
#include "HANEDANRPG3/Characters/HAnimInstance.h"
#include "HANEDANRPG3/Characters/HCharacter.h"
#include "HANEDANRPG3/HUD/CharacterOverlayWidget.h"
#include "HANEDANRPG3/HUD/GenelWidget.h"
#include "HANEDANRPG3/HUD/HHUD.h"
#include "HANEDANRPG3/PlayerControllers/HPlayerController.h"
#include "HANEDANRPG3/Weapons/Weapon.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if(NormalAttackAnimMontageArray.Num()>=1)
	{
		MaxAttackAnimAmount = NormalAttackAnimMontageArray.Num()-1;
	}
	else
	{
		MaxAttackAnimAmount = 0;
	}

	if(OwnerHCharacter)
	{
		OwnerHMeshComponent = OwnerHCharacter->GetMesh();
		if(OwnerHMeshComponent)
		{
			OwnerHAnimInstance = Cast<UHAnimInstance>(OwnerHMeshComponent->GetAnimInstance());
		}
		
	}

	if(OwnerHCharacter && OwnerHCharacter->HasAuthority())
	{
		CurrentHP = MaxHP;
		CurrentMP = MaxMP;

		TemporaryMaxBlockAmount = MaxBlockAmount;

		OwnerHCharacter->GetCharacterMovement()->MaxWalkSpeed =NormalSpeed;
		CurrentSpeed = NormalSpeed;
		
	}

	FTimerHandle T_Handle;
	GetWorld()->GetTimerManager().SetTimer(T_Handle,this,&UCombatComponent::HandleBeginSpawnAndAttachToBackWeapon,0.2);
	
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(OwnerHPlayerController && OwnerHPlayerController->HasAuthority())
	{
		ServerMapTime = OwnerHPlayerController->ServerMapTime;
	}

	if(bDashOnCooldown)
	{
		DashTrack += DeltaTime;
		if(DashTrack >= DashCooldown)
		{
			DashTrack = 0;
			bDashOnCooldown = false;
		}
	}

	if(bSkill1OnCooldDown)
	{
		SetGenelWidgetSkill1Text(Skill1TimeRemainSeconds);
		Skill1TimeRemainSeconds -= DeltaTime;
		if(Skill1TimeRemainSeconds <= 0)
		{
			DeactivateSkill1Cooldown();
		}
	}

	if(bSkill2OnCooldDown)
	{
		SetGenelWidgetSkill2Text(Skill2TimeRemainSeconds);
		Skill2TimeRemainSeconds -= DeltaTime;
		if(Skill2TimeRemainSeconds <= 0)
		{
			DeactivateSkill2Cooldown();
		}
	}
	
	if(bSkill3OnCooldDown)
	{
		SetGenelWidgetSkill3Text(Skill3TimeRemainSeconds);
		Skill3TimeRemainSeconds -= DeltaTime;
		if(Skill3TimeRemainSeconds <= 0)
		{
			DeactivateSkill3Cooldown();
		}
	}

	if(bSkill4OnCooldDown)
	{
		SetGenelWidgetSkill4Text(Skill4TimeRemainSeconds);
		Skill4TimeRemainSeconds -= DeltaTime;
		if(Skill4TimeRemainSeconds <= 0)
		{
			DeactivateSkill4Cooldown();
		}
	}

	if(bSkillEOnCooldDown)
	{
		SetGenelWidgetSkillEText(SkillETimeRemainSeconds);
		SkillETimeRemainSeconds -= DeltaTime;
		if(SkillETimeRemainSeconds <= 0)
		{
			DeactivateSkillECooldown();
		}
	}
	
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCombatComponent,bWeaponOnHand);
	DOREPLIFETIME(UCombatComponent,NormalAttackNumber);
	DOREPLIFETIME(UCombatComponent,FirstWeapon);
	DOREPLIFETIME(UCombatComponent,SecondWeapon);
	DOREPLIFETIME(UCombatComponent,bMovementModeIsArmed);
	DOREPLIFETIME(UCombatComponent,CurrentHP);
	DOREPLIFETIME(UCombatComponent,CurrentMP);
	DOREPLIFETIME(UCombatComponent,CurrentBlockAmount);
	DOREPLIFETIME(UCombatComponent,CurrentSpeed);
}

void UCombatComponent::RequestNormalAttack()
{
	if(OwnerHCharacter->ActorHasTag(FName("MageCharacter"))) // yanlış olanı düzeltmek yerine hızlı yama
	{
		Server_NormalAttack();
		return;
	}
	if(MaxAttackAnimAmount == 0) return;
	Server_NormalAttack();
}

void UCombatComponent::Server_NormalAttack_Implementation()
{
	if(OwnerHCharacter->ActorHasTag(FName("MageCharacter"))) // yanlış olanı düzeltmek yerine hızlı yama
	{
		Multicast_PlayNormalAttackMontage();
		return;
	}
	if(!OwnerHCharacter->bVisible)
	{
		OwnerHCharacter->Multicast_BrokeInvisibility();
	}
	Multicast_PlayNormalAttackMontage();
	NormalAttackNumber++;
	if(NormalAttackNumber > MaxAttackAnimAmount)
	{
		NormalAttackNumber = 0;
	}
}

void UCombatComponent::Multicast_PlayNormalAttackMontage_Implementation()
{
	if(OwnerHCharacter->ActorHasTag(FName("MageCharacter"))) // yanlış olanı düzeltmek yerine hızlı yama
	{
		if(OwnerHAnimInstance && NormalAttackAnimMontageArray[0])
		{
			OwnerHAnimInstance->Montage_Play(NormalAttackAnimMontageArray[0],1);
		}
		return;
	}
	if(OwnerHAnimInstance && NormalAttackAnimMontageArray[NormalAttackNumber])
	{
		OwnerHAnimInstance->Montage_Play(NormalAttackAnimMontageArray[NormalAttackNumber],1);
	}
}

void UCombatComponent::RefreshNormalAttacks()
{
	if(GetOwner()->HasAuthority())
	{
		NormalAttackNumber = 0;
	}
}

void UCombatComponent::SetMovementMode(bool bToArmed)
{
	if(OwnerHCharacter==nullptr) return;
	if(bToArmed)
	{
		bMovementModeIsArmed = true;
		OwnerHCharacter->bUseControllerRotationYaw = true;
		OwnerHCharacter->GetCharacterMovement()->bOrientRotationToMovement=false;
	}
	else
	{
		bMovementModeIsArmed = false;
		OwnerHCharacter->bUseControllerRotationYaw = false;
		OwnerHCharacter->GetCharacterMovement()->bOrientRotationToMovement=true;
	}
}

void UCombatComponent::OnRep_MovementMode()
{
	if(bMovementModeIsArmed)
	{
		OwnerHCharacter->bUseControllerRotationYaw = true;
		OwnerHCharacter->GetCharacterMovement()->bOrientRotationToMovement=false;
	}
	else
	{
		OwnerHCharacter->bUseControllerRotationYaw = false;
		OwnerHCharacter->GetCharacterMovement()->bOrientRotationToMovement=true;
	}
}

void UCombatComponent::Server_PlayEquipWeaponMontageAndSetMoveMode_Implementation()
{
	if(bWeaponOnHand)
	{
		bWeaponOnHand = false;

		if(OwnerHAnimInstance && DisarmMontage)
		{
			OwnerHAnimInstance->Montage_Play(DisarmMontage);
		}

		SetMovementMode(false);

		if(OwnerHCharacter->TargetLockComponent->bTargeting)
		{
			OwnerHCharacter->TargetLockComponent->Server_DisableTargeting();
		}
	}
	else
	{
		bWeaponOnHand = true;
		
		if(OwnerHAnimInstance && EquipMontage)
		{
			OwnerHAnimInstance->Montage_Play(EquipMontage);
		}

		SetMovementMode(true);
	}
}

void UCombatComponent::OnRep_bWeaponOnHand()
{
	if(bWeaponOnHand)
	{
		if(OwnerHAnimInstance && EquipMontage)
		{
			OwnerHAnimInstance->Montage_Play(EquipMontage);
		}
	}
	else
	{
		if(OwnerHAnimInstance && DisarmMontage)
		{
			OwnerHAnimInstance->Montage_Play(DisarmMontage);
		}
	}
}

void UCombatComponent::EquipMontageCallNotify()
{
	if(OwnerHMeshComponent == nullptr) return;
	if(FirstWeapon)
	{
		FDetachmentTransformRules DetachmentTransformRules = FDetachmentTransformRules::KeepWorldTransform;
		FirstWeapon->DetachAllSceneComponents(OwnerHMeshComponent,DetachmentTransformRules);
		FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules::SnapToTargetIncludingScale;
		FirstWeapon->AttachToComponent(OwnerHMeshComponent,AttachmentTransformRules,FirstWeapon->HandSocketName);
	}
	if(SecondWeapon)
	{
		FDetachmentTransformRules DetachmentTransformRules = FDetachmentTransformRules::KeepWorldTransform;
		SecondWeapon->DetachAllSceneComponents(OwnerHMeshComponent,DetachmentTransformRules);
		FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules::SnapToTargetIncludingScale;
		SecondWeapon->AttachToComponent(OwnerHMeshComponent,AttachmentTransformRules,SecondWeapon->HandSocketName);
	}
}

void UCombatComponent::DisarmMontageCallNotify()
{
	if(OwnerHMeshComponent == nullptr) return;
	if(FirstWeapon)
	{
		FDetachmentTransformRules DetachmentTransformRules = FDetachmentTransformRules::KeepWorldTransform;
		FirstWeapon->DetachAllSceneComponents(OwnerHMeshComponent,DetachmentTransformRules);
		FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules::SnapToTargetIncludingScale;
		FirstWeapon->AttachToComponent(OwnerHMeshComponent,AttachmentTransformRules,FirstWeapon->BackSocketName);
	}
	if(SecondWeapon)
	{
		FDetachmentTransformRules DetachmentTransformRules = FDetachmentTransformRules::KeepWorldTransform;
		SecondWeapon->DetachAllSceneComponents(OwnerHMeshComponent,DetachmentTransformRules);
		FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules::SnapToTargetIncludingScale;
		SecondWeapon->AttachToComponent(OwnerHMeshComponent,AttachmentTransformRules,SecondWeapon->BackSocketName);
	}
}

void UCombatComponent::HandleBeginSpawnAndAttachToBackWeapon()
{
	if(GetOwner()->HasAuthority())
	{
		if(OwnerHMeshComponent == nullptr) return;
		if(FirstWeaponClass) 
		{
			FActorSpawnParameters SParams;
			SParams.Owner = GetOwner(); // actor serverowned actor oluyor dikkat et
			FirstWeapon = GetWorld()->SpawnActor<AWeapon>(FirstWeaponClass,SParams);
			FirstWeapon->SetOwner(GetOwner());
			FirstWeapon->OwnerHPlayerController = OwnerHPlayerController;
		}
		if(SecondWeaponClass)
		{
			FActorSpawnParameters SParams;
			SParams.Owner = GetOwner();
			SecondWeapon = GetWorld()->SpawnActor<AWeapon>(SecondWeaponClass,SParams);
			SecondWeapon->SetOwner(GetOwner());
			SecondWeapon->OwnerHPlayerController = OwnerHPlayerController;
		}
		Multicast_AttachWeaponToBack();
	}
}

void UCombatComponent::Multicast_AttachWeaponToBack_Implementation()
{
	if(OwnerHMeshComponent == nullptr) return;
	if(FirstWeapon)
	{
		FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules::SnapToTargetIncludingScale;
		FirstWeapon->AttachToComponent(OwnerHMeshComponent,AttachmentTransformRules,FirstWeapon->BackSocketName);
	}
	if(SecondWeapon)
	{
		FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules::SnapToTargetIncludingScale;
		SecondWeapon->AttachToComponent(OwnerHMeshComponent,AttachmentTransformRules,SecondWeapon->BackSocketName);
	}
}

void UCombatComponent::GetDamage(float Damage, const FVector& HitLocation, const EHitType HitType,
                                 const AActor* DamageCauserActor) // Serverde gerçekleştiğinden eminiz
{
	// Finisherlarda onca ihtimal ortaya çıkabilir eğer hasar veya etki alırsa projeyi o kadar uzatmak istemiyoruz
	if(OwnerHCharacter->CurrentCharacterActionStateStruct.bBeingExecuted || OwnerHCharacter->CurrentCharacterActionStateStruct.bOnExecution) return;

	if(!OwnerHCharacter->bVisible) OwnerHCharacter->Server_SetCharacterVisibility(true);  // karakter görünmezse görünmezli bozulur.
	
	if(OwnerHCharacter->ActorHasTag(FName("GreatSwordCharacter")))
	{
		float px = (Damage*GreatSwordCharacterPassiveDamageReductionPercent)/100;
		Damage = Damage - px;
	}
	UE_LOG(LogTemp,Error,TEXT("%f damage alındı"),Damage);
	if(CurrentBlockAmount > 0) // Block Active
	{
		if(CurrentBlockAmount > Damage || CurrentBlockAmount == Damage) // Block açık ve gelen damage bloktan küçük veya eşit
		{
			CurrentBlockAmount = FMath::Clamp(CurrentBlockAmount - Damage,0,MaxBlockAmount);
			TemporaryMaxBlockAmount = CurrentBlockAmount;

			if(CurrentBlockAmount > 0) // Damage Kırılmadı block hit anim oynatıyoruz
			{
				Multicast_PlayBlockMontageHit();

				DIndicatorForShieldDamage(Damage);
			}
			
			if(CurrentBlockAmount == 0) // block kırıldı
			{
				SetVisibilityOfShieldBar(false);
				
				Multicast_PlayBlockMontageBroken(); // broken anim giriyor

				StartTimerForRefreshTempBlockAmount();

				DIndicatorForShieldDamage(Damage);
			}
			
			UpdateCharacterOverlayShieldBar();
			if(OwnerHHUD) // ownerhhud localcontrollerda setlenecek  aynı zamanda serverdede olabilir host mantığı olduğu için
			{
				UpdateGenelWidgetShieldBar();
			}
		}
		else // Block açık ve gelen damage bloktan büyük
		{
			float DamageAmount = Damage-CurrentBlockAmount;
			
			CurrentBlockAmount =0;
			TemporaryMaxBlockAmount = CurrentBlockAmount;

			Multicast_PlayBlockMontageBroken(); // broken anim giriyor

			DIndicatorForNormalDamage(DamageAmount);

			StartTimerForRefreshTempBlockAmount();
			
			UpdateCharacterOverlayShieldBar();
			if(OwnerHHUD) // ownerhhud localcontrollerda setlenecek  aynı zamanda serverdede olabilir host mantığı olduğu için
			{
				UpdateGenelWidgetShieldBar();
			}
			
			CurrentHP = FMath::Clamp(CurrentHP-DamageAmount,0,MaxHP); // Hasar 
			
			UpdateCharacterOverlayHPBar();
			if(OwnerHHUD) // ownerhhud localcontrollerda setlenecek  aynı zamanda serverdede olabilir host mantığı olduğu için
			{
				UpdateGenelWidgetHPBar();
			}
			
			if(CurrentHP == 0)
			{
				OwnerHCharacter->Die();
			}
			else
			{
				StartHPRegenCooldownTimer();
			}
		}
	}
	else // Blok etkin değilse
	{
		CurrentHP = FMath::Clamp(CurrentHP-Damage,0,MaxHP); // Hasar

		DIndicatorForNormalDamage(Damage);
		
		UpdateCharacterOverlayHPBar();
		if(OwnerHHUD) // ownerhhud localcontrollerda setlenecek  aynı zamanda serverdede olabilir host mantığı olduğu için
		{
			UpdateGenelWidgetHPBar();
		}
		
		if(CurrentHP == 0)
		{
			OwnerHCharacter->Die();
		}
		else
		{
			StartHPRegenCooldownTimer();
		}
	}
	
}

void UCombatComponent::GetEffectDamage(float Damage, bool bFire)
{
	// Finisherlarda onca ihtimal ortaya çıkabilir eğer hasar veya etki alırsa, projeyi o kadar uzatmak istemiyoruz
	if(OwnerHCharacter->CurrentCharacterActionStateStruct.bBeingExecuted || OwnerHCharacter->CurrentCharacterActionStateStruct.bOnExecution) return;

	if(!OwnerHCharacter->bVisible) OwnerHCharacter->Server_SetCharacterVisibility(true); // karakter görünmezse görünmezli bozulur.
	
	if(bFire)
	{
		DIndicatorForFireDamage(Damage);
	}
	else
	{
		DIndicatorForFrostDamage(Damage);
	}
	if(CurrentBlockAmount > 0) // Block Active
	{
		if(CurrentBlockAmount > Damage || CurrentBlockAmount == Damage) // Block açık ve gelen damage bloktan küçük veya eşit
		{
			CurrentBlockAmount = CurrentBlockAmount - Damage;
			TemporaryMaxBlockAmount = CurrentBlockAmount;
			
			if(CurrentBlockAmount == 0)
			{
				SetVisibilityOfShieldBar(false);
				
				//Multicast_PlayBlockMontageBroken();
				OwnerHCharacter->CurrentCharacterState = ECharacterState::ECS_Unoccupied;

				StartTimerForRefreshTempBlockAmount();
			}
			
			UpdateCharacterOverlayShieldBar();
			if(OwnerHHUD) // ownerhhud localcontrollerda setlenecek  aynı zamanda serverdede olabilir host mantığı olduğu için
			{
				UpdateGenelWidgetShieldBar();
			}
		}
		else // Block açık ve gelen damage bloktan büyük
		{
			float DamageAmount = Damage-CurrentBlockAmount;
			
			CurrentBlockAmount =0;
			TemporaryMaxBlockAmount = CurrentBlockAmount;

			StartTimerForRefreshTempBlockAmount();
			
			UpdateCharacterOverlayShieldBar();
			if(OwnerHHUD) // ownerhhud localcontrollerda setlenecek  aynı zamanda serverdede olabilir host mantığı olduğu için
			{
				UpdateGenelWidgetShieldBar();
			}
			
			CurrentHP = FMath::Clamp(CurrentHP-DamageAmount,0,MaxHP); // Hasar 
			
			UpdateCharacterOverlayHPBar();
			if(OwnerHHUD) // ownerhhud localcontrollerda setlenecek  aynı zamanda serverdede olabilir host mantığı olduğu için
			{
				UpdateGenelWidgetHPBar();
			}
			
			if(CurrentHP == 0)
			{
				OwnerHCharacter->Die();
			}
			else
			{
				StartHPRegenCooldownTimer();
			}
		}
	}
	else // Blok etkin değilse
	{
		CurrentHP = FMath::Clamp(CurrentHP-Damage,0,MaxHP); // Hasar
		
		UpdateCharacterOverlayHPBar();
		if(OwnerHHUD) // ownerhhud localcontrollerda setlenecek  aynı zamanda serverdede olabilir host mantığı olduğu için
		{
			UpdateGenelWidgetHPBar();
		}
		
		if(CurrentHP == 0)
		{
			OwnerHCharacter->Die();
		}
		else
		{
			StartHPRegenCooldownTimer();
		}
	}
}

void UCombatComponent::SetCurrentSpeed(uint16 Speed)
{
	CurrentSpeed = Speed;
	
	if(OwnerHCharacter && OwnerHCharacter->GetCharacterMovement())
	{
		OwnerHCharacter->GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
	}
}

void UCombatComponent::OnRep_CurrentSpeed()
{
	if(OwnerHCharacter && OwnerHCharacter->GetCharacterMovement())
	{
		OwnerHCharacter->GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
	}
}

void UCombatComponent::OnRep_CurrentHP()
{
	UpdateCharacterOverlayHPBar();
	if(OwnerHHUD)
	{
		UpdateGenelWidgetHPBar();
	}
}

void UCombatComponent::OnRep_CurrentMP()
{
	UpdateCharacterOverlayMPBar();
	if(OwnerHHUD) 
	{
		UpdateGenelWidgetMPBar();
	}
}

void UCombatComponent::StartHPRegenCooldownTimer()
{
	if(GetWorld() == nullptr) return;
	if(!OwnerHCharacter->bIsAlive || OwnerHCharacter->bOnFinisherDying) return;
	GetWorld()->GetTimerManager().ClearTimer(HPRegenCooldownTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(HPRegenTimerHandle);
	
	GetWorld()->GetTimerManager().SetTimer(HPRegenCooldownTimerHandle,this,&ThisClass::StartHPRegen,HPRegenCooldownTime);
	
}

void UCombatComponent::StartHPRegen()
{
	GetWorld()->GetTimerManager().SetTimer(HPRegenTimerHandle,this,&ThisClass::HPRegen,HPRegenTimeRate,true);
}

void UCombatComponent::HPRegen()
{
	if(!OwnerHCharacter->bIsAlive || OwnerHCharacter->bOnFinisherDying)
	{
		GetWorld()->GetTimerManager().ClearTimer(HPRegenTimerHandle);
		return;
	}
	
	if(CurrentHP == MaxHP) GetWorld()->GetTimerManager().ClearTimer(HPRegenTimerHandle);
	CurrentHP = FMath::Clamp(CurrentHP + HPRegenAmount,0,MaxHP);
	
	UpdateCharacterOverlayHPBar();
	if(OwnerHHUD) // ownerhhud localcontrollerda setlenecek  aynı zamanda serverdede olabilir host mantığı olduğu için
	{
		UpdateGenelWidgetHPBar();
	}
	
}

void UCombatComponent::StartMPRegenCooldownTimer()
{
	if(GetWorld() == nullptr) return;
	if(!OwnerHCharacter->bIsAlive || OwnerHCharacter->bOnFinisherDying) return;
	GetWorld()->GetTimerManager().ClearTimer(MPRegenCooldownTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(MPRegenTimerHandle);
	
	GetWorld()->GetTimerManager().SetTimer(MPRegenCooldownTimerHandle,this,&ThisClass::StartMPRegen,MPRegenCooldownTime);
}

void UCombatComponent::StartMPRegen()
{
	GetWorld()->GetTimerManager().SetTimer(MPRegenTimerHandle,this,&ThisClass::MPRegen,MPRegenTimeRate,true);
}

void UCombatComponent::MPRegen()
{
	if(!OwnerHCharacter->bIsAlive || OwnerHCharacter->bOnFinisherDying)
	{
		GetWorld()->GetTimerManager().ClearTimer(MPRegenTimerHandle);
		return;
	}
	
	if(CurrentMP == MaxMP) GetWorld()->GetTimerManager().ClearTimer(MPRegenTimerHandle);
	CurrentMP = FMath::Clamp(CurrentMP + MPRegenAmount,0,MaxMP);
	
	UpdateCharacterOverlayMPBar();
	if(OwnerHHUD) // ownerhhud localcontrollerda setlenecek  aynı zamanda serverdede olabilir host mantığı olduğu için
	{
		UpdateGenelWidgetMPBar();
	}
}

bool UCombatComponent::CheckCurrentManaAndReduce(float ManaCost)
{
	if(CurrentMP < ManaCost) return false;
	CurrentMP = FMath::Clamp(CurrentMP - ManaCost,0,MaxMP);
	UpdateCharacterOverlayMPBar();
	if(OwnerHHUD) // ownerhhud localcontrollerda setlenecek  aynı zamanda serverdede olabilir host mantığı olduğu için
	{
		UpdateGenelWidgetMPBar();
	}
	StartMPRegenCooldownTimer();
	return true;
}

void UCombatComponent::DestroyAttachedAllSkillActors()
{
	if(ActiveAttachedSkillActors.Num()>0)
	{
		for (auto Element : ActiveAttachedSkillActors)
		{
			if(Element)
			{
				Element->Destroy();
			}
		}
		ActiveAttachedSkillActors.Empty();
	}
}

void UCombatComponent::AddSkillActorToActiveAttachedSkillActorArray(ASkillActor* LSkillActor)
{
	ActiveAttachedSkillActors.Add(LSkillActor);
}

void UCombatComponent::SetGenelWidgetSkill1Text(float TimeRemainSeconds)
{
	if(OwnerHHUD && OwnerHHUD->GenelWidget)
	{
		int32 ITimeRemainSeconds = TimeRemainSeconds; 
		FText TT = FText::AsNumber(ITimeRemainSeconds);
		OwnerHHUD->GenelWidget->Skill1Text->SetText(TT);
	}
}

void UCombatComponent::ActivateSkill1Cooldown()
{
	bSkill1OnCooldDown = true;
	Skill1TimeRemainSeconds = Skill1Cooldown - (LastRoundTripTime/2); // serverde LastRoundTripTime her zaman 0 dır.
	if(OwnerHHUD && OwnerHHUD->GenelWidget)
	{
		OwnerHHUD->GenelWidget->Skill1Image->SetRenderOpacity(0.3f);
	}
}

void UCombatComponent::DeactivateSkill1Cooldown()
{
	bSkill1OnCooldDown = false;
	Skill1TimeRemainSeconds = 0;
	if(OwnerHHUD && OwnerHHUD->GenelWidget)
	{
		OwnerHHUD->GenelWidget->Skill1Image->SetRenderOpacity(1.f);
		OwnerHHUD->GenelWidget->Skill1Text->SetText(FText::GetEmpty());
	}
}

void UCombatComponent::SetGenelWidgetSkill2Text(float TimeRemainSeconds)
{
	if(OwnerHHUD && OwnerHHUD->GenelWidget)
	{
		int32 ITimeRemainSeconds = TimeRemainSeconds; 
		FText TT = FText::AsNumber(ITimeRemainSeconds);
		OwnerHHUD->GenelWidget->Skill2Text->SetText(TT);
	}
}

void UCombatComponent::ActivateSkill2Cooldown()
{
	bSkill2OnCooldDown = true;
	Skill2TimeRemainSeconds = Skill2Cooldown - (LastRoundTripTime/2); // serverde LastRoundTripTime her zaman 0 dır.
	if(OwnerHHUD && OwnerHHUD->GenelWidget)
	{
		OwnerHHUD->GenelWidget->Skill2Image->SetRenderOpacity(0.3f);
	}
}

void UCombatComponent::DeactivateSkill2Cooldown()
{
	bSkill2OnCooldDown = false;
	Skill2TimeRemainSeconds = 0;
	if(OwnerHHUD && OwnerHHUD->GenelWidget)
	{
		OwnerHHUD->GenelWidget->Skill2Image->SetRenderOpacity(1.f);
		OwnerHHUD->GenelWidget->Skill2Text->SetText(FText::GetEmpty());
	}
}

void UCombatComponent::SetGenelWidgetSkill3Text(float TimeRemainSeconds)
{
	if(OwnerHHUD && OwnerHHUD->GenelWidget)
	{
		int32 ITimeRemainSeconds = TimeRemainSeconds; 
		FText TT = FText::AsNumber(ITimeRemainSeconds);
		OwnerHHUD->GenelWidget->Skill3Text->SetText(TT);
	}
}

void UCombatComponent::ActivateSkill3Cooldown()
{
	bSkill3OnCooldDown = true;
	Skill3TimeRemainSeconds = Skill3Cooldown - (LastRoundTripTime/2); // serverde LastRoundTripTime her zaman 0 dır.
	if(OwnerHHUD && OwnerHHUD->GenelWidget)
	{
		OwnerHHUD->GenelWidget->Skill3Image->SetRenderOpacity(0.3f);
	}
}

void UCombatComponent::DeactivateSkill3Cooldown()
{
	bSkill3OnCooldDown = false;
	Skill3TimeRemainSeconds = 0;
	if(OwnerHHUD && OwnerHHUD->GenelWidget)
	{
		OwnerHHUD->GenelWidget->Skill3Image->SetRenderOpacity(1.f);
		OwnerHHUD->GenelWidget->Skill3Text->SetText(FText::GetEmpty());
	}
}

void UCombatComponent::SetGenelWidgetSkill4Text(float TimeRemainSeconds)
{
	if(OwnerHHUD && OwnerHHUD->GenelWidget)
	{
		int32 ITimeRemainSeconds = TimeRemainSeconds; 
		FText TT = FText::AsNumber(ITimeRemainSeconds);
		OwnerHHUD->GenelWidget->Skill4Text->SetText(TT);
	}
}

void UCombatComponent::ActivateSkill4Cooldown()
{
	bSkill4OnCooldDown = true;
	Skill4TimeRemainSeconds = Skill4Cooldown - (LastRoundTripTime/2); // serverde LastRoundTripTime her zaman 0 dır.
	if(OwnerHHUD && OwnerHHUD->GenelWidget)
	{
		OwnerHHUD->GenelWidget->Skill4Image->SetRenderOpacity(0.3f);
	}
}

void UCombatComponent::DeactivateSkill4Cooldown()
{
	bSkill4OnCooldDown = false;
	Skill4TimeRemainSeconds = 0;
	if(OwnerHHUD && OwnerHHUD->GenelWidget)
	{
		OwnerHHUD->GenelWidget->Skill4Image->SetRenderOpacity(1.f);
		OwnerHHUD->GenelWidget->Skill4Text->SetText(FText::GetEmpty());
	}
}

void UCombatComponent::SetGenelWidgetSkillEText(float TimeRemainSeconds)
{
	int32 ITimeRemainSeconds = TimeRemainSeconds; 
	FText TT = FText::AsNumber(ITimeRemainSeconds);
	if(OwnerHHUD)
	{
		OwnerHHUD->GenelWidget->SkillEText->SetText(TT);
	}
}

void UCombatComponent::ActivateSkillECooldown()
{
	bSkillEOnCooldDown = true;
	SkillETimeRemainSeconds = SkillECooldown - (LastRoundTripTime/2); // serverde LastRoundTripTime her zaman 0 dır.
	if(OwnerHHUD && OwnerHHUD->GenelWidget)
	{
		OwnerHHUD->GenelWidget->SkillEImage->SetRenderOpacity(0.3f);
	}
}

void UCombatComponent::DeactivateSkillECooldown()
{
	bSkillEOnCooldDown = false;
	SkillETimeRemainSeconds = 0;
	if(OwnerHHUD && OwnerHHUD->GenelWidget)
	{
		OwnerHHUD->GenelWidget->SkillEImage->SetRenderOpacity(1.f);
		OwnerHHUD->GenelWidget->SkillEText->SetText(FText::GetEmpty());
	}
}

void UCombatComponent::Client_ActivateSkillECooldown_Implementation()
{
	ActivateSkillECooldown();
}

void UCombatComponent::Server_CastSkill_Implementation(uint8 SkillNumber)
{
	if(OwnerHCharacter && OwnerHCharacter->bCanCast())
	{
		if(!OwnerHCharacter->bVisible) // görünmezken skill atarsa görünmezlik bozulmalı, zaten sadece asasta olacağı için sadece buraya koymak yeterli
		{
			OwnerHCharacter->Multicast_BrokeInvisibility(); 
		}
		
		switch (SkillNumber)
		{
		default:
			break;
		case 1:
			if(bSkill1OnCooldDown) return;
			if(CheckCurrentManaAndReduce(Skill1ManaCost) == false) return;
			ActivateSkill1Cooldown(); // serverdeki local oyuncu ise işlemler yapılır, değilse checkler geçilemez
			// checkler geçilmezse aşağıdaki fonkun içine yerleştireceğiz if(HHUD) girerse clientteki local oyuncudur
			Multicast_PlaySkillMontage(0); // 1 eksiğini yolluyoruz çünkü array 0 dan başlıyor
			break;
		case 2:
			if(bSkill2OnCooldDown) return;
			if(CheckCurrentManaAndReduce(Skill2ManaCost) == false) return;
			ActivateSkill2Cooldown();
			Multicast_PlaySkillMontage(1);
			break;
		case 3:
			if(bSkill3OnCooldDown) return;
			if(CheckCurrentManaAndReduce(Skill3ManaCost) == false) return;
			ActivateSkill3Cooldown();
			Multicast_PlaySkillMontage(2);
			break;
		case 4:
			if(bSkill4OnCooldDown) return;
			if(CheckCurrentManaAndReduce(Skill4ManaCost) == false) return;
			ActivateSkill4Cooldown();
			Multicast_PlaySkillMontage(3);
			break;
		}
	}
}

void UCombatComponent::Multicast_PlaySkillMontage_Implementation(uint8 SkillNumber)
{
	if(SkillMontages.Num()>0)
	{
		if(!SkillMontages[SkillNumber]) return;
		if(SkillNumber == 0 &&!OwnerHCharacter->HasAuthority() && OwnerHHUD) // clientteki local oyuncuyu bulduk, serverdekini zaten öncesinde başlatmıştık
		{
			ActivateSkill1Cooldown();
		}
		else if(SkillNumber == 1 &&!OwnerHCharacter->HasAuthority() && OwnerHHUD) // clientteki local oyuncuyu bulduk, serverdekini zaten öncesinde başlatmıştık
		{
			ActivateSkill2Cooldown();
		}
		else if(SkillNumber == 2 &&!OwnerHCharacter->HasAuthority() && OwnerHHUD) // clientteki local oyuncuyu bulduk, serverdekini zaten öncesinde başlatmıştık
		{
			ActivateSkill3Cooldown();
		}
		else if(SkillNumber == 3 &&!OwnerHCharacter->HasAuthority() && OwnerHHUD) // clientteki local oyuncuyu bulduk, serverdekini zaten öncesinde başlatmıştık
		{
			ActivateSkill4Cooldown();
		}
		OwnerHAnimInstance->Montage_Play(SkillMontages[SkillNumber]);
	}
}

void UCombatComponent::OnRep_CurrentBlockAmount()
{
	
	if(CurrentBlockAmount > 0 && !OwnerCharacterOverlayWidget->ShieldBar->IsVisible())
	{
		SetVisibilityOfShieldBar(true);
	}

	if(CurrentBlockAmount == 0)
	{
		SetVisibilityOfShieldBar(false);
	}
	
	UpdateCharacterOverlayShieldBar();
	if(OwnerHHUD) 
	{
		UpdateGenelWidgetShieldBar();
	}
}

void UCombatComponent::Server_PerformBlock_Implementation()
{
	if(OwnerHCharacter && OwnerHCharacter->bCanBlock())
	{
		if(TemporaryMaxBlockAmount > 0)
		{
			CurrentBlockAmount = TemporaryMaxBlockAmount;
			Multicast_PlayBlockMontage();

			UpdateCharacterOverlayShieldBar();
			
			if(OwnerHHUD) // ownerhhud localcontrollerda setlenecek  aynı zamanda serverdede olabilir host mantığı olduğu için
			{
				UpdateGenelWidgetShieldBar();
			}
			
			SetVisibilityOfShieldBar(true);
		}
	}
}

void UCombatComponent::Server_CloseBlock_Implementation()
{
	if(OwnerHCharacter->CurrentCharacterState == ECharacterState::ECS_Blocking && OwnerHCharacter->bCanDoMoveActionsCharacterActionStateStruct() && bWeaponOnHand
		&& CurrentBlockAmount > 0)
	{
		CurrentBlockAmount = 0;
		
		if(OwnerHCharacter->ActorHasTag(FName("MageCharacter")))
		{
			OwnerHCharacter->CurrentCharacterState = ECharacterState::ECS_Unoccupied;
		}
		
		Multicast_PlayCloseBlockMontage();

		if(OwnerHHUD) // ownerhhud localcontrollerda setlenecek  aynı zamanda serverdede olabilir host mantığı olduğu için
		{
			UpdateGenelWidgetShieldBar();
		}
		
		SetVisibilityOfShieldBar(false);
	}
}

void UCombatComponent::Multicast_PlayBlockMontage_Implementation()
{
	if(OwnerHAnimInstance && BlockMontage)
	{
		OwnerHAnimInstance->Montage_Play(BlockMontage);
	}
}

void UCombatComponent::PlayBlockMontageHit()
{
	if(OwnerHAnimInstance && HitMontage)
	{
		OwnerHAnimInstance->Montage_Play(HitMontage);
	}
}

void UCombatComponent::Multicast_PlayBlockMontageHit_Implementation()
{

	if(OwnerHAnimInstance && HitMontage)
    	{
    		OwnerHAnimInstance->Montage_Play(HitMontage);
    	}
}

void UCombatComponent::Multicast_PlayCloseBlockMontage_Implementation()
{
	if(OwnerHAnimInstance && BlockMontage)
	{
		OwnerHAnimInstance->Montage_Play(BlockMontage);
		OwnerHAnimInstance->Montage_JumpToSection(FName("Close"),BlockMontage);
	}
	if(OwnerHCharacter->ActorHasTag(FName("MageCharacter"))) // magede eksik montaj asseti old. için farklı bir yola gidildi.
	{
		if(OwnerHAnimInstance)
		{
			OwnerHAnimInstance->StopAllMontages(1);
		}
	}
}

void UCombatComponent::Multicast_PlayBlockMontageBroken_Implementation()
{
	if(OwnerHAnimInstance && BlockMontage)
	{
		OwnerHAnimInstance->Montage_Play(BlockMontage);
		OwnerHAnimInstance->Montage_JumpToSection(FName("Broken"),BlockMontage);
	}
}

void UCombatComponent::ContinueBlock()
{
	if(OwnerHAnimInstance && BlockMontage)
	{
		OwnerHAnimInstance->Montage_Play(BlockMontage);
		OwnerHAnimInstance->Montage_JumpToSection(FName("Loop"),BlockMontage);
		OwnerHCharacter->CurrentCharacterState = ECharacterState::ECS_Blocking; // bazen an de sorun olabiliyor tam çözemedim daha o yüzden koyduk
	}
}

void UCombatComponent::StartTimerForRefreshTempBlockAmount()
{
	if(bRefreshTimerForTempBlockAmountStarted)
	{
		GetWorld()->GetTimerManager().ClearTimer(RefreshTempBlockAmountTimerHandle);
	}
	GetWorld()->GetTimerManager().SetTimer(RefreshTempBlockAmountTimerHandle,this,&UCombatComponent::RefreshTempBlockAmount,RefreshTempBlockAmountTime);
	bRefreshTimerForTempBlockAmountStarted = true;
}

void UCombatComponent::RefreshTempBlockAmount()
{
	TemporaryMaxBlockAmount = MaxBlockAmount;
	bRefreshTimerForTempBlockAmountStarted = false;
}

bool UCombatComponent::bcanKnockdown()
{
	bool b1 = (
		OwnerHCharacter->CurrentCharacterState != ECharacterState::ECS_Dashing &&
		OwnerHCharacter->CombatComponent->CurrentBlockAmount == 0);
	bool b2= (!OwnerHCharacter->CurrentCharacterActionStateStruct.bFrozed &&
		!OwnerHCharacter->CurrentCharacterActionStateStruct.bBeingExecuted &&
		!OwnerHCharacter->CurrentCharacterActionStateStruct.bOnFall &&
		!OwnerHCharacter->CurrentCharacterActionStateStruct.bOnExecution &&
		!OwnerHCharacter->CurrentCharacterActionStateStruct.bOnSuperArmor) ;

	return (b1 && b2);
}

void UCombatComponent::PerformKnockdown(bool bForward)
{
	if(OwnerHCharacter->HasAuthority() && bcanKnockdown() && OwnerHAnimInstance)
	{
		if(KnockDownForwardMontage && KnockDownBackwardMontage)
		{
			Multicast_PlayKnockDownMontage(bForward);
		}
	}
}

void UCombatComponent::Server_PerformKnockdown_Implementation(bool bForward)
{
	if(OwnerHCharacter->HasAuthority() && bcanKnockdown() && OwnerHAnimInstance)
	{
		if(KnockDownForwardMontage && KnockDownBackwardMontage)
		{
			Multicast_PlayKnockDownMontage(bForward);
		}
	}
}

void UCombatComponent::Multicast_PlayKnockDownMontage_Implementation(bool bForward)
{
	if(OwnerHAnimInstance == nullptr) return;
	if(bForward)
	{
		OwnerHAnimInstance->Montage_Play(KnockDownForwardMontage);
	}
	else
	{
		OwnerHAnimInstance->Montage_Play(KnockDownBackwardMontage);
	}
	SetVisibilityOfShieldBar(false);
}

void UCombatComponent::UpdateCharacterOverlayHPBar()
{
	if(OwnerCharacterOverlayWidget)
	{
		OwnerCharacterOverlayWidget->HPBar->SetPercent(CurrentHP/MaxHP);
	}
}

void UCombatComponent::UpdateCharacterOverlayMPBar()
{
	if(OwnerCharacterOverlayWidget)
	{
		OwnerCharacterOverlayWidget->MPBar->SetPercent(CurrentMP/MaxMP);
	}
}

void UCombatComponent::UpdateCharacterOverlayShieldBar()
{
	if(OwnerCharacterOverlayWidget)
	{
		OwnerCharacterOverlayWidget->ShieldBar->SetPercent(CurrentBlockAmount/MaxBlockAmount);
	}
}

void UCombatComponent::SetVisibilityOfShieldBar(bool bVisible)
{
	if(OwnerCharacterOverlayWidget)
	{
		if(bVisible)
		{
			OwnerCharacterOverlayWidget->ShieldBar->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			OwnerCharacterOverlayWidget->ShieldBar->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UCombatComponent::UpdateGenelWidgetHPBar()
{
	if(OwnerHHUD)
	{
		OwnerHHUD->SetGenelWidgetHPBar(CurrentHP/MaxHP);
	}
}

void UCombatComponent::UpdateGenelWidgetMPBar()
{
	if(OwnerHHUD)
	{
		OwnerHHUD->SetGenelWidgetMPBar(CurrentMP/MaxMP);
	}
}

void UCombatComponent::UpdateGenelWidgetShieldBar()
{
	if(OwnerHHUD)
	{
		OwnerHHUD->SetGenelWidgetShieldBar(CurrentBlockAmount/MaxBlockAmount);
	}
}

void UCombatComponent::DIndicatorForNormalDamage(float Damage)
{
	UDamageIndicatorWidgetComponent* DITComp = NewObject<UDamageIndicatorWidgetComponent>(OwnerHCharacter,DamageIndicatorWidgetComponentClass);
	DITComp->RegisterComponent();
	DITComp->AttachToComponent(OwnerHCharacter->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
	DITComp->PlayNormalDamageWidgetAnim(Damage);
	DITComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	Multicast_DIndicatorForNormalDamage(Damage);
}

void UCombatComponent::Multicast_DIndicatorForNormalDamage_Implementation(uint16 Damage)
{
	if(OwnerHCharacter->HasAuthority()) return;
	
	UDamageIndicatorWidgetComponent* DITComp = NewObject<UDamageIndicatorWidgetComponent>(OwnerHCharacter,DamageIndicatorWidgetComponentClass);
	DITComp->RegisterComponent();
	DITComp->AttachToComponent(OwnerHCharacter->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
	DITComp->PlayNormalDamageWidgetAnim(Damage);
	DITComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}

void UCombatComponent::DIndicatorForShieldDamage(float Damage)
{
	UDamageIndicatorWidgetComponent* DITComp = NewObject<UDamageIndicatorWidgetComponent>(OwnerHCharacter,DamageIndicatorWidgetComponentClass);
	DITComp->RegisterComponent();
	DITComp->AttachToComponent(OwnerHCharacter->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
	DITComp->PlayShieldDamageWidgetAnim(Damage);
	DITComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	Multicast_DIndicatorForShieldDamage(Damage);
}

void UCombatComponent::Multicast_DIndicatorForShieldDamage_Implementation(uint16 Damage)
{
	if(OwnerHCharacter->HasAuthority()) return;

	UDamageIndicatorWidgetComponent* DITComp = NewObject<UDamageIndicatorWidgetComponent>(OwnerHCharacter,DamageIndicatorWidgetComponentClass);
	DITComp->RegisterComponent();
	DITComp->AttachToComponent(OwnerHCharacter->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
	DITComp->PlayShieldDamageWidgetAnim(Damage);
	DITComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}

void UCombatComponent::DIndicatorForFireDamage(float Damage)
{
	UDamageIndicatorWidgetComponent* DITComp = NewObject<UDamageIndicatorWidgetComponent>(OwnerHCharacter,DamageIndicatorWidgetComponentClass);
	DITComp->RegisterComponent();
	DITComp->AttachToComponent(OwnerHCharacter->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
	DITComp->PlayBurnDamageWidgetAnim(Damage);
	DITComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	Multicast_DIndicatorForFireDamage(Damage);
}

void UCombatComponent::Multicast_DIndicatorForFireDamage_Implementation(uint16 Damage)
{
	if(OwnerHCharacter->HasAuthority()) return;

	UDamageIndicatorWidgetComponent* DITComp = NewObject<UDamageIndicatorWidgetComponent>(OwnerHCharacter,DamageIndicatorWidgetComponentClass);
	DITComp->RegisterComponent();
	DITComp->AttachToComponent(OwnerHCharacter->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
	DITComp->PlayBurnDamageWidgetAnim(Damage);
	DITComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}

void UCombatComponent::DIndicatorForFrostDamage(float Damage)
{
	UDamageIndicatorWidgetComponent* DITComp = NewObject<UDamageIndicatorWidgetComponent>(OwnerHCharacter,DamageIndicatorWidgetComponentClass);
	DITComp->RegisterComponent();
	DITComp->AttachToComponent(OwnerHCharacter->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
	DITComp->PlayFrozeDamageWidgetAnim(Damage);
	DITComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	Multicast_DIndicatorForFrostDamage(Damage);
}

bool UCombatComponent::bCanBeExecuted()
{
	bool b1 = (OwnerHCharacter->CurrentCharacterState != ECharacterState::ECS_Dashing &&
		OwnerHCharacter->CurrentCharacterState != ECharacterState::ECS_Blocking);
	
	bool b2 =(
		!OwnerHCharacter->CurrentCharacterActionStateStruct.bBeingExecuted &&
		!OwnerHCharacter->CurrentCharacterActionStateStruct.bOnExecution &&
		!OwnerHCharacter->CurrentCharacterActionStateStruct.bOnSuperArmor );

	bool b3 = CurrentHP/MaxHP <= 0.25;

	return b1 && b2 && b3;
}

void UCombatComponent::Multicast_DIndicatorForFrostDamage_Implementation(uint16 Damage)
{
	if(OwnerHCharacter->HasAuthority()) return;

	UDamageIndicatorWidgetComponent* DITComp = NewObject<UDamageIndicatorWidgetComponent>(OwnerHCharacter,DamageIndicatorWidgetComponentClass);
	DITComp->RegisterComponent();
	DITComp->AttachToComponent(OwnerHCharacter->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
	DITComp->PlayFrozeDamageWidgetAnim(Damage);
	DITComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}

void UCombatComponent::PerformGreatSwordExecutioner()
{
	if(OwnerHCharacter->bCanCast())
	{
		if(OwnerHAnimInstance && GreatSwordExecutionerMontage)
		{
			Multicast_PlayGreatSwordExecutionerMontage();
		}
	}
}

void UCombatComponent::Multicast_PlayGreatSwordExecutionerMontage_Implementation()
{
	if(OwnerHAnimInstance && GreatSwordExecutionerMontage)
	{
		OwnerHAnimInstance->Montage_Play(GreatSwordExecutionerMontage);
	}
}

void UCombatComponent::PerformGreatSwordExecuted()
{
	if(bCanBeExecuted())
	{
		if(OwnerHAnimInstance && GreatSwordExecutedMontage)
		{
			Multicast_PlayGreatSwordExecutedMontage();
		}
	}
}

void UCombatComponent::Multicast_PlayGreatSwordExecutedMontage_Implementation()
{
	if(OwnerHAnimInstance && GreatSwordExecutedMontage)
	{
		OwnerHAnimInstance->Montage_Play(GreatSwordExecutedMontage);
	}
}

bool UCombatComponent::bCanBeAmbushed()
{
	bool b1 = (OwnerHCharacter->CurrentCharacterState != ECharacterState::ECS_Dashing &&
		OwnerHCharacter->CurrentCharacterState != ECharacterState::ECS_Blocking);
	
	bool b2 =(
		!OwnerHCharacter->CurrentCharacterActionStateStruct.bBeingExecuted &&
		!OwnerHCharacter->CurrentCharacterActionStateStruct.bOnExecution &&
		!OwnerHCharacter->CurrentCharacterActionStateStruct.bOnSuperArmor );

	return b1 && b2 ;
}

void UCombatComponent::PerformDualSwordAmbush()
{
	if(OwnerHCharacter->bCanCast())
	{
		if(OwnerHAnimInstance && DualSwordAmbushMontage)
		{
			Multicast_PlayDualSwordAmbushMontage();
		}
	}
}

void UCombatComponent::Multicast_PlayDualSwordAmbushMontage_Implementation()
{
	if(OwnerHAnimInstance && DualSwordAmbushMontage)
	{
		OwnerHAnimInstance->Montage_Play(DualSwordAmbushMontage);
	}
}

void UCombatComponent::PerformDualSwordAmbushed()
{
	if(bCanBeAmbushed())
	{
		if(OwnerHAnimInstance && DualSwordAmbushedMontage)
		{
			Multicast_PlayDualSwordAmbushedMontage();
		}
	}
}

void UCombatComponent::Server_PerformMageESkill_Implementation()
{
	if(OwnerHCharacter->bCanCast() && !bSkillEOnCooldDown)
	{
		if(CheckCurrentManaAndReduce(SkillEManaCost) == false) return;
		ActivateSkillECooldown();
		if(OwnerHAnimInstance && MageESkillMontage)
		{
			Multicast_PlayMageESkillMontage();
		}
	}
}

void UCombatComponent::Multicast_PlayMageESkillMontage_Implementation()
{
	if(!OwnerHCharacter->HasAuthority() && OwnerHHUD) // clientteki local oyuncuyu bulduk, serverdekini zaten öncesinde başlatmıştık
	{
		ActivateSkillECooldown();
	}
	if(OwnerHAnimInstance && MageESkillMontage)
	{
		OwnerHAnimInstance->Montage_Play(MageESkillMontage);
	}
}

void UCombatComponent::Multicast_PlayDualSwordAmbushedMontage_Implementation()
{
	if(OwnerHAnimInstance && DualSwordAmbushedMontage)
	{
		OwnerHAnimInstance->Montage_Play(DualSwordAmbushedMontage);
	}
}

void UCombatComponent::Server_CastSkillEMage_Implementation()
{
	
}

void UCombatComponent::Server_CastSkillEDualSword_Implementation()
{
	if(bSkillEOnCooldDown) return;
	if(OwnerHCharacter->bVisible) return; // bu skilin atılması için invisible olması lazım
	//if(CheckCurrentManaAndReduce(SkillEManaCost) == false) return;
	OwnerHCharacter->Multicast_BrokeInvisibility();
	if(DualSwordSkillEActorClass)
	{
		FActorSpawnParameters SParams;
		SParams.Owner = OwnerHCharacter;
		SParams.Instigator = OwnerHCharacter;
		GetWorld()->SpawnActor<ASkillActor>(DualSwordSkillEActorClass,OwnerHCharacter->GetActorLocation(),FRotator::ZeroRotator,SParams);
	}
}

void UCombatComponent::Server_CastSkillEGreatSword_Implementation()
{
	if(bSkillEOnCooldDown) return;
	//if(CheckCurrentManaAndReduce(SkillEManaCost) == false) return;
	if(GreatSwordSkillEActorClass)
	{
		FActorSpawnParameters SParams;
		SParams.Owner = OwnerHCharacter;
		SParams.Instigator = OwnerHCharacter;
		GetWorld()->SpawnActor<ASkillActor>(GreatSwordSkillEActorClass,OwnerHCharacter->GetActorLocation(),FRotator::ZeroRotator,SParams);
	}
}



