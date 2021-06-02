// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "MovingPlatform.h"

// Sets default values
ATriggerPlatform::ATriggerPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("PlatformMesh"));
	TriggerMesh->SetRelativeScale3D(FVector(1.f, 1.f, 0.2f));
	RootComponent = TriggerMesh;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(FName("PlatformTrigger"));
	TriggerBox->SetupAttachment(TriggerMesh);
	TriggerBox->SetBoxExtent(FVector(50.f, 50.f, 100.f));
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATriggerPlatform::OnPlayerEnter);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ATriggerPlatform::OnPlayerExit);

}

// Called when the game starts or when spawned
void ATriggerPlatform::BeginPlay()
{
	Super::BeginPlay();

	for (AMovingPlatform* Platform : Platforms)
	{
		Platform->bUseTrigger = true;
		Platform->bEnabled = false;
	}
	
}

// Called every frame
void ATriggerPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATriggerPlatform::OnPlayerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("PlayerSteppedIn"));

	for (AMovingPlatform* Platform : Platforms)
	{
		Platform->bEnabled = true;
	}
}

void ATriggerPlatform::OnPlayerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("PlayerSteppedOut"));

	for (AMovingPlatform* Platform : Platforms)
	{
		Platform->bEnabled = false;
	}
}
