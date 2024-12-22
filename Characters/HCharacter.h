// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "HANEDANRPG3/Enums/EnumsType.h"
#include "HANEDANRPG3/Interfaces/HitInterface.h"
#include "HCharacter.generated.h"

#define NAME_SkeletalMesh ECollisionChannel::ECC_GameTraceChannel1
#define NAME_WeaponTraceChannel ECollisionChannel::ECC_GameTraceChannel2

class AHPlayerController;
class AHHUD;
class UCharacterOverlayWidget;
class UWidgetComponent;
class UTargetLockComponent;

/**
 * değişiklik yaparsan enumstype daki enumuda değiştir artı state checkleri kontrol et
 * değişiklik yaparsan void Server_SetCurrentCharacterActionStateStruct fonk bak
 */
USTRUCT(BlueprintType)
struct FCharacterActionStateStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bStunned = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bSilenced = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bSlowed = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bFrozed = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bBurning= false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bOnFall = false; // Karakter big hit alıp yere düşmüşse.

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bBeingExecuted = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bOnExecution = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bOnSuperArmor = false;
	
};

class UGamePlayEffectsComponent;
class UCombatComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class HANEDANRPG3_API AHCharacter : public ACharacter , public IHitInterface
{
	GENERATED_BODY()

public:
	
	AHCharacter();

	virtual void Tick(float DeltaTime) override;

protected:
	
	virtual void BeginPlay() override;

public:

	virtual void PostInitializeComponents() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit(const float Damage, const FVector& HitLocation, const EHitType HitType, const AActor* DamageCauserActor) override;

	/**
	 * COMPONENTS
	 */

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere,Replicated)
	UCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere)
	UGamePlayEffectsComponent* GamePlayEffectsComponent;

	UPROPERTY(VisibleAnywhere)
	UTargetLockComponent* TargetLockComponent;

	UPROPERTY(VisibleAnywhere,Replicated)
	UWidgetComponent* CharacterOverlayWidgetComponent;
	UPROPERTY()
	UCharacterOverlayWidget* CharacterOverlayWidget; // CharacterOverlayWidget Referansını alıyoruz.
	void InitializeCharacterOverlayWidgetReference();

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* LockOnIconWidgetComponent;
	virtual void ActivateLockOnIcon() override;
	virtual void DeactivateLockOnIcon() override;
	

	/**
	 * REFERENCES
	 */

	/** Hudı çekmeye çalışıp combat componentteki referansı dolduruyor*/
	void HudReferenceWorks();

	UPROPERTY(VisibleAnywhere)
	AHHUD* HHudReference;

	UPROPERTY()
	AHPlayerController* HPlayerController;

	/**
	 * DESTROY CHARACTER WORKS
	 */

	UPROPERTY(ReplicatedUsing=OnRep_bIsAlive,BlueprintReadOnly)
	bool bIsAlive = true;
	UPROPERTY(Replicated)
	bool bOnFinisherDying = false;
	UFUNCTION()
	void OnRep_bIsAlive();
	
	void Die(); // Serverde Çağırdığına emin ol
	
	void DisableCapsuleCollision();

	void EnablePhysicWhenDying();
	
	void EnableDieAndPhysicImpulseWhenOnFinisher(float ImpulseScale, bool bImpulseFromForward); // Serverde Çağır
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_EnableDieAndPhysicImpulseWhenOnFinisher(float ImpulseScale, bool bImpulseFromForward);
	
	void DestroyHCharacter(); // Serverde çağırılmasına özen göster
	void UnpossesHPlayerController();
	virtual void Destroyed() override; // Replicated bir fonk oldugu için kullanabiliriz
	

	

	/**
	 * HUD WORKS
	 */

	/** Karakter destroy edilirken hud da destroy ediilmesi gerekn widgetleri destroy etmek için */
	void DestroyHUDworksForDestroyHCHaracter();
	
	/**
	 * INPUT
	 */
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
	
	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Jumping Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	virtual void Jump() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* EquipAction;

	void I_Equip();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* EnableTargetingAction;

	void I_EnableTargeting();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DisableTargetingAction;

	void I_DisableTargeting();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* NormalAttackAction;

	void I_NormalAttack();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RightClickAction;

	void I_RightClickOn();
	void I_RightClickOff();

	float OldTargetArmLength = 600.f;
	void SetCameraToOldMode();
	void SetCameraToAimingMode();

	bool bCanSwitchCameraMode();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DashAction;
	void I_Dash();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* BlockAction;
	void I_Block();
	bool bCanBlock();
	
	bool bCanCast();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Skill1Action;
	void I_Skill1();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Skill2Action;
	void I_Skill2();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Skill3Action;
	void I_Skill3();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Skill4Action;
	void I_Skill4();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SkillEAction;
	void I_SkillE();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ChangeCameraOffsetAction;
	void I_ChangeCameraOffset(const FInputActionValue& Value);
	
	/**
	 * FREE SPACE
	 */

	void InitializePlayerControllerAndEnhancedInput();

	UFUNCTION(NetMulticast,Reliable,BlueprintCallable)
	void Multicast_SetUseControllerYaw(bool bUseYaw);

	UFUNCTION(NetMulticast,Reliable,BlueprintCallable)
	void Multicast_SetActorRotation(const FRotator& NewRotation);
	
	UPROPERTY(ReplicatedUsing=OnRep_bTargeting)
	bool bTargeting = false;

	UPROPERTY(Replicated)
	bool bTargeted = false;
	UPROPERTY()
	AHCharacter* TargetedAhCharacter;
	UFUNCTION()
	void OnRep_bTargeting();

	/**
	 * Visibility Works
	 */
	UPROPERTY(ReplicatedUsing=OnRep_bVisible)
	bool bVisible = true;
	
	UFUNCTION(Server,Reliable,BlueprintCallable)
	void Server_SetCharacterVisibility(bool visible);
	void SetCharacterVisibility(bool visible);
	UFUNCTION()
	void OnRep_bVisible();

	virtual void Interface_BrokeInvisibility() override;

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void SetScreenInvisibilityEffectWidget(bool visible);

	/**
	 * Normal setvisibilty fonkun kullanılamayacağı yerlerde karkateri tüm makinelerde görünür yapmak için kullanulır
	 */
	UFUNCTION(NetMulticast,Reliable,BlueprintCallable)
	void Multicast_BrokeInvisibility(); 
	
	/**
	 * STATES
	 */

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Properties")
	EWeaponType CurrentWeaponType = EWeaponType::EWT_GreatSword;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Replicated)
	ECharacterState CurrentCharacterState = ECharacterState::ECS_Unoccupied;
	bool bCanCastCurrentCharacterState();
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,ReplicatedUsing=OnRep_CurrentCharacterActionStateStruct)
	FCharacterActionStateStruct CurrentCharacterActionStateStruct;
	/** Character Action State Struct ta bulunan durumlara göre karakterin her hangi bir movement actionu gerçekleştirebileceğini check etmeye yarar*/
	bool bCanDoMoveActionsCharacterActionStateStruct();
	bool bCanCastActionStateStruct();
	UFUNCTION(BlueprintCallable)
	void RefreshCurrentCharacterActionStateStruct();
	UFUNCTION()
	void OnRep_CurrentCharacterActionStateStruct();

	UFUNCTION(Server,Reliable)
	void Server_SetCurrentCharacterState(ECharacterState NewCharacterState);

	UFUNCTION(Server,Reliable)
	void Server_SetCurrentCharacterActionStateStruct(const ECAS& ChangeActionStructValue, bool bNewValue);

	bool bCanEquipWeapon(); // todo
	bool bCanNormalAttack(); // todo
	bool bCanTargeting(); // todo


	/**
	 * DASH-DODGE MECHANIC
	 */
	
	float ForwardOrBackward;
	float RightOrLeft;

	FName GetDashDirectionName();

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	UAnimMontage* DashOrRollMontage;

	UFUNCTION(BlueprintCallable)
	bool CanDash(); // önce clientte sonra serverdede check et
	bool bCanDash(); // aynısı garip bir bugı test etmek için kullanıyoruz

	UFUNCTION(Server,Reliable)
	void Server_PerformDash(const FName DirectionName);
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_PlayDashMontage(const FName DirectionName);

	/**
	 * DEBUG
	 */

	bool bDebugOnFall = false;

	

	
	

	

	

	
	
	








	
	

};
