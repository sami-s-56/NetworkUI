// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class ECurrentWeapon : uint8
{
	ECW_Primary UMETA(DisplayName = "Primary"),
	ECW_Secondary UMETA(DisplayName = "Secondary"),
	ECW_None UMETA(DisplayName = "None")
};


UCLASS()
class COOPSHOOTER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnPrimary();

	void SpawnSecondary();


private:

	UPROPERTY(EditDefaultsOnly, Category="PlayerComponents")
	class USpringArmComponent* CharacterSpringArm;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerComponents")
	class UCameraComponent* PlayerCamera;

	UPROPERTY(EditDefaultsOnly)
		float CameraLerpSpeed = 5.f;

	bool bADS = false;

	UPROPERTY(EditDefaultsOnly)
		float ADS_Fov;

	float Normal_Fov;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class APlayerWeapon> PrimaryWeapon;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class APlayerWeapon> SecondaryWeapon;

	UPROPERTY(VisibleAnywhere)
		FName WeaponSocketName = "WeaponSocket";

	ECurrentWeapon CWeapon;
	class APlayerWeapon* CurrentWeapon;
	
	UFUNCTION()
	void MoveForward(float v);

	UFUNCTION()
	void MoveSideward(float v);

	UFUNCTION()
	void RotateHorizontal(float v);

	UFUNCTION()
	void RotateVertical(float v);

	UFUNCTION()
		void OnJump();

	UFUNCTION()
		void OnCrouch();

	UFUNCTION()
		void AimDownSights();

	UFUNCTION()
		void NormalAim();

	UFUNCTION()
		void FireWeapon();

	UFUNCTION()
		void StopFiring();

	UFUNCTION()
		void ToggleFire();


	virtual FVector GetPawnViewLocation() const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SwitchWeapon();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
