// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"

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

}


