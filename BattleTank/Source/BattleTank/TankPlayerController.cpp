// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "TankAimingComponent.h"

#define OUT

void ATankPlayerController::BeginPlay() 
{
	Super::BeginPlay();

	AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) return;
	FoundAimingComponent(AimingComponent);
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair()
{
	
	FVector HitLocation(0.0);
	bool hits = GetSightRayHitLocation(OUT HitLocation);
	if (hits) 
	{
		// UE_LOG(LogTemp, Warning, TEXT("Look direction: %s"), *HitLocation.ToString())
		AimingComponent->AimAt(HitLocation);
	}
}

/*
	Get world location of linetrace through crosshair, returns true if hits the landscape
*/
bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{
	HitLocation = FVector(1.0);
	// Find the crosshair position
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	FVector2D ScreenLocation(ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation);
	// UE_LOG(LogTemp, Warning, TEXT("Screen location: %s"), *ScreenLocation.ToString())

	// "De-project" the screen position of the crosshair to a world direction
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, OUT LookDirection)) 
	{
		// UE_LOG(LogTemp, Warning, TEXT("LookDirection to: %s"), *LookDirection.ToString())
		return GetLookVectorHitLocation(LookDirection, OUT HitLocation);
	}

	// Linetrace along that LookDirection, and see what we hit...
	return false;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
	FVector WorldLocation; // to fill the method (Not necessary)
	return DeprojectScreenPositionToWorld(
		ScreenLocation.X,
		ScreenLocation.Y,
		OUT WorldLocation,
		OUT LookDirection
	);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);

	if (GetWorld()->LineTraceSingleByChannel(
			OUT HitResult,
			StartLocation,
			EndLocation,
			ECollisionChannel::ECC_Visibility)
		) 
	{
		HitLocation = HitResult.Location;
		return true;
	}

	HitLocation = FVector(0.0);
	return false;
}


