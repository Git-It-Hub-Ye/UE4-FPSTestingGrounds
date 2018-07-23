// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FXClass.h"
#include "ExplosionFX.generated.h"

/**
 * 
 */
UCLASS()
class LETHALENTERTAINMENT_API AExplosionFX : public AFXClass
{
	GENERATED_BODY()
	
protected:
	/** Get fx for material type */
	virtual UParticleSystem * GetImpactFX(TEnumAsByte<EPhysicalSurface> SurfaceType) const override;
	
	
};
