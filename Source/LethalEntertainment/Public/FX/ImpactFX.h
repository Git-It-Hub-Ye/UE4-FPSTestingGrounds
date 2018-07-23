// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FXClass.h"
#include "ImpactFX.generated.h"

/**
 * 
 */
UCLASS()
class LETHALENTERTAINMENT_API AImpactFX : public AFXClass
{
	GENERATED_BODY()
	
	
protected:
	/** Get fx for material type */
	virtual UParticleSystem * GetImpactFX(TEnumAsByte<EPhysicalSurface> SurfaceType) const override;

	/** Get sound for material type */
	virtual USoundBase * GetImpactSound(TEnumAsByte<EPhysicalSurface> SurfaceType) const override;

	/** Get decal for material type */
	virtual UMaterial * GetImpactDecal(TEnumAsByte<EPhysicalSurface> SurfaceType) override;
	
};
