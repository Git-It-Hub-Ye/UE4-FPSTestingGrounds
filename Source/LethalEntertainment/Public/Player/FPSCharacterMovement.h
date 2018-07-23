// Fill out your copyright notice in the Description page of Project Settings.

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
