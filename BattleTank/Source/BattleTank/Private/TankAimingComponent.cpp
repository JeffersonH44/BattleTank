// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "Engine/World.h"

#define OUT

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsBeginPlay = true;
	// ...
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();
	LastFireTime = GetWorld()->GetTimeSeconds();
}


// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	bool isReloading = (GetWorld()->GetTimeSeconds() - LastFireTime) < ReloadTimeInSeconds;

	if (isReloading) 
	{
		FiringState = EFiringState::Reloading;
	}
	else if (IsBarrelMoving()) 
	{
		FiringState = EFiringState::Aiming;
	}
	else 
	{
		FiringState = EFiringState::Locked;
	}
	// ...
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Barrel)) return;

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
	
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0.f,
		0.f,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);

	/*bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0.f,
		0.f,
		ESuggestProjVelocityTraceOption::DoNotTrace,
		FCollisionResponseParams::DefaultResponseParam,
		TArray<AActor*>(),
		true
	);*/

	if (bHaveAimSolution) 
	{
		AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
	}
}

void UTankAimingComponent::Initialize(UTankBarrel * Barrel, UTankTurret * Turret)
{
	if (!ensure(Barrel && Turret)) return;
	this->Barrel = Barrel;
	this->Turret = Turret;
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	if (!ensure(Barrel && Turret)) return;
	// Calculate delta rotator given the current barrel rotation and aim direction
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimRotator = AimDirection.Rotation();
	auto DeltaRotator = AimRotator - BarrelRotator;
	
	Barrel->Elevate(DeltaRotator.Pitch);
	auto Yaw = DeltaRotator.Yaw;
	Turret->Rotate(FMath::Abs(Yaw) < 180 ? Yaw : -Yaw);
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) return false;
	auto BarrelRotator = Barrel->GetForwardVector();
	return BarrelRotator.Equals(AimDirection, 0.01);
}

void UTankAimingComponent::Fire()
{
	if (!ensure(Barrel && ProjectileBlueprint) || (FiringState == EFiringState::Reloading)) return;

	FVector ProjectileLocation = Barrel->GetSocketLocation(FName("Projectile"));
	FRotator ProjectileRotation = Barrel->GetSocketRotation(FName("Projectile"));
	// Spawn a projectile at the socket location
	auto Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileBlueprint,
		ProjectileLocation,
		ProjectileRotation
		);

	Projectile->LaunchProjectile(LaunchSpeed);

	LastFireTime = GetWorld()->GetTimeSeconds();
}

EFiringState UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}

