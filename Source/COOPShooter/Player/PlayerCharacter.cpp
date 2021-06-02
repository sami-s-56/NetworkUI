// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "PlayerWeapon.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CharacterSpringArm->bUsePawnControlRotation = true;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	PlayerCamera->SetupAttachment(CharacterSpringArm);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	CWeapon = ECurrentWeapon::ECW_None;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerCamera)
	{
		Normal_Fov = PlayerCamera->FieldOfView;
	}

	SpawnPrimary();
}

void APlayerCharacter::SpawnPrimary()
{
	if (CWeapon != ECurrentWeapon::ECW_Primary)
	{
		FActorSpawnParameters SP;
		SP.Owner = this;
		SP.Instigator = this->GetInstigator();

		if (PrimaryWeapon)
		{
			if (CurrentWeapon != nullptr)
				CurrentWeapon->Destroy();
			CurrentWeapon = GetWorld()->SpawnActor<APlayerWeapon>(PrimaryWeapon, FVector::ZeroVector, FRotator::ZeroRotator, SP);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
			CWeapon = ECurrentWeapon::ECW_Primary;
		}
	}
}

void APlayerCharacter::SpawnSecondary()
{
	if (CWeapon != ECurrentWeapon::ECW_Secondary)
	{
		FActorSpawnParameters SP;
		SP.Owner = this;
		SP.Instigator = this->GetInstigator();

		if (SecondaryWeapon)
		{
			if (CurrentWeapon != nullptr)
				CurrentWeapon->Destroy();
			CurrentWeapon = GetWorld()->SpawnActor<APlayerWeapon>(SecondaryWeapon, FVector::ZeroVector, FRotator::ZeroRotator, SP);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
			CWeapon = ECurrentWeapon::ECW_Secondary;
		}
	}
}

void APlayerCharacter::FireWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Fire();
	}
}

void APlayerCharacter::StopFiring()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->ReleaseTrigger();
	}
}

void APlayerCharacter::MoveForward(float v)
{
	AddMovementInput(GetActorForwardVector(), v);
}

void APlayerCharacter::MoveSideward(float v)
{
	AddMovementInput(GetActorRightVector(), v);
}

void APlayerCharacter::RotateHorizontal(float v)
{
	AddControllerYawInput(v);
}

void APlayerCharacter::RotateVertical(float v)
{
	AddControllerPitchInput(v);
}

void APlayerCharacter::OnJump()
{
	Jump();
}

void APlayerCharacter::OnCrouch()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void APlayerCharacter::AimDownSights()
{
	bADS = true;
}

void APlayerCharacter::NormalAim()
{
	bADS = false;
}

FVector APlayerCharacter::GetPawnViewLocation() const
{
	if (PlayerCamera && CharacterSpringArm)
	{
		FVector location = PlayerCamera->GetComponentLocation() + (PlayerCamera->GetForwardVector() * CharacterSpringArm->TargetArmLength);
		return location;
	}
	
	return Super::GetPawnViewLocation();
	
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	float d_fov;

	if (PlayerCamera)
	{
		float c_fov = PlayerCamera->FieldOfView;
		if (bADS)
		{
			d_fov = FMath::FInterpTo(c_fov, ADS_Fov, DeltaTime, CameraLerpSpeed);
		}
		else
		{
			d_fov = FMath::FInterpTo(c_fov, Normal_Fov, DeltaTime, CameraLerpSpeed);
		}
		PlayerCamera->SetFieldOfView(d_fov);
	}

	
}

void APlayerCharacter::SwitchWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("Switching Weapon"));
	switch (CWeapon)
	{
	case ECurrentWeapon::ECW_Primary:
		SpawnSecondary();
		break;
	case ECurrentWeapon::ECW_Secondary:
		SpawnPrimary();
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Default Case"));
		break;
	}
}

void APlayerCharacter::ToggleFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->SwitchFireMode();
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Forward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Sideward", this, &APlayerCharacter::MoveSideward);

	PlayerInputComponent->BindAxis("VerticalRotation", this, &APlayerCharacter::RotateVertical);
	PlayerInputComponent->BindAxis("HorizontalRotation", this, &APlayerCharacter::RotateHorizontal);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::OnJump);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::OnCrouch);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &APlayerCharacter::AimDownSights);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &APlayerCharacter::NormalAim);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::FireWeapon);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::StopFiring);

	PlayerInputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &APlayerCharacter::SwitchWeapon);

	PlayerInputComponent->BindAction("SwitchFire", IE_Pressed, this, &APlayerCharacter::ToggleFire);
}

