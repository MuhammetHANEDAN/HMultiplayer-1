// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "HANEDANRPG3/Interfaces/HitInterface.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	WeaponStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Static Mesh Component"));
	SetRootComponent(WeaponStaticMeshComponent);
	WeaponStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		TargetObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));
		ActorsToIgnore.Add(Owner);

		I_OwnerHCharacter = Cast<IHitInterface>(Owner);
	}
}

void AWeapon::StartHitTrace()
{
	bTracing = true;
}

void AWeapon::EndHitTrace()
{
	bTracing = false;
	HittedActors.Empty();
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(HasAuthority())
	{
		if(bTracing)
		{
			TraceStart = WeaponStaticMeshComponent->GetSocketLocation(FName("TraceStart"));
			TraceEnd = WeaponStaticMeshComponent->GetSocketLocation(FName("TraceEnd"));
			
			bool bHitted = UKismetSystemLibrary::SphereTraceMultiForObjects(this,TraceStart,TraceEnd,TraceRadius,TargetObjectTypes,
				false,ActorsToIgnore,EDrawDebugTrace::None,HitResults,true);
			if(bHitted)
			{
				for (auto Element : HitResults)
				{
					if(!HittedActors.Contains(Element.GetActor()))
					{
						HittedActors.AddUnique(Element.GetActor());
						if(IHitInterface* HitActor = Cast<IHitInterface>(Element.GetActor()))
						{
							HitActor->GetHit(WeaponDamage,Element.ImpactPoint,EHitType::EHT_NormalHit,GetOwner());
							if(I_OwnerHCharacter)
							{
								I_OwnerHCharacter->Interface_BrokeInvisibility();
							}
						}
					}
					else
					{
						return;
					}
				}
			}
		}
	}

}

