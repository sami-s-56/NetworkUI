// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	SetReplicates(true);
	SetReplicateMovement(true);

	StartLocation = GetActorLocation();

	Destination = GetTransform().TransformPosition(Destination);

}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	if (HasAuthority())
	{
		if (!bUseTrigger || bEnabled)
		{
			//CAlculate Direction
			if (FVector::Dist(Destination, GetActorLocation()) > TargetOffset&& bIsGoing)
			{
				FVector DeltaPos = Destination - GetActorLocation();
				DeltaPos.Normalize();

				AddActorLocalOffset(DeltaPos * speed * DeltaTime);
			}
			else if (FVector::Dist(StartLocation, GetActorLocation()) > TargetOffset && !bIsGoing)
			{
				FVector DeltaPos = StartLocation - GetActorLocation();
				DeltaPos.Normalize();

				AddActorLocalOffset(DeltaPos * speed * DeltaTime);
			}
			else
			{
				bIsGoing = !bIsGoing;
			}
		}
	}
} 