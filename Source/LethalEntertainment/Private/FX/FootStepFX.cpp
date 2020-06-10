// Copyright 2018 - 2020 Stuart McDonald.

#include "LethalEntertainment.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "FootStepFX.h"

void AFootStepFX::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UPhysicalMaterial * HitPhysMat = SurfaceHit.PhysMaterial.Get();
	EPhysicalSurface HitSurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitPhysMat);

	/** Spawn impact sound */
	USoundBase * ImpactSound = GetFootStepSound(HitSurfaceType);
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}

	// Destroy actor
	Destroy();
}

USoundBase * AFootStepFX::GetFootStepSound(TEnumAsByte<EPhysicalSurface> SurfaceType) const
{
	USoundBase * FootStepSound = nullptr;

	switch (SurfaceType)
	{
		case SURFACE_DIRT:    FootStepSound = DirtSound;	break;
		case SURFACE_METAL:	  FootStepSound = MetalSound;	break;
		case SURFACE_ROCK:    FootStepSound = RockSound;	break;
		case SURFACE_SAND:    FootStepSound = SandBagSound; break;
		default:              FootStepSound = DefaultSound; break;
	}

	return FootStepSound;
}

