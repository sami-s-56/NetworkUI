// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grenade.generated.h"

UCLASS()
class COOPSHOOTER_API AGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
		float Impulse;

	UPROPERTY(EditDefaultsOnly)
		float BlastDelay;

	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* GrenadeMesh;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* Explosion;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void Explode();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
