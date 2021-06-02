// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERBASICS_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
	AMovingPlatform();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Movement")
	bool bUseTrigger = false;
	
	bool bEnabled = false;

protected:

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Destination", META = (MakeEditWidget = true))
	FVector Destination;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float speed;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float TargetOffset;

	UPROPERTY(VisibleAnywhere, Category = "Destination")
	FVector StartLocation;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	bool bIsGoing = true;

};
