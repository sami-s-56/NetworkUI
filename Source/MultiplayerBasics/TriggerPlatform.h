// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerPlatform.generated.h"

UCLASS()
class MULTIPLAYERBASICS_API ATriggerPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggerPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "PlatformMesh")
		UStaticMeshComponent* TriggerMesh;

	UPROPERTY(EditDefaultsOnly, Category = "PlatformTrigger")
		class UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, Category = "ControlledPlatforms")
	TArray<class AMovingPlatform*> Platforms;

	UFUNCTION()
		void OnPlayerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnPlayerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
