// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"


void UTankMovementComponent::IntendMoveForward(float Throw) 
{
	UE_LOG(LogTemp, Warning, TEXT("Intend move forward throw: %f"), Throw)

	if (!LeftTrack || !RightTrack) return;
	this->LeftTrack->SetThrottle(Throw);
	this->RightTrack->SetThrottle(Throw);
}

void UTankMovementComponent::IntendTurnRight(float Throw)
{

	if (!LeftTrack || !RightTrack) return;
	this->LeftTrack->SetThrottle(Throw);
	this->RightTrack->SetThrottle(-Throw);
}

void UTankMovementComponent::initialize(UTankTrack * LeftTrack, UTankTrack * RightTrack)
{
	if (!LeftTrack || !RightTrack) return;
	this->LeftTrack = LeftTrack;
	this->RightTrack = RightTrack;

	// TODO: prevent double speed due to dual control use
}
