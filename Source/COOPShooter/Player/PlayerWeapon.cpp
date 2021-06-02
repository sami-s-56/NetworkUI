// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Grenade.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "COOPShooter.h"

// Sets default values
APlayerWeapon::APlayerWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = WeaponMesh;

}

// Called when the game starts or when spawned
void APlayerWeapon::BeginPlay()
{
	Super::BeginPlay();

	FireDelay = 1.f / RateOfFire;
}

void APlayerWeapon::Fire()
{
	bIsTriggerPressed = true;
	AActor* Owner = GetOwner();
	if (Owner != nullptr)
	{
		if (bCanFire)
		{
			if (ProjectileType == EProjectileType::EPT_Bullet)
			{
				FireBullet(Owner);
			}
			if (ProjectileType == EProjectileType::EPT_Grenade)
			{
				LaunchGrenade(Owner);
			}
			bCanFire = false;

			GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &APlayerWeapon::ResetFire, FireDelay, false);

		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner is not set"));
	}
}

void APlayerWeapon::ResetFire()
{
	bCanFire = true;
	if (bIsAuto && bIsTriggerPressed)
	{
		Fire();
	}
}

void APlayerWeapon::ReleaseTrigger()
{
	bIsTriggerPressed = false;
}

void APlayerWeapon::SwitchFireMode()
{
	if (FireMode == EFireMode::EFM_Auto)
	{
		bIsAuto = !bIsAuto;
	}
}

void APlayerWeapon::FireBullet(AActor* Owner)
{
	
	if (Owner != nullptr)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector EndLocation = EyeLocation + (EyeRotation.Vector() * 10000);
		FHitResult Hit;

		FVector MuzzleLocation = WeaponMesh->GetSocketLocation(EmmiterSocketName);
		FVector TracerEndLocation = EndLocation;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(Owner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = false;	//Checks collision on triangle level, thus returning detailed info
		QueryParams.bReturnPhysicalMaterial = true;

		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, WeaponMesh, EmmiterSocketName);
		}

		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, EndLocation, ECC_Visibility, QueryParams))
		{

			UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *Hit.GetActor()->GetFName().ToString());
			TWeakObjectPtr < class UPhysicalMaterial > PhysMat = Hit.PhysMaterial;

			float damage;
			UParticleSystem* PS_ToPlay = nullptr;

			if (PhysMat != nullptr)
			{
				switch (PhysMat->SurfaceType)
				{
				case BODY_SURFACE:
					damage = 20.f;
					PS_ToPlay = BloodEffects;
					break;
				case HEAD_SURFACE:
					damage = 100.f;
					PS_ToPlay = BloodEffects;
					break;
				case WALL_SURFACE:
					damage = 0.f;
					PS_ToPlay = OtherEffects;
					UE_LOG(LogTemp, Warning, TEXT("FloorTakingDamage"));
					break;
				default:
					break;
				}
			}

			AActor* DamagedActor = Hit.GetActor();
			if (DamagedActor != nullptr)
			{
				UGameplayStatics::ApplyPointDamage(DamagedActor, damage, EyeRotation.Vector(), Hit, Owner->GetInstigatorController(), this, WeaponDamageType);
			}
			TracerEndLocation = Hit.Location;

			if (PS_ToPlay)
			{
				UParticleSystemComponent* DamageFX = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PS_ToPlay, Hit.Location);
			}

		}

		if (Tracer)
		{
			UParticleSystemComponent* PSTracer = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Tracer, MuzzleLocation);
			if (PSTracer)
			{
				PSTracer->SetVectorParameter(EmmiterEnd, TracerEndLocation);
				UE_LOG(LogTemp, Warning, TEXT("%s"), *TracerEndLocation.ToString());
			}
		}

		DrawDebugLine(GetWorld(), EyeLocation, TracerEndLocation, FColor::Emerald, 1.0, 1.0);
	}
}

void APlayerWeapon::LaunchGrenade(AActor* Owner)
{
	if (Owner != nullptr)
	{
		FVector LaunchLocation = WeaponMesh->GetSocketLocation(EmmiterSocketName);
		
		FVector EyeLocation;
		FRotator EyeRotation;
		Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FActorSpawnParameters SP;
		SP.Owner = this;
		SP.Instigator = Owner->GetInstigator();

		if (Grenade != nullptr)
		{
			AActor* SpawnedGrenade = GetWorld()->SpawnActor<AGrenade>(Grenade, LaunchLocation, EyeRotation, SP);
		}
	}
	
}



// Called every frame
void APlayerWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

