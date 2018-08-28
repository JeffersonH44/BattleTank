// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Tank.h"
#include "Engine/World.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	auto ControlledTank = Cast<ATank>(GetPawn());
	if (PlayerTank) 
	{
		auto PlayerLocation = PlayerTank->GetActorLocation();
		// UE_LOG(LogTemp, Warning, TEXT("Player location at: %s"), *PlayerLocation.ToString())
		ControlledTank->AimAt(PlayerLocation);
		ControlledTank->Fire();
	}
}


