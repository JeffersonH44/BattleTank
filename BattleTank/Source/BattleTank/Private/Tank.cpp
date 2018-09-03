// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankAimingComponent.h"
#include "TankMovementComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Engine/World.h"
#include "Projectile.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("Aiming Component"));
	// TankMovementComponent = CreateDefaultSubobject<UTankMovementComponent>(FName("Movement Component"));
}

void ATank::AimAt(FVector HitLocation)
{
	// TODO: log aim at
	if (!TankAimingComponent) return;
	TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
}

void ATank::SetBarrelReference(UTankBarrel* Barrel)
{
	this->Barrel = Barrel;
}

void ATank::Fire()
{

	bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;
	if (!Barrel || !isReloaded) return;

	FVector ProjectileLocation = Barrel->GetSocketLocation(FName("Projectile"));
	FRotator ProjectileRotation = Barrel->GetSocketRotation(FName("Projectile"));
	// Spawn a projectile at the socket location
	auto Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileBlueprint, 
		ProjectileLocation, 
		ProjectileRotation
		);

	Projectile->LaunchProjectile(LaunchSpeed);
	
	LastFireTime = FPlatformTime::Seconds();
}

