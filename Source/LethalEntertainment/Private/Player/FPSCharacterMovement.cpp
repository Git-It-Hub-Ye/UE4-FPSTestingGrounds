// Copyright 2018 - 2020 Stuart McDonald.

#include "LethalEntertainment.h"
#include "Mannequin.h"
#include "FPSCharacterMovement.h"

float UFPSCharacterMovement::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

	const AMannequin * OwnerCharacter = Cast<AMannequin>(PawnOwner);

	if (OwnerCharacter)
	{
		if (OwnerCharacter->IsAiming())
		{
			MaxSpeed *= OwnerCharacter->GetAimingSpeedModifier();
		}
		if (OwnerCharacter->IsCrouching())
		{
			MaxSpeed *= OwnerCharacter->GetCrouchingSpeedModifier();
		}
	}
	return MaxSpeed;
}
