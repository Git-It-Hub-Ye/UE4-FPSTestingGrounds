// Copyright 2018 - 2021 Stuart McDonald.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FPSCharacterMovement.generated.h"

UCLASS()
class LETHALENTERTAINMENT_API UFPSCharacterMovement : public UCharacterMovementComponent
{
	GENERATED_BODY()

private:
	/** Get max speed character can currently move */
	virtual float GetMaxSpeed() const override;
	
};
