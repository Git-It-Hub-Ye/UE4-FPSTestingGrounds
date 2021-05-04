// Copyright 2018 - 2021 Stuart McDonald.

#include "LethalEntertainment.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "ExplosionFX.h"

void AExplosionFX::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UPhysicalMaterial * HitPhysMat = SurfaceHit.PhysMaterial.Get();
	EPhysicalSurface HitSurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitPhysMat);

	/** Spawn impact fx */
	UParticleSystem * ImpactFX = GetExplosionFX(HitSurfaceType);
	if (ImpactFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactFX, GetActorLocation(), GetActorRotation());
	}

	// Destroy actor
	Destroy();
}

UParticleSystem * AExplosionFX::GetExplosionFX(TEnumAsByte<EPhysicalSurface> SurfaceType) const
{
	UParticleSystem * ExplosionFX = nullptr;

	switch (SurfaceType)
	{
	case SURFACE_SOFTMETAL:
	case SURFACE_METAL:            ExplosionFX = MetalFX; break;
	case SURFACE_DIRT:			   ExplosionFX = DirtFX; break;
	default:                       ExplosionFX = DefaultFX; break;
	}

	return ExplosionFX;
}

