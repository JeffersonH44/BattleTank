// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTurret.h"
#include "Engine/World.h"

void UTankTurret::Rotate(float RelativeRotationSpeed) 
{
	// Rotates the turret on the yaw per game tick

	RelativeRotationSpeed = FMath::Clamp<float>(RelativeRotationSpeed, -1, 1);
	auto RotationChange = RelativeRotationSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	//auto Elevation = FMath::Clamp<float>(RelativeRotation.Pitch + ElevationChange, MinElevationDegrees, MaxElevationDegrees);
	auto Rotation = RelativeRotation.Yaw + RotationChange;

	SetRelativeRotation(FRotator(0, Rotation, 0));
}

