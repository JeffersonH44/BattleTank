// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"


void UTankMovementComponent::IntendMoveForward(float Throw) 
{
	if (!ensure(LeftTrack && RightTrack)) return;
	this->LeftTrack->SetThrottle(Throw);
	this->RightTrack->SetThrottle(Throw);
}

void UTankMovementComponent::IntendTurnRight(float Throw)
{

	if (!ensure(LeftTrack && RightTrack)) return;
	this->LeftTrack->SetThrottle(Throw);
	this->RightTrack->SetThrottle(-Throw);
}

void UTankMovementComponent::initialize(UTankTrack * LeftTrack, UTankTrack * RightTrack)
{
	if (!ensure(LeftTrack && RightTrack)) return;
	this->LeftTrack = LeftTrack;
	this->RightTrack = RightTrack;
}

void UTankMovementComponent::RequestDirectMove(const FVector & MoveVelocity, bool bForceMaxSpeed)
{
	auto TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
	auto AIForwardIntention = MoveVelocity.GetSafeNormal();

	auto ThrowForward = FVector::DotProduct(TankForward, AIForwardIntention);
	auto ThrowRight = FVector::CrossProduct(TankForward, AIForwardIntention).Z;

	IntendMoveForward(ThrowForward);
	IntendTurnRight(ThrowRight);
	//UE_LOG(LogTemp, Warning, TEXT("%s with move velocity: %s"), *Name, *AIForwardIntention)
}
