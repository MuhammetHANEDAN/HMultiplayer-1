// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetLockComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HANEDANRPG3/Characters/HCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

UTargetLockComponent::UTargetLockComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UTargetLockComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if(OwnerHCharacter && OwnerHCharacter->HasAuthority())
	{
		ActorsToIgnore.Add(OwnerHCharacter);
		TargetObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));
	}
}

void UTargetLockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(bTargeting && TargetingActor && OwnerHCharacter)
	{
		FVector TargetLocation = TargetingActor->GetActorLocation();
		FVector TargeterLocation = OwnerHCharacter->GetActorLocation();

		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(TargeterLocation,TargetLocation);
		FRotator I_TargetRotation = UKismetMathLibrary::RInterpTo(OwnerHCharacter->GetActorRotation(),TargetRotation,DeltaTime,InterpSpeed);

		if(OwnerHCharacter->Controller && OwnerHCharacter->Controller->IsLocalController())
		{
			OwnerHCharacter->Controller->SetControlRotation(TargetRotation);
		}
		else
		{
			OwnerHCharacter->SetActorRotation(TargetRotation);
		}
		
	}
	
}

void UTargetLockComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UTargetLockComponent,bTargeting);
	DOREPLIFETIME(UTargetLockComponent,TargetingActor);
}

void UTargetLockComponent::Server_EnableTargeting_Implementation()
{
	if(OwnerHCharacter == nullptr) return;
	
	if(OwnerHCharacter->HasAuthority())
	{
		
		if(bTargeting)
		{
			Server_DisableTargeting();
			return;
		}

		FVector TraceStart = OwnerHCharacter->GetActorLocation();
		FVector TraceEnd = TraceStart + (OwnerHCharacter->GetActorForwardVector()*TraceLength);
		FHitResult OutHitResult;

		bool bHitted = UKismetSystemLibrary::SphereTraceSingleForObjects(this,TraceStart,TraceEnd,TraceRadius,TargetObjectTypes,
			false,ActorsToIgnore,EDrawDebugTrace::Type::ForDuration,OutHitResult,true);

		if(bHitted)
		{
			if(IHitInterface* HittedHActor = Cast<IHitInterface>(OutHitResult.GetActor()))
			{
				UE_LOG(LogTemp,Error,TEXT("serverde targeting enable edildi"));
				
				TargetingActor = OutHitResult.GetActor();
				if(AHCharacter* hactor = Cast<AHCharacter>(TargetingActor)) // sonradan ekledik hızlıca
				{
					if(hactor->bVisible == false)
					{
						return; // eğer aktör invisible ise returnlemeliyiz. 
					}
					hactor->bTargeted = true;
					hactor->TargetedAhCharacter = OwnerHCharacter;
				}
				OwnerHCharacter->bTargeting = true;
				bTargeting = true;

				Client_SetCameraHeightToTargeting();
				
			}
		}
		
	}
}

void UTargetLockComponent::Server_DisableTargeting_Implementation()
{
	if(OwnerHCharacter == nullptr) return;
	
	if(OwnerHCharacter->HasAuthority())
	{
		UE_LOG(LogTemp,Error,TEXT("serverde targeting disable edildi"));
		
		Client_SetCameraHeightToDefault();
		
		OwnerHCharacter->bTargeting = false;
		if(AHCharacter* hactor = Cast<AHCharacter>(TargetingActor)) // sonradan ekledik hızlıca
		{
			hactor->bTargeted = false;
			hactor->TargetedAhCharacter = nullptr;
		}
		TargetingActor = nullptr;
		bTargeting = false;
	}
	
}

void UTargetLockComponent::Client_SetCameraHeightToTargeting_Implementation()
{
	if(OwnerHCharacter && OwnerHCharacter->CameraBoom)
	{
		OwnerHCharacter->CameraBoom->SocketOffset.Z=150;
	}
	if(OwnerHCharacter->HasAuthority())
	{
		if(TargetingActor)
		{
			if(IHitInterface* Targeting = Cast<IHitInterface>(TargetingActor))
			{
				Targeting->ActivateLockOnIcon();
			}
		}
	}
	else
	{
		FTimerHandle TimerHandle1;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle1,this,&UTargetLockComponent::Delayed1,0.2f);
	}
	
}

void UTargetLockComponent::Client_SetCameraHeightToDefault_Implementation()
{
	if(OwnerHCharacter && OwnerHCharacter->CameraBoom)
	{
		OwnerHCharacter->CameraBoom->SocketOffset.Z=0;
	}

	if(TargetingActor)
	{
		if(IHitInterface* Targeting = Cast<IHitInterface>(TargetingActor))
		{
			Targeting->DeactivateLockOnIcon();
		}
	}
	
}

void UTargetLockComponent::Delayed1()
{
	if(TargetingActor)
	{
		if(IHitInterface* Targeting = Cast<IHitInterface>(TargetingActor))
		{
			Targeting->ActivateLockOnIcon();
		}
	}
}

void UTargetLockComponent::Delayed2()
{
	if(TargetingActor)
	{
		if(IHitInterface* Targeting = Cast<IHitInterface>(TargetingActor))
		{
			Targeting->DeactivateLockOnIcon();
		}
	}
}






