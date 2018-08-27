// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBarrel.h"
#include "Engine/World.h"

void UTankBarrel::Elevate(float RelativeElevationSpeed) 
{
	// Move the barrel the right amount this frame given a max elevation speed and the frame time

	RelativeElevationSpeed = FMath::Clamp<float>(RelativeElevationSpeed, -1, 1);
	auto ElevationChange = RelativeElevationSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto Elevation = FMath::Clamp<float>(RelativeRotation.Pitch + ElevationChange, MinElevationDegrees, MaxElevationDegrees);

	SetRelativeRotation(FRotator(Elevation, 0, 0));
}


