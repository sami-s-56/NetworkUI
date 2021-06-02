// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerWeapon.generated.h"

UENUM(BlueprintType)
enum class EProjectileType : uint8
{
	EPT_Bullet UMETA(DisplayName = "Bullet"),
	EPT_Grenade UMETA(DisplayName = "Grenade")
};

UENUM(BlueprintType)
enum class EFireMode : uint8
{
	EFM_SemiAuto UMETA(DisplayName = "SemiAuto"), 
	EFM_Auto UMETA(DisplayName = "Auto")
};

UCLASS()
class COOPSHOOTER_API APlayerWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* WeaponMesh;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UDamageType> WeaponDamageType;

	UPROPERTY(EditDefaultsOnly)
		EProjectileType ProjectileType;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* MuzzleFlash;

	UPROPERTY(VisibleAnywhere)
		FName EmmiterSocketName = "MuzzleFlash";

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* Tracer;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* BloodEffects;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* OtherEffects;

	UPROPERTY(VisibleAnywhere)
		FName EmmiterEnd = "Target";

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AGrenade> Grenade;

	UPROPERTY(EditDefaultsOnly)
		EFireMode FireMode = EFireMode::EFM_SemiAuto;

	bool bCanFire = true;	//To be set after fire intervals even in Semi Auto to stop spaming
	bool bIsAuto = false;
	bool bIsTriggerPressed = false;

	UPROPERTY(EditDefaultsOnly)
		float RateOfFire;

	float FireDelay; //(1 / Rate of fire)

	FTimerHandle FireTimer;
		

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void Fire();

	void FireBullet(AActor* Owner);

	void LaunchGrenade(AActor* Owner);

	UFUNCTION()
		void ResetFire();

	UFUNCTION()
		void ReleaseTrigger();

	UFUNCTION()
		TSubclassOf<UDamageType> GetDamageType() { return WeaponDamageType; }

	UFUNCTION()
		void SwitchFireMode();
};
