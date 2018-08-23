// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"

#define OUT

void ATankPlayerController::BeginPlay() 
{
	Super::BeginPlay();
	auto TankController = GetControlledTank();
	if (!TankController) 
	{
		UE_LOG(LogTemp, Error, TEXT("No tank controller!!!"))
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("TankPlayerController possessing: %s"), *(TankController->GetName()))
	}
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
}

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());;
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetControlledTank()) return;
	
	FVector HitLocation(0.0);
	bool hits = GetSightRayHitLocation(OUT HitLocation);
	if (hits) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Look direction: %s"), *HitLocation.ToString())
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
		GetLookVectorHitLocation(LookDirection, OUT HitLocation);

		return true;
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


