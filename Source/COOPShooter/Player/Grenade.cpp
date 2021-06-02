// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerWeapon.h"

// Sets default values
AGrenade::AGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GrenadeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grenade"));
	RootComponent = GrenadeMesh;

	GrenadeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GrenadeMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	GrenadeMesh->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);
	GrenadeMesh->SetGenerateOverlapEvents(true);
	GrenadeMesh->SetSimulatePhysics(true);

}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();
	
	GrenadeMesh->OnComponentBeginOverlap.AddDynamic(this, &AGrenade::OnHit);

	GrenadeMesh->AddImpulse((GetActorUpVector() * Impulse) + (GetActorForwardVector() * (Impulse / 20.f)));

	FTimerHandle UnusedHandle;
	GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &AGrenade::Explode, BlastDelay, false);
}

void AGrenade::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != GetOwner() && OtherActor != GetOwner()->GetOwner())
	{
		Explode();
	}
}

void AGrenade::Explode()
{ 
	AActor* Owner = GetOwner();
	if (Owner != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Owner->GetName());
		APlayerWeapon* Weapon = Cast<APlayerWeapon>(Owner);
		TArray<AActor*> IgnoreActors;
		
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Weapon->GetName());
		
		if (Weapon)
		{
			UE_LOG(LogTemp, Warning, TEXT("Cast Successfull"));
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, GetActorLocation());
			UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), 80.f, 20.f, GetActorLocation(), 500.f, 1200.f, 1.f, Weapon->GetDamageType(), IgnoreActors, Owner, Instigator->GetController(), ECC_Visibility);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Cast Failed"));
		}
	}

	this->Destroy();
}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

