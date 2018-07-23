// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FXClass.h"
#include "FootStepFX.generated.h"

/**
 * 
 */
UCLASS()
class LETHALENTERTAINMENT_API AFootStepFX : public AFXClass
{
	GENERATED_BODY()
	
	
protected:
	/** Get sound for material type */
	virtual USoundBase * GetImpactSound(TEnumAsByte<EPhysicalSurface> SurfaceType) const override;

};
