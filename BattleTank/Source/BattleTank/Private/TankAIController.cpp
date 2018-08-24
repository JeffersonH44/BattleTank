// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Tank.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	auto TankController = GetControlledTank();
	if (!TankController)
	{
		UE_LOG(LogTemp, Error, TEXT("No AI tank controller!!!"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TankAIController possessing: %s"), *(TankController->GetName()))
	}

	auto PlayerTank = GetPlayerTank();
	if (!PlayerTank) 
	{
		UE_LOG(LogTemp, Error, TEXT("No player controller found from: %s"), *(TankController->GetName()))
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("Aiming at: %s"), *(PlayerTank->GetName()))
	}
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto PlayerTank = GetPlayerTank();
	auto OurTank = GetControlledTank();
	if (!PlayerTank || !OurTank) return;

	OurTank->AimAt(PlayerTank->GetActorLocation());
}

ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());;
}

ATank* ATankAIController::GetPlayerTank() const
{
	auto PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return nullptr;

	return Cast<ATank>(PlayerController->GetPawn());
}


